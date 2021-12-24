cmake_minimum_required(VERSION 3.20)
project(socket)

set(CMAKE_CXX_STANDARD 17)

set(SOURCES
    ${PROJECT_SOURCE_DIR}/TCP/Sources/socketTCP.cpp
    ${PROJECT_SOURCE_DIR}/UDP/Sources/socketUDP.cpp
)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/Libs/")

include_directories(${PROJECT_SOURCE_DIR}/TCP/Headers)
include_directories(${PROJECT_SOURCE_DIR}/UDP/Headers)

add_library(
    ${PROJECT_NAME}
    STATIC
    ${SOURCES}
)
