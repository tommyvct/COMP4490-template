# COMP4490 CMake Template

## Structure
- `contrib/`: External dependencies like `glew`, `freeglut` and `glm`
- `src/`: C++ and GLSL source codes
    - `CMakeLists.txt`: Lists of all examples
    - `common.h`: common header used by the examples
    - `common_tessellation.h`: common header used by the tesselation executables
    - `main.cpp`: Main function driver code
    - `main_tessellation.cpp`: Main function driver code for the tesselation examples
    - `example0/`: `.cpp` and GLSL shader files
        - `CMakeLists.txt`: contains example-specific configurations, including which file to compile and settting working directory for Visual Studio or XCode
        - `*.cpp` and shaders
- `CMakeLists.txt`: CMake rules to find dependencies
- `cmake/`: CMake helper macros

## Modify for Assignment Use

0. Delete unused targets from `src/`, and `src/CMakeLists.txt`
1. Rename the target folder, and change folder name accordingly in `src/CMakeLists.txt`  
   For example, if `src/example0` was renamed to `src/Assignment1`, then in `src/CMakeLists.txt`, line
   ```cmake
   add_subdirectory(example0)
   ```
   should be
   ```cmake
   add_subdirectory(Assignment1)
   ```
2. Inside the target folder, change filenames in `CMakeLists.txt`  
   For example, given `Assignment1.cpp` and `Assignment1.hpp`, `src/Assignment1/CMakeLists.txt` should be modified from
   ```cmake
   target_sources(${TARGET_NAME} PUBLIC
    FILE_SET HEADERS
    BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/.. 
    FILES ../common.h) # more headers go here
    target_sources(${TARGET_NAME} PUBLIC 
    example0.cpp) # C++ codes go here
   ```
   to 
   ```cmake
   target_sources(${TARGET_NAME} PUBLIC
    FILE_SET HEADERS
    BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/.. 
    FILES ../common.h Assignment1.hpp) # more headers go here
    target_sources(${TARGET_NAME} PUBLIC 
    Assignment1.cpp) # C++ codes go here
   ```
3. Inside the top-level `CMakeLists.txt`, modify the line
   ```cmake
   set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT example0)
   ```
   to
   ```cmake
   set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Assignment1)
   ```
   in order to set target `Assignment1` as the startup project in Visual Studio.

## Build instructions
Make sure you have a recent and working C++ compiler, and CMake version 3.23+.
### External Dependencies
Most of the dependencies are build out-of-tree and installed at `contrib/install`. CMake is told to find dependencies from here.

#### Building `glew`
Official GLEW repo: https://github.com/nigels-com/glew
- Very bad CMake support
- Source is generated using Unix-only tools and very complicated
- It's better to take the binaries instead of compiling.  
  - For Windows:
    1. Download the latest Win32 binary
    2. Copy `lib/Release/x64/glew32s.lib` from the ZIP file to `contrib/install/lib`
    3. For dynamic linkage, copy `lib/Release/x64/glew32.lib` from the ZIP file to `contrib/install/lib`, and `bin/Release/x64/glew32.dll` from the ZIP file to `contrib/install/bin`
  - For macOS and Linux:  
    Install binary packages from their respective package manager, see Linux/Unix specific

#### GLM, FreeGLUT
Regardless of platform, run this:
```bash
cd contrib
# GLM
cmake -B buildglm -S glm -DCMAKE_INSTALL_PREFIX="./install"
cmake --buildglm build --config Release
# FreeGLUT
cmake -B buildfreeglut -S glm -DCMAKE_INSTALL_PREFIX="./install"
cmake --build buildfreeglut --config Release
```
macOS and Linux can install binary packages of FreeGLUT from their respective package manager instead of compiling it, see Linux/Unix specific

### Build This Project
For all platforms:
```bash
cmake -B build -S .  # configure
cmake --build build  # build
```

There are a few configure options available:  
| Option | Description | Default Value |
|--------|-------------|---------------|
|`OPENMP`| Use OpenMP acceleration, can siginificantly speed up rendering, especially for raytracer | `ON` except MSVC |
|`STATIC_GLEW`| Link to static verion of GLEW | `ON` except Linux |
|`STATIC_FREEGLUT`| Link to static verion of FreeGLUT | `ON` except Linux |

Configure using these options like this:
```bash
cmake -B build -S . -DOPENMP=OFF
```

To run the compiled executables:
```bash
# change to glsl directory 
cd src/example0
# run the executable from where the GLSL shaders are
../../build/example0
```

### Windows Specific
1. You can open `build/COMP4490.sln` using Visual Studio. Each executable in `src/` should be a project inside the solution
2. Make sure the the startup project is assigned to a project other than `ALL_BUILD` and `ZERO_CHECK`.

### macOS Specific
You can open `build/COMP4490.xcodeproj` using XCode.

### Linux/Unix Specific
OpenGL related libraries are to be installed via their respective package manager first.

For Ubuntu, the following packages need to be installed:
```
libglu1-mesa-dev freeglut3-dev mesa-common-dev libglew-dev
```

If using Ninja or Makefile as generator, regardless of IDE or editor you are using, please make sure the working directory to where your GLSL shaders are (in this case, where your `.cpp` files are) before debugging and running.
