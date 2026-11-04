# Juki2D

Juki2D is a small 2D game engine I’m building for fun and to learn more about C++ and engine development.

Built with **C++20, SFML 3.1, and CMake**. It provides reusable components for making 2D games without a visual editor.

I’m currently using it to develop **Where We Left Off**, an upcoming slice-of-life romance visual novel.

## Features

- Window management and fixed-timestep updates
- Keyboard and mouse input
- Scene management
- Texture and font caching
- Text rendering, wrapping, and typewriter reveal
- Buttons, sliders, and UI layout helpers
- Music and sound effects with separate volume controls
- Sprite animation playback
- AABB collisions, swept movement, collision layers, and triggers
- Background blur and screen fades

## Building

Requires CMake 3.28 or newer and a C++20 compiler. CMake downloads SFML automatically; its platform dependencies must also be available.

```sh
cmake -S . -B build
cmake --build build --config Release
```

## Project Status

Juki2D is primarily built for my own projects and learning. **I don’t currently recommend it for use in other people’s projects.**

The API is still changing, documentation is limited, and compatibility and stability haven’t been thoroughly tested across platforms. Features are developed around my own games’ needs, so the engine may lack functionality or flexibility that other projects require.

The repository is public to share my progress and source code. It is not yet a stable, supported engine release.

## License

See [LICENSE.md](LICENSE.md). Dependencies retain their respective licenses.
