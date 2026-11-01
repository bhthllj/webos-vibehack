// ESP32 version of home-watch.py: when LG's home appears, open the launcher on top.
// Press Home twice within ESCAPE_MS to stay on LG's home.
#include <Arduino.h>
#include <WiFi.h>
#include <WebSocketsClient.h>
#include <ArduinoJson.h>
#include "secrets.h"

static const char *LAUNCHER = "com.gathin.tvhome";
static const char *LG_HOME = "com.webos.app.home";
static const uint32_t ESCAPE_MS = 3000;
static const uint32_t RETRY_MS = 10000;

// Unsigned manifest on purpose, same as lgtv.py: newer firmware rejects the shared signed one.
static const char *PERMISSIONS[] = {
  "LAUNCH", "LAUNCH_WEBAPP", "APP_TO_APP", "CLOSE", "TEST_OPEN", "TEST_PROTECTED", "CONTROL_AUDIO",
  "CONTROL_DISPLAY", "CONTROL_INPUT_JOYSTICK", "CONTROL_INPUT_MEDIA_RECORDING",
  "CONTROL_INPUT_MEDIA_PLAYBACK", "CONTROL_INPUT_TV", "CONTROL_POWER", "CONTROL_TV_SCREEN",
  "READ_APP_STATUS", "READ_CURRENT_CHANNEL", "READ_INPUT_DEVICE_LIST", "READ_NETWORK_STATE",
  "READ_RUNNING_APPS", "READ_TV_CHANNEL_LIST", "WRITE_NOTIFICATION_TOAST", "READ_POWER_STATE",
  "READ_COUNTRY_INFO", "CONTROL_INPUT_TEXT", "CONTROL_MOUSE_AND_KEYBOARD", "READ_INSTALLED_APPS",
  "READ_SETTINGS", "READ_STORAGE_DEVICE_LIST",
};

static WebSocketsClient ws;
static String clientKey = CLIENT_KEY;
static uint32_t lastRedirect = 0;   // millis() of the last redirect; 0 = none
static bool haveRedirected = false;

static void sendJson(JsonDocument &doc) {
  String out;
  serializeJson(doc, out);
  ws.sendTXT(out);
}

static void sendRegister() {
  JsonDocument doc;
  doc["type"] = "register";
  doc["id"] = "register_0";
  JsonObject p = doc["payload"].to<JsonObject>();
  p["forcePairing"] = false;
  p["pairingType"] = "PROMPT";
  JsonObject m = p["manifest"].to<JsonObject>();
  m["manifestVersion"] = 1;
  m["appVersion"] = "1.1";
  JsonArray perms = m["permissions"].to<JsonArray>();
  for (const char *perm : PERMISSIONS) perms.add(perm);
  if (clientKey.length()) p["client-key"] = clientKey;
  else Serial.println("No key yet: accept the prompt on the TV...");
  sendJson(doc);
}

static void subscribeForeground() {
  JsonDocument doc;
  doc["type"] = "subscribe";
  doc["id"] = "fg";
  doc["uri"] = "ssap://com.webos.applicationManager/getForegroundAppInfo";
  doc["payload"].to<JsonObject>();
  sendJson(doc);
}

static void launchLauncher() {
  JsonDocument doc;
  doc["type"] = "request";
  doc["id"] = "launch";
  doc["uri"] = "ssap://system.launcher/launch";
  doc["payload"]["id"] = LAUNCHER;
  sendJson(doc);
}

static void onForeground(const char *app) {
  if (!app || strcmp(app, LG_HOME) != 0) return;
  uint32_t now = millis();
  if (haveRedirected && now - lastRedirect < ESCAPE_MS) {
    Serial.println("Home pressed twice: staying on LG home");
    haveRedirected = false;
    return;
  }
  lastRedirect = now;
  haveRedirected = true;
  launchLauncher();
  Serial.println("LG home -> launcher");
}

static void onMessage(const char *text) {
  JsonDocument doc;
  if (deserializeJson(doc, text)) return;
  const char *type = doc["type"] | "";
  const char *id = doc["id"] | "";

  if (!strcmp(type, "registered")) {
    String key = doc["payload"]["client-key"] | "";
    if (key.length() && key != clientKey) {
      clientKey = key;
      Serial.println("\n=== Pairing key. Put it in secrets.h as CLIENT_KEY and flash again: ===");
      Serial.println(clientKey);
      Serial.println("===\n");
    }
    Serial.println("registered");
    subscribeForeground();
  } else if (!strcmp(type, "error")) {
    const char *err = doc["error"] | "?";
    Serial.printf("TV error (%s): %s\n", id, err);
    // A stale or rejected key: forget it so the next connection asks for pairing again.
    if (!strcmp(id, "register_0")) clientKey = "";
  } else if (!strcmp(id, "fg")) {
    onForeground(doc["payload"]["appId"] | (const char *)nullptr);
  }
}

static void onEvent(WStype_t type, uint8_t *payload, size_t length) {
  switch (type) {
    case WStype_CONNECTED:
      Serial.println("connected to TV");
      sendRegister();
      break;
    case WStype_DISCONNECTED:
      Serial.println("disconnected (TV off or standby?), retrying");
      break;
    case WStype_TEXT:
      onMessage((const char *)payload);
      break;
    default:
      break;
  }
}

static void connectWifi() {
  WiFi.mode(WIFI_STA);
  WiFi.setAutoReconnect(true);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.printf("Wi-Fi: connecting to %s", WIFI_SSID);
  uint32_t start = millis();
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (millis() - start > 30000) ESP.restart();   // router not up yet (e.g. after a power cut)
  }
  Serial.printf("\nWi-Fi up, IP %s\n", WiFi.localIP().toString().c_str());
}

void setup() {
  Serial.begin(115200);
  delay(300);
  connectWifi();
  // No CA is given, so the library does not verify the TV's self-signed certificate (same as lgtv.py).
  ws.beginSSL(TV_HOST, 3001, "/");
  ws.onEvent(onEvent);
  ws.setReconnectInterval(RETRY_MS);
  ws.enableHeartbeat(10000, 5000, 2);   // detects a TV that dropped off without closing
}

void loop() {
  ws.loop();
}
