add_library(utils STATIC
    utils.c
    utils.h
)

# Open header files for using
target_include_directories(utils PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../ui
)

target_link_libraries(utils PUBLIC ui)

# Debug
target_compile_options(utils PRIVATE -g)