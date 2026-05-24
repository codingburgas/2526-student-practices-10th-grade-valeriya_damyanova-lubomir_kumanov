set(CMAKE_TOOLCHAIN_FILE
    "C:/Users/TP14s/vcpkg/scripts/buildsystems/vcpkg.cmake"
    CACHE STRING ""
)

cmake_minimum_required(VERSION 3.20)

project(Vanema)

set(CMAKE_CXX_STANDARD 17)

find_package(raylib CONFIG REQUIRED)
find_package(SQLiteCpp CONFIG REQUIRED)

add_executable(Vanema
    "src/PL/Login.cpp"
    "src/PL/Menu.cpp"
    "src/PL/main.cpp"
 "src/PL/Booking.cpp"  
 "src/PL/SignUp.cpp" "src/PL/SignUp.h")

target_link_libraries(Vanema
    PRIVATE
    raylib
    SQLiteCpp
)

set_target_properties(Vanema PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
)

file(COPY ${CMAKE_SOURCE_DIR}/assets
     DESTINATION ${CMAKE_BINARY_DIR})

file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})