# ESP32 Home watcher

Does what [`home-watch.py`](../home-watch.py) does, on a small always-on ESP32: when LG's home
appears, it opens the launcher on top. Press Home twice within 3 seconds to stay on LG's home.

Any ESP32 dev board with Wi-Fi works (ESP32-WROOM / DevKitC, ESP32-S3 and C3 too, though the
`platformio.ini` board line is for the classic `esp32dev`). Power it from a USB charger near the TV.

1. `cp src/secrets.h.example src/secrets.h` and fill in the Wi-Fi name, password and TV IP.
2. Give the TV a fixed IP (DHCP reservation in the router).
3. `pio run -t upload && pio device monitor` (PlatformIO, `pip install platformio`).
4. First run, with `CLIENT_KEY` empty: accept the prompt on the TV. The serial monitor prints a key.
   Put it in `secrets.h` and flash again. (Or copy the key for the TV's IP from `../.lgtv-key.json`.)

Serial log you should see: `Wi-Fi up` → `connected to TV` → `registered`. Press Home on the remote:
`LG home -> launcher`. Turn the TV off and on: it logs `disconnected`, then reconnects within ~10 s.
