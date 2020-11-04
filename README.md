# CuckyVoxel

Shameless Minecraft Clone

## Dependencies

* SDL2 (if `COMPILE_SDL2` is set to ON)
* GLEW (if `COMPILE_SDL2` is set to ON)
* pkg-config (for builds that require static-linkage)

## Building

This project uses CMake, allowing it to be built with a range of compilers.

Switch to the terminal and `cd` into this folder.
After that, generate the files for your build system with:

```
cmake -B build -DCMAKE_BUILD_TYPE=Release
```

MSYS2 users should append `-G"MSYS Makefiles" -DPKG_CONFIG_STATIC_LIBS=ON` to this command, also.

You can also add the following flags:

Name | Function
--------|--------
`-DCOMPILE_CLIENT=ON` | When set to ON, a CuckyVoxel client is compiled. ~~Otherwise, a CuckyVoxel server is compiled~~ (unimplemented)
`-DCOMPILE_SDL2=ON` | When set to ON, the SDL2 backend will be compiled for CuckyVoxel clients.
`-DLTO=ON` | Enable link-time optimisation
`-DPKG_CONFIG_STATIC_LIBS=ON` | On platforms with pkg-config, static-link the dependencies (good for Windows builds, so you don't need to bundle DLL files)
`-DMSVC_LINK_STATIC_RUNTIME=ON` | Link the static MSVC runtime library, to reduce the number of required DLL files (Visual Studio only)

You can pass your own compiler flags with `-DCMAKE_C_FLAGS` and `-DCMAKE_CXX_FLAGS`.

You can then compile CuckyVoxel with this command:

```
cmake --build build --config Release
```
