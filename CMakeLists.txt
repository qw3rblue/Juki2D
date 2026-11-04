cmake_minimum_required(VERSION 3.28)
project(Juki2D VERSION 0.1.0 LANGUAGES CXX)
option(JUKI2D_USE_SYSTEM_SFML "Use an installed SFML 3.1 package" OFF)
if(NOT TARGET SFML::Graphics OR NOT TARGET SFML::Audio)
    if(JUKI2D_USE_SYSTEM_SFML)
        find_package(SFML 3.1 REQUIRED COMPONENTS Graphics Audio)
    else()
        include(FetchContent)
        FetchContent_Declare(SFML
            GIT_REPOSITORY https://github.com/SFML/SFML.git
            GIT_TAG 3.1.0
            GIT_SHALLOW ON
            EXCLUDE_FROM_ALL
            SYSTEM
        )
        FetchContent_MakeAvailable(SFML)
    endif()
endif()
add_library(Juki2D STATIC
    src/Application.cpp
    src/Audio.cpp
    src/BlurTexture.cpp
    src/Body2D.cpp
    src/Collision.cpp
    src/FontStore.cpp
    src/Input.cpp
    src/SceneManager.cpp
    src/SplashScene.cpp
    src/SpriteAnimator.cpp
    src/Text.cpp
    src/TextureStore.cpp
    src/UI.cpp
)
add_library(Juki2D::Juki2D ALIAS Juki2D)
target_include_directories(Juki2D PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>")
target_compile_features(Juki2D PUBLIC cxx_std_20)
target_link_libraries(Juki2D PUBLIC SFML::Graphics SFML::Audio)
set_target_properties(Juki2D PROPERTIES CXX_EXTENSIONS OFF)
