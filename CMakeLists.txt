cmake_minimum_required(VERSION 3.19)
cmake_policy(VERSION 3.19...3.22)

set(PROJECT hk_tdm_m4_app)

set(CUSTOM_TARGETS_JSON_PATH custom_targets/custom_targets.json5)
set(MBED_APP_JSON_PATH mbed_app.json5)

include(mbed-os/tools/cmake/mbed_toolchain_setup.cmake)

project(${PROJECT} LANGUAGES C CXX ASM) 

include(mbed_project_setup)

add_subdirectory(custom_targets)
add_subdirectory(mbed-os)

add_executable(${PROJECT})

target_compile_definitions(${PROJECT} PRIVATE "__LITTLE_ENDIAN__")
target_compile_definitions(${PROJECT} PRIVATE "__ARM_M4__")
target_compile_definitions(${PROJECT} PRIVATE "PRINTF_IS_ARM_PRINTF")

target_compile_options(mbed-os PRIVATE
    $<$<COMPILE_LANGUAGE:C>:-Wno-unused-but-set-variable>
    $<$<COMPILE_LANGUAGE:C>:-Wno-unused-function>
    $<$<COMPILE_LANGUAGE:C>:-Wno-cpp>
)

target_sources(${PROJECT} PUBLIC
   ${CMAKE_CURRENT_LIST_DIR}/src/hk_tdm_v1_main.cpp
   ${CMAKE_CURRENT_LIST_DIR}/src/COPacketCmdHkTdm.cpp
   ${CMAKE_CURRENT_LIST_DIR}/src/HkTdmCtrl.cpp
   ${CMAKE_CURRENT_LIST_DIR}/src/HkTdmErrors.cpp
   ${CMAKE_CURRENT_LIST_DIR}/lib/redlibs/common/protocols/COPacket/COPacket.cpp
   ${CMAKE_CURRENT_LIST_DIR}/lib/redlibs/targets/m4/mbed-os/controllers/IICController.cpp
)

target_include_directories(${PROJECT} PUBLIC 
   ${CMAKE_CURRENT_LIST_DIR}/include
   ${CMAKE_CURRENT_LIST_DIR}/lib/redlibs
   ${CMAKE_CURRENT_LIST_DIR}/lib/redlibs/targets
)

# NUCLEO_F446RE
if(MBED_TARGET STREQUAL "NUCLEO_F446RE")
   target_sources(mbed-os INTERFACE
      ${CMAKE_CURRENT_LIST_DIR}/mbed-os/targets/TARGET_STM/TARGET_STM32F4/clock_cfg/TARGET_STM32F4_180MHZ/system_clock.c
   )
endif()

target_link_libraries(${PROJECT} mbed-os)

mbed_set_post_build(${PROJECT})
