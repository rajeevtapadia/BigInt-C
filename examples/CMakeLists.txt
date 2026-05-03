file(GLOB EXAMPLE_SOURCES *.c)

foreach(src ${EXAMPLE_SOURCES})
    get_filename_component(name ${src} NAME_WE)

    add_executable(${name} ${src})
    target_link_libraries(${name} PRIVATE bigint)

    # optional: nicer output dir
    set_target_properties(${name} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/examples
    )
endforeach()
