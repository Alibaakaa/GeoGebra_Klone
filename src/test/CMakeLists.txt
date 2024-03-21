add_executable(test test.c)

target_include_directories(test PRIVATE ${HEADER_DIR}/expressions)
target_link_libraries(test PRIVATE expressions)
