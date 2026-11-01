# Intent: main

Fork of [webos-vibehack](https://github.com/bhthllj/webos-vibehack) for the family's LG TV
(192.168.0.109). **Scope: the TV launcher and its Home-button watcher only.** Sensor work lives
on the `esphome-sensors` branch.

## Done
- Launcher translated to simple Albanian (greeting, days, months, Hyrjet, Cilësimet, toasts), 24-hour clock.
- Dock: Satellit (HD+, `hdp.opapp`), Shqip (`nimitv`), YouTube, Netflix; Prime Video starts row 2.
  Tile names are overridden in `app/config.js` (`titles`); dock entries match by id or title.
- Hidden: 8 Ball Pool, Amazon Alexa (still installed on the TV).
- Installed on the TV with `TV_HOST=192.168.0.109 ./install.sh`.
- `esp32-watcher/`: ESP32 port of `home-watch.py` (PlatformIO, compiles; untested on hardware).

## Open
- Push to GitHub: needs `gh auth login` (the push failed with no credentials). 5+ commits are local only.
- Number key shortcut: assign **Home** (the launcher) to a number key on the remote (hold the key).
- Home button redirect: run the watcher on an always-on device (ESP32 once the board arrives, or a Pi).
  Give the TV a DHCP reservation first.
- ESP32 watcher: flash, pair (key printed on serial, or from `.lgtv-key.json`), check log
  `Wi-Fi up` → `connected to TV` → `registered` → `LG home -> launcher`.
- Developer Mode session expires: press Extend Session Time in the TV's Developer Mode app now and then.

## Decisions
- Only the classic ESP32 or C3 for the watcher; the C6 is not supported by stock PlatformIO yet.
- Wi-Fi power on SuperMini-type boards may need lowering if the connection drops.
