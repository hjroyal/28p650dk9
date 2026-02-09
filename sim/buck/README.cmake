# Building buck example with CMake

This directory contains a CMakeLists.txt for building the OPiL buck example.

## Prerequisites
- CMake 3.18 or higher
- C compiler (GCC on Linux, MSVC or MinGW on Windows)

## Building

### Windows (Visual Studio)
```bash
mkdir build
cd build
cmake ..
cmake --build . --config Release
```

### Linux
```bash
mkdir build
cd build
cmake ..
make
```

## Targets

The CMake build generates the following targets:

1. **opiltarget** - Controller executable (opiltarget.exe on Windows, opiltarget on Linux)
2. **opil_plecs** - PLECS DLL (libopil_plecs.dll on Windows, libopil_plecs.so on Linux) - optional

To build the PLECS DLL, enable the `BUILD_PLECS_DLL` option:
```bash
cmake -DBUILD_PLECS_DLL=ON ..
```

## Output files

- `build/Release/opiltarget.exe` (Windows) or `build/opiltarget` (Linux)
- `build/Release/libopil_plecs.dll` (Windows) or `build/libopil_plecs.so` (Linux) if built

## Configuration

The default port for communication is 8090. To change it, modify the `TARGET_COMM_SOCK_SERVER_PORT` definition in CMakeLists.txt.

## Running

1. Run the controller: `./build/Release/opiltarget.exe`
2. Open the PLECS model `plecs/opil_buck.plecs` and run simulation.

Note: The PLECS DLL must be built and placed in the appropriate directory for PLECS to load it. The default Makefile places it in `plecs/build/`. You may need to copy the built DLL there or adjust the PLECS model.