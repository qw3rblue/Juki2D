cmake_minimum_required(VERSION 3.28)
project(Juki2D LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.1.0
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

add_library(Juki2D STATIC
    "src/Collision.cpp"
    "src/Application.cpp"
)

target_include_directories(Juki2D PUBLIC
    include
)

target_compile_features(Juki2D PUBLIC cxx_std_20)
target_link_libraries(Juki2D PUBLIC SFML::Graphics)

add_executable(Sandbox
    src/main.cpp
    src/Game.cpp
    src/Player.cpp
)

target_link_libraries(Sandbox PRIVATE Juki2D)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Sandbox)
