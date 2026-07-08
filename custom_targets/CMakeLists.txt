
add_library(mbed-target-hk-tdm-f446re INTERFACE)

#TARGET_STM32F4_180MHZ
#mbed_disable_mcu_target_file(mbed-stm32f446xe system_clock.c)
mbed_disable_mcu_target_file(mbed-stm32f4 system_clock.c)

target_sources(mbed-target-hk-tdm-f446re INTERFACE TARGET_HK_TDM_F446RE/PeripheralPins.c)
target_sources(mbed-target-hk-tdm-f446re INTERFACE TARGET_HK_TDM_F446RE/system_clock.c)

target_include_directories(mbed-target-hk-tdm-f446re INTERFACE TARGET_HK_TDM_F446RE)

target_link_libraries(mbed-target-hk-tdm-f446re INTERFACE mbed-stm32f446xe)

#target_link_libraries(mbed-target-hk-tdm-f446re INTERFACE mbed-nucleo-f446re)

