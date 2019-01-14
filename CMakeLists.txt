set(COMPONENT_SRCS
        PN532.c
        nfc_console.c
        )
set(COMPONENT_ADD_INCLUDEDIRS "include")

set(COMPONENT_REQUIRES
        )
set(COMPONENT_PRIV_REQUIRES
        nvs_flash
        console
        )


register_component()
