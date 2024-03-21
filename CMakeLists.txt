cmake_minimum_required(VERSION 3.20)

project(GeoGebra_Klone)

set(HEADER_DIR "${CMAKE_SOURCE_DIR}/include")
set(EXTERNAL_DIR "${CMAKE_SOURCE_DIR}/3rdparty")

add_subdirectory(src/expressions)
add_subdirectory(src/gui)
