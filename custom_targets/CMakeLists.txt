if(MBED_TARGET STREQUAL "TARGET_HK_TDM_F446RE")

   add_library(mbed-target-hk-tdm-f446re INTERFACE)

   #mbed_disable_mcu_target_file(mbed-stm32f4 system_clock.c)
   #target_sources(mbed-target-hk-tdm-f446re INTERFACE TARGET_HK_TDM_F446RE/system_clock.c)

   target_sources(mbed-target-hk-tdm-f446re INTERFACE TARGET_HK_TDM_F446RE/PeripheralPins.c)

   target_include_directories(mbed-target-hk-tdm-f446re INTERFACE TARGET_HK_TDM_F446RE)

   target_link_libraries(mbed-target-hk-tdm-f446re INTERFACE mbed-stm32f446xe)

endif()

