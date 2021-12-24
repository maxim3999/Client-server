cmake_minimum_required(VERSION 3.20)
project(server)

set(CMAKE_CXX_STANDARD 17)

set(SOURCES
    ${PROJECT_SOURCE_DIR}/TCP/Sources/serverTCP.cpp
    ${PROJECT_SOURCE_DIR}/UDP/Sources/serverUDP.cpp
)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/Libs/")

include_directories(${PROJECT_SOURCE_DIR}/TCP/Headers)
include_directories(${PROJECT_SOURCE_DIR}/UDP/Headers)

include_directories(${PROJECT_SOURCE_DIR}/../Socket/TCP/Headers)
include_directories(${PROJECT_SOURCE_DIR}/../Socket/UDP/Headers)

add_library(
    ${PROJECT_NAME}
    STATIC
    ${SOURCES}
)