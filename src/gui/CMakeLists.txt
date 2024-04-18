find_package(SDL2 REQUIRED)

set(SOURCES
    main.c
    Window.c
)

add_executable(geogebra_klone ${SOURCES})
target_include_directories(geogebra_klone 
    PUBLIC ${HEADER_DIR}/gui
)
target_link_libraries(geogebra_klone 
    PUBLIC SDL2::SDL2
    PUBLIC render
    PUBLIC expressions
)
