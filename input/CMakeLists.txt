add_library(input_parser STATIC
    parser.c
    parser.h
)

# Open header files for using
target_include_directories(input_parser PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../package_manager
)

target_link_libraries(input_parser PUBLIC package_manager)

# Debug
target_compile_options(input_parser PRIVATE -g)