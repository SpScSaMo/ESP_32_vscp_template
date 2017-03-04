deps_config := \
	/Users/gerhard/esp/esp-idf/components/bt/Kconfig \
	/Users/gerhard/esp/esp-idf/components/esp32/Kconfig \
	/Users/gerhard/esp/esp-idf/components/ethernet/Kconfig \
	/Users/gerhard/esp/esp-idf/components/freertos/Kconfig \
	/Users/gerhard/esp/esp-idf/components/log/Kconfig \
	/Users/gerhard/esp/esp-idf/components/lwip/Kconfig \
	/Users/gerhard/esp/esp-idf/components/mbedtls/Kconfig \
	/Users/gerhard/esp/esp-idf/components/openssl/Kconfig \
	/Users/gerhard/esp/esp-idf/components/spi_flash/Kconfig \
	/Users/gerhard/esp/esp-idf/components/bootloader/Kconfig.projbuild \
	/Users/gerhard/esp/esp-idf/components/esptool_py/Kconfig.projbuild \
	/Users/gerhard/esp/esp-idf/components/partition_table/Kconfig.projbuild \
	/Users/gerhard/esp/esp-idf/Kconfig

include/config/auto.conf: \
	$(deps_config)


$(deps_config): ;
