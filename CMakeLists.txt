# Generated Cmake Pico project file

# --- STEP 1 (REQUIRED, must be the first real command in the file) ---
# Declares the minimum CMake version this file needs. CMake refuses to
# process anything below this line without it.
cmake_minimum_required(VERSION 3.13)

# --- STEP 2 (OPTIONAL but standard convention) ---
# Language standard + tooling settings. Not required for a build to work,
# but C11/C++17 are the conventional choices for Pico SDK projects, and
# COMPILE_COMMANDS is what gives VS Code's IntelliSense accurate info.
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# --- STEP 3 (REQUIRED — but auto-generated/managed, not hand-written) ---
# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
# This block just locates which SDK/toolchain/picotool versions are
# installed on this machine and wires them in. The extension regenerates
# this if you change SDK version through its UI — never hand-edit it.
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.3.0)
set(toolchainVersion 15_2_Rel1)
set(picotoolVersion 2.3.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================

# --- STEP 4 (REQUIRED, must come before pico_sdk_import.cmake below) ---
# Tells the SDK which physical board this is being compiled for — this is
# what makes WiFi/CYW43439 support available later, since that's tied to
# the pico2_w board definition specifically (plain "pico2" has no radio).
set(PICO_BOARD pico2_w CACHE STRING "Board type")

# --- STEP 5 (REQUIRED, must come before project()) ---
# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

# --- STEP 6 (REQUIRED) ---
# Standard CMake project declaration. C CXX ASM = this project compiles
# C, C++, and assembly files (the SDK itself uses all three internally).
project(Productivity C CXX ASM)

# --- STEP 7 (REQUIRED, must come right after project()) ---
# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# --- STEP 8 (REQUIRED) ---
# Add executable. Default name is the project name, version 0.1
# Defines the build target. If you split code into multiple .c files
# later, they all get listed here (e.g. Productivity.c wifi.c button.c).
add_executable(Productivity Productivity.c )

# --- STEP 9 (OPTIONAL) ---
# Metadata only — shows up in `picotool info` and USB descriptors. Doesn't
# affect how the program runs.
pico_set_program_name(Productivity "Productivity")
pico_set_program_version(Productivity "0.1")

# --- STEP 10 (OPTIONAL, but you'll want USB on to see printf output) ---
# Modify the below lines to enable/disable output over UART/USB
# Controls where printf()/stdio goes: physical UART pins, or the USB
# port you're already plugged in with. Currently both OFF — flip the USB
# one to 1 when we're ready to test and read output over serial monitor.
pico_enable_stdio_uart(Productivity 0)
pico_enable_stdio_usb(Productivity 1)

# --- STEP 11 (REQUIRED — pico_stdlib at minimum) ---
# Add the standard library to the build
# Links the libraries your code calls into. pico_stdlib is the baseline
# (gpio, stdio, time). More get added here as you need them — e.g. WiFi
# later means adding pico_cyw43_arch_lwip_threadsafe_background or similar.

target_link_libraries(Productivity
pico_cyw43_arch_lwip_threadsafe_background
pico_stdlib
)
 

# --- STEP 12 (OPTIONAL — only needed if you add your own .h files) ---
# Add the standard include files to the build
target_include_directories(Productivity PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)

# --- STEP 14 (REQUIRED for flashing) ---
# Generates the .uf2 file (and .hex/.bin/.map) — the .uf2 is what you drag
# onto the Pico in BOOTSEL mode, or what the extension flashes for you.
pico_add_extra_outputs(Productivity)

