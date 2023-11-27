# PN532 I2C ESP-IDF 5.1 Module

**Tested only on ESP32 S3**

To add compatibility with other microcontrollers, you can edit the `PN532.h` file.

## Note on Using the 40 MHz Crystal as SCLK

When using the 40 MHz crystal as SCLK, the timeout value is limited to a maximum of 2^(22 * 25) ns, which is approximately 104.9 ms.

```c
#define ESP32S3_40MH_TIMEOUT 22 // 2^SCLK_period
```

## How to use as module

```bash
git submodule add -f https://github.com/ElPernotador/esp-pn532.git components/esp-pn532 
git submodule update --init --recursive
```
