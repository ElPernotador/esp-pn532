file(GLOB_RECURSE SRCS "PN532.c" "*.c" "*.cpp")

file(GLOB_RECURSE HDRS "*.h")

idf_component_register(
    SRCS ${SRCS} ${HDRS}
    INCLUDE_DIRS .
    REQUIRES
        driver
        nvs_flash
        console
)
