# How to use in example-esp

In the [example-esp](https://github.com/ubirch/example-esp32/tree/NFC)
project, go to the branch NFC and refresh the submodules, by using
```bash
git submodule update --init --recursive
```

The modules will be included in the console, by the definition in the [CmakeLists.txt](https://github.com/ubirch/example-esp32/blob/NFC/CMakeLists.txt#L26)

## esp-pn532
Port of the Adafruit PN532 Library for Arduino to ESP32
