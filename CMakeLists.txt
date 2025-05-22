idf_component_register(SRCS "PN532.c" "nfc_console.c"
                       INCLUDE_DIRS "."
                       REQUIRES driver nvs_flash console)
