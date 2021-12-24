cmake_minimum_required(VERSION 3.20)
project(Server)

set(CMAKE_CXX_STANDARD 17)

include_directories(${PROJECT_SOURCE_DIR}/../../Socket/TCP/Headers)
include_directories(${PROJECT_SOURCE_DIR}/../../Socket/UDP/Headers)

add_executable(
    ${PROJECT_NAME}
    main.cpp
)

target_link_libraries(
    ${PROJECT_NAME}
    ${PROJECT_SOURCE_DIR}/../../Socket/Libs/libsocket.a
)