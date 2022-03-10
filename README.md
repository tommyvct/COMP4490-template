# COMP4490 CMake Template

## Structure
- `glsl/`: GLSL shader codes
- `src/`: C++ source codes

The rests are dependencies.


## Modify `CMakeLists.txt` for Assignments
Each `add_executable` directive produces one binary, or a "Project" in Visual Studio. The first parameter is the executable name, and the subsequent parameters are source codes to be compiled and linked together.

For assignment use, modify the executable and your C++ code file name like this:

```cmake
add_executable(Assignment1  # target name
    src/main.cpp         
    src/assignment1.cpp)    # your C++ code filename
```

You can have multiple targets by duplicating target then changing target name and file names.

To enable debugging on these targets on Visual Studio or Xcode, duplicate the working properties and change the target name accordingly:
```cmake
if (WIN32)
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT example0)
    set_property(TARGET Assignment1
        PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/glsl")
elseif (APPLE)
    set_property(TARGET Assignment1 
        PROPERTY XCODE_SCHEME_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/glsl")
endif()
```

If using Ninja or Makefile as generator, regardless of IDE or editor you are using, please make sure the working directory is set to the `glsl/` directory before debugging and running.



## Build instructions
For all platforms:
```bash
mkdir build
cd build
cmake ..
cmake --build .
```

To run the compiled executables:
```bash
# change to glsl directory 
cd ../glsl
# run the executable from here
../build/example0
```

### Windows Specific
- If using Visual Studio, `-A"Win32"` is required since the bundled libraries are 32-bit.
- Make sure the the startup project is assigned to a project other `ALL_BUILD` and `ZERO_CHECK`.

### macOS Specific
CMake cannot set the working directory for Xcode debugging. To manually set the working directory for debugging, go to `Product` -> `Scheme` -> `Edit Scheme`, under `Options` tab, tick `Use custom working directory` and set it to where the GLSL shaders are.

## Linux/Unix Specific
OpenGL related libraries are to be installed via package manager first.

For Ubuntu, the following packages need to be installed:
```
libglu1-mesa-dev freeglut3-dev mesa-common-dev libglew-dev
```
