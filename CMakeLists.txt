# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.0)
set(toolchainVersion 13_3_Rel1)
set(picotoolVersion 2.1.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico_w CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(embarcatech-interfaces-de-comunicacao C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(embarcatech-interfaces-de-comunicacao embarcatech-interfaces-de-comunicacao.c ssd1306.c )
pico_generate_pio_header(embarcatech-interfaces-de-comunicacao ${CMAKE_CURRENT_LIST_DIR}/matriz_leds.pio)

pico_set_program_name(embarcatech-interfaces-de-comunicacao "embarcatech-interfaces-de-comunicacao")
pico_set_program_version(embarcatech-interfaces-de-comunicacao "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(embarcatech-interfaces-de-comunicacao 1)
pico_enable_stdio_usb(embarcatech-interfaces-de-comunicacao 1)

# Add the standard library to the build
target_link_libraries(embarcatech-interfaces-de-comunicacao
        pico_stdlib)

# Add the standard include files to the build
target_include_directories(embarcatech-interfaces-de-comunicacao PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)

# Add any user requested libraries
target_link_libraries(embarcatech-interfaces-de-comunicacao 
        pico_bootrom
        hardware_clocks
        hardware_adc
        hardware_pio
        hardware_i2c
        
        )

pico_add_extra_outputs(embarcatech-interfaces-de-comunicacao)

