set(SOURCES
    PlotData.c
    Primitives.c
    Render.c
)

add_library(render STATIC ${SOURCES})
target_include_directories(render PUBLIC ${HEADER_DIR}/render)
