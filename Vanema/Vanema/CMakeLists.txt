cmake_minimum_required(VERSION 3.20)
project(Vanema)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_TOOLCHAIN_FILE AND EXISTS "C:/Users/TP14s/vcpkg/scripts/buildsystems/vcpkg.cmake")    
    set(CMAKE_TOOLCHAIN_FILE "C:/Users/TP14s/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "" FORCE)
endif()

find_package(raylib CONFIG REQUIRED)
find_package(SQLiteCpp CONFIG REQUIRED)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin/Debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin/Release)

add_executable(Vanema
    # Excluded Vanema.cpp here to resolve the duplicate main entry points linker issue
    Vanema.h

    src/PL/main.cpp
    # Excluded src/PL/main_old.cpp here to prevent any older main loops from conflicting
    src/PL/Booking.cpp
    src/PL/Booking.h
    src/PL/Films.cpp
    src/PL/Films.h
    src/PL/Login.cpp
    src/PL/Login.h
    src/PL/Menu.cpp
    src/PL/Menu.h
    src/PL/MovieDetails.cpp
    src/PL/MovieDetails.h
    src/PL/Offers.cpp
    src/PL/Offers.h
    src/PL/SignUp.cpp
    src/PL/SignUp.h
    src/PL/Spots.cpp
    src/PL/Spots.h

    src/BLL/Movie.cpp
    src/BLL/Movie.h
    src/BLL/MovieService.cpp
    src/BLL/MovieService.h
    src/BLL/User.cpp
    src/BLL/User.h
    src/BLL/UserService.cpp
    src/BLL/UserService.h

    src/DAL/bcrypt.cpp
    src/DAL/bcrypt.h
    src/DAL/DAL.cpp
    src/DAL/DAL.h
    src/DAL/DataSeeder.cpp
    src/DAL/DataSeeder.h
    src/DAL/framework.h
    src/DAL/MovieRepository.cpp
    src/DAL/MovieRepository.h
    src/DAL/UserRepository.cpp
    src/DAL/UserRepository.h
    src/DAL/UserSeeder.cpp
    src/DAL/UserSeeder.h
 "src/PL/AddMovie.cpp" "src/PL/AddMovie.h" "src/PL/FilePicker.cpp")

target_link_libraries(Vanema PRIVATE
    raylib
    SQLiteCpp
)

# This block allows all nested versions of your includes to work seamlessly
target_include_directories(Vanema PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/PL
    ${CMAKE_CURRENT_SOURCE_DIR}/src/BLL
    ${CMAKE_CURRENT_SOURCE_DIR}/src/DAL
)

if(MSVC)    
    target_compile_options(Vanema PRIVATE "/FI${CMAKE_CURRENT_SOURCE_DIR}/Vanema.h")
else()    
    target_compile_options(Vanema PRIVATE "-include" "${CMAKE_CURRENT_SOURCE_DIR}/Vanema.h")
endif()

set_target_properties(Vanema PROPERTIES
    VS_SETTINGS "LocalDebuggerWorkingDirectory=${CMAKE_CURRENT_SOURCE_DIR}"
)

add_custom_command(TARGET Vanema POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:Vanema>/assets
)