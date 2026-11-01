// Launcher configuration. Edit this file, then run webos-launcher/install.sh.
// App ids: run `python3 sync-apps.py --list` (or look in app/apps.js after a sync).
window.CONFIG = {
  // Apps pinned to the dock row at the bottom, in this order. Missing apps are skipped.
  // Each entry is an app id, or a word from the app's title (case-insensitive) if the id is unknown.
  // Check ids with: venv/bin/python sync-apps.py $TV_HOST --list
  dockApps: ['hdp.opapp', 'nimitv', 'youtube.leanback.v4', 'netflix', 'amazon'],

  // Names shown on the tiles instead of the TV's own titles. Keyed by the same entries as dockApps.
  titles: { 'hdp.opapp': 'Satellit', 'nimitv': 'Shqip' },

  // Apps never shown as tiles (inputs and Live TV are already in the Inputs menu).
  hiddenApps: [
    'com.gathin.tvhome',
    'com.webos.app.livetv',
    'com.webos.app.hdmi1', 'com.webos.app.hdmi2', 'com.webos.app.hdmi3', 'com.webos.app.hdmi4',
    'com.webos.app.lgchannels',   // LG Channels
    'shortcutcopilot',            // Copilot
    'com.miniclip.eightballpool', // 8 Ball Pool
    'amazon.alexa.view'           // Amazon Alexa
  ],

  // The Live TV app, offered at the end of the Inputs menu. Leave empty to omit.
  liveTvApp: 'com.webos.app.livetv',

  // Tiles per row in the grid; the dock always has this many slots.
  columns: 4
};
