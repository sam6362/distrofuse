cmake_minimum_required(VERSION 3.15)
project(distfuse DESCRIPTION "DistFuse Universal Package Manager" LANGUAGES C)

# Project Version
set(DISTFUSE_VERSION "1.0.0")

# Debug: Print current directory
message(STATUS "CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR}")

# Compiler Options
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_FLAGS "-Wall -Wextra -Werror -O2")

# Check if directories exist and add them as subdirectories
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/UI")
    message(STATUS "UI directory found")
    add_subdirectory(UI)
elseif(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/ui")
    message(STATUS "ui directory found")
    add_subdirectory(ui)
else()
    message(FATAL_ERROR "UI directory not found!")
endif()

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/utils")
    message(STATUS "utils directory found")
    add_subdirectory(utils)
else()
    message(FATAL_ERROR "utils directory not found!")
endif()

# Sub directories
add_subdirectory(input)
add_subdirectory(package_manager)

# Main executable file
add_executable(distfuse main.c)

# Derleme zamanında sürüm makrosunu tanımla
# Bu satır 'add_executable' sonrasında yer almalı
target_compile_definitions(distfuse PRIVATE
    DISTFUSE_APP_VERSION="${DISTFUSE_VERSION}"
)

# Links - Make sure target names match exactly
target_link_libraries(distfuse PRIVATE 
    input_parser
    package_manager
    ui
    utils
)

# Header files paths
target_include_directories(distfuse PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/input
    ${CMAKE_CURRENT_SOURCE_DIR}/package_manager
    ${CMAKE_CURRENT_SOURCE_DIR}/ui
    ${CMAKE_CURRENT_SOURCE_DIR}/utils
)

# Installation options
install(TARGETS distfuse DESTINATION bin)