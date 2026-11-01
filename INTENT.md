# Intent: esphome-sensors

Sensor nodes for Home Assistant, using ESP32 boards and ESPHome. **Not part of the TV launcher
project**: this branch only parks the first draft until it gets its own repo.

## Plan
- Measure temperature and water level, report to Home Assistant over Wi-Fi.
- Board: ESP32-C3 (SuperMini 4 MB clones for cheap nodes; Seeed XIAO ESP32-C3 where signal is weak,
  it has a U.FL antenna connector). C6 only if Zigbee/Thread is wanted later.
- Display: 0.96" SSD1306 I2C OLED (4-pin version, 0x3C) for local readings.

## Done
- `esphome/temp-node.yaml`: DS18B20 on GPIO4, OLED on GPIO6/7, Wi-Fi signal sensor, float switch
  block commented out. Passes `esphome config`; **not compiled or flashed**.
- `esphome/secrets.yaml.example`.

## Open
- Order boards (test 5-10 first), DS18B20 probes, 4.7k resistors, cases (plastic, IP65 if damp),
  cable glands, float switches, USB-C data cables, power adapters, a multimeter.
- Check the sensors' data line voltage: ESP32 GPIO is **3.3 V**, max ~3.6 V. Divide anything higher.
- Water level: float switch (digital) first; for continuous level use a pressure transducer
  (0.5-4.5 V, divide down) with an ADS1115, or ultrasonic (JSN-SR04T, 5 V echo, divide down).
- Compile and flash one node; check the log lists the OLED at 0x3C.
- Add `output_power: 8.5dB` (already set) and confirm Wi-Fi stability on the real boards.
- Move this into its own repo and drop it from the TV fork.

## Notes
- Keep analog sensors on GPIO0-4 on the C3 (GPIO5 is ADC2, unreliable with Wi-Fi).
- OLED draws ~10-20 mA and can burn in: turn it off after a few seconds if it stays on for years.
