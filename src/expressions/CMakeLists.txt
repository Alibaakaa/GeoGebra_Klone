set(SOURCES
    Expression.c
)

add_library(expressions STATIC ${SOURCES})
target_include_directories(expressions
    PRIVATE ${EXTERNAL_DIR}/expr
    PUBLIC ${HEADER_DIR}/expressions
)
target_link_libraries(expressions PRIVATE m)
