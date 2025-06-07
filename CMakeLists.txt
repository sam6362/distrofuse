cmake_minimum_required(VERSION 3.15)
project(distfuse DESCRIPTION "DistFuse Universal Package Manager" LANGUAGES C)

# Compiler Options
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_FLAGS "-Wall -Wextra -Werror -O2")

# Sub directories
add_subdirectory(input)
#add_subdirectory(UI)
add_subdirectory(package_manager)

# Main executable file
add_executable(distfuse main.c)

# Liks
target_link_libraries(distfuse PRIVATE 
    input_parser
    package_manager
)

# Header files paths
target_include_directories(distfuse PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/input
    ${CMAKE_CURRENT_SOURCE_DIR}/package_manager
)

# Installation options
install(TARGETS distfuse DESTINATION bin)