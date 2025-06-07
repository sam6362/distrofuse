add_library(ui STATIC
    ui.c
    ui.h
)

# Open header files for using
target_include_directories(ui PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Debug
target_compile_options(ui PRIVATE -g)