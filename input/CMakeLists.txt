add_library(input_parser STATIC
    parser.c
    parser.h
)

# Open header files for using
target_include_directories(input_parser PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../package_manager
    ${CMAKE_CURRENT_SOURCE_DIR}/../ui
    ${CMAKE_CURRENT_SOURCE_DIR}/../utils
)

target_link_libraries(input_parser PUBLIC package_manager ui utils)

# Debug
target_compile_options(input_parser PRIVATE -g)