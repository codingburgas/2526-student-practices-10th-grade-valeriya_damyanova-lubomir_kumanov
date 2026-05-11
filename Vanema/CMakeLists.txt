cmake_minimum_required(VERSION 3.20)

project(Vanema)

set(CMAKE_CXX_STANDARD 17)

find_package(raylib CONFIG REQUIRED)
find_package(SQLiteCpp CONFIG REQUIRED)

add_executable(Vanema
    src/main.cpp
)

target_link_libraries(Vanema
    PRIVATE
    raylib
    SQLiteCpp
)