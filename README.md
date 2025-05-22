# ESP-PN532 NFC Component for ESP-IDF

This component provides a driver for the NXP PN532 NFC/RFID transceiver, primarily for use with the ESP32 series of microcontrollers and the ESP-IDF framework. It communicates with the PN532 via I2C.

**This version has been tested with ESP-IDF v5.1 - v5.3 on the ESP32-S3.**

## Features

*   Communication via I2C.
*   Support for ISO14443A, Mifare Classic, Mifare Ultralight, and NTAG2xx cards.
*   Firmware version checking.
*   GPIO control (P3 pins) via `readGPIO` and `writeGPIO`.
*   SAM (Secure Access Module) configuration.
*   Includes an `nfc_console` utility for basic command-line interaction (reading/writing Mifare Ultralight pages).
*   Configurable via `menuconfig` for debug output and specific operational modes.

## Dependencies

This component relies on the following ESP-IDF components:
*   `driver` (for I2C and GPIO)
*   `nvs_flash` (though direct usage in component is minimal, listed as a general dependency)
*   `console` (for the `nfc_console` utility)
*   `argtable3` (for parsing arguments in `nfc_console`)

## Integration into Your Project

You can integrate this component into your ESP-IDF project in a couple of ways:

**1. As a Git Submodule (Recommended for development):**

   This is the current method described and is useful if you're actively developing this component or want to pin it to a specific commit.

   ```bash
   # Navigate to your ESP-IDF project's root directory
   git submodule add https://github.com/ElPernotador/esp-pn532.git components/esp-pn532
   git submodule update --init --recursive
   ```
   Ensure your main project's `CMakeLists.txt` knows where to find components (e.g., `set(COMPONENTS_DIRS components)`).

**2. As a Managed Component (Future/Alternative):**

   If this component were published to the [ESP-IDF Component Registry](https://components.espressif.com/), or if you place it in a local `components` directory that your project's `idf_component.yml` manifest can see, you could add it via its manifest file.
   For example, if you copied this component into `my_project/components/esp-pn532`, your project's `idf_component.yml` might look like:

   ```yaml
   dependencies:
     # ... other dependencies
     ElPernotador/esp-pn532: "*" # Or specify a version
     # If using a local copy:
     # esp-pn532:
     #   path: components/esp-pn532 
     #   version: "*" 
   ```
   (The exact syntax for local components might vary; refer to ESP-IDF documentation.)

After adding the component, it should be automatically picked up by the build system.

## Configuration

Once the component is added to your project, configure it via `menuconfig`:

1.  Open the project configuration:
    ```bash
    idf.py menuconfig
    # or for PlatformIO:
    # pio run -t menuconfig
    ```
2.  Navigate to `Component config` ---> `PN532 NFC Component Configuration`.
3.  Set the desired options:

    *   **`Enable PN532 NFC Component`**: (Default: `y`)
        *   Enable or disable the entire component.
    *   **`Enable PN532 Debug Output`**: (Default: `n`)
        *   Enables detailed debug messages from the core PN532 driver.
    *   **`Enable Mifare Debug Output`**: (Default: `n`)
        *   Enables debug messages specific to Mifare card operations.
    *   **`Enable IRQ Debug Output`**: (Default: `n`)
        *   Enables debug messages for IRQ handling.
    *   **`Enable IRQ ISR handling`**: (Default: `y`)
        *   Use interrupt service routine for the IRQ pin. If disabled, the driver will use polling to check for card presence/readiness, which might be less efficient.
    *   **`Use External I2C Driver`**: (Default: `n`)
        *   Enable this if you are initializing and managing the I2C bus driver externally to this component. If enabled, this component will not perform `i2c_driver_install` or `i2c_param_config`.

## `nfc_console` Utility

This component includes a simple command-line utility that registers an `nfc` command with ESP-IDF's system console. It can be used for basic interaction with Mifare Ultralight tags.

*   **`nfc r <page>`**: Reads a specific page from a Mifare Ultralight tag.
    *   Example: `nfc r 4`
*   **`nfc w <page> <value>`**: Writes a 4-byte hex value to a specific page.
    *   Example: `nfc w 4 11223344`

To use this, ensure you have console support enabled in your project (e.g., via `CONFIG_ESP_CONSOLE_ UART_DEFAULT_EN`).

## Note on ESP32-S3 I2C Timeout (40 MHz Crystal)

When using the ESP32-S3 with its 40 MHz crystal as the SCLK source for I2C, the hardware timeout value for I2C operations has specific characteristics. The `PN532.h` file defines `ESP32S3_40MH_TIMEOUT` (set to `22`) which is used with `i2c_set_timeout`. This value corresponds to an I2C SCL timeout of approximately 104.9 ms. This is generally suitable for PN532 communication.

```c
// From PN532.h, used in init_PN532_I2C()
#define ESP32S3_40MH_TIMEOUT 22
```

## Troubleshooting

*   **Ensure Correct Wiring:** Double-check your I2C (SDA, SCL) and control pins (Reset, IRQ - if used) between the ESP32 and PN532.
*   **I2C Pull-ups:** Ensure appropriate pull-up resistors are present on the I2C lines.
*   **ESP-IDF Version:** This library is tested with ESP-IDF v5.1-v5.3. Compatibility with other versions is not guaranteed.
*   **Enable Debug Logs:** If you encounter issues, enable the debug options in `menuconfig` to get more detailed output from the driver.
