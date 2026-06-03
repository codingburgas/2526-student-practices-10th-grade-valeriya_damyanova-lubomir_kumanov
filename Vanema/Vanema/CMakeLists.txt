cmake_minimum_required(VERSION 3.20)

project(Vanema)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(raylib CONFIG REQUIRED)
find_package(SQLiteCpp CONFIG REQUIRED)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin/Debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin/Release)

add_executable(Vanema
    src/PL/Login.cpp
    src/PL/Menu.cpp
    src/PL/main.cpp
    src/PL/Booking.cpp
    src/PL/SignUp.cpp

    src/BLL/Movie.cpp
    src/BLL/MovieService.cpp
)

target_link_libraries(Vanema PRIVATE
    raylib
    SQLiteCpp
)

target_include_directories(Vanema PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/PL
    ${CMAKE_SOURCE_DIR}/src/BLL
)
set_target_properties(Vanema PROPERTIES
    VS_SETTINGS "LocalDebuggerWorkingDirectory=${CMAKE_CURRENT_SOURCE_DIR}"
)

add_custom_command(TARGET Vanema POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:Vanema>/assets
)