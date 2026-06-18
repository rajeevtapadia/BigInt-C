file(GLOB_RECURSE TEST_SOURCES *.c)

foreach(src ${TEST_SOURCES})
    get_filename_component(name ${src} NAME_WE)

    add_executable(${name} ${src})
    target_link_libraries(${name} PRIVATE bigint -fsanitize=address)

    target_compile_options(${name} PRIVATE
        -Wall
        -Wextra
        -ggdb
        -fsanitize=address
    )

    add_link_options(
        -fsanitize=address
    )

    set_target_properties(${name} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/tests
    )

    add_test(NAME ${name} COMMAND ${name})
endforeach()
