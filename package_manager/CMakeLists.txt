add_library(package_manager STATIC
    detector.c
    optimizer.c
)

target_include_directories(package_manager PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_options(package_manager PRIVATE -g)