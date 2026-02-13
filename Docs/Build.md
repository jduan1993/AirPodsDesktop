# Build Instructions

## Preparations
```bash
git clone --recursive https://github.com/jduan1993/AirPodsDesktop.git
cd AirPodsDesktop
```
The current path will be named ___BuildPath___ in the rest of this document.

1. Download and install [CMake](https://cmake.org/download/) (>= v3.20) if you didn't have it.

2. Choose a target platform to continue:
    - [Windows](#windows)

## Windows

1. Download and install __Visual Studio 2026__ if you didn't have it.

2. Clone and bootstrap [vcpkg](https://github.com/microsoft/vcpkg#quick-start-windows) if you didn't have it.

3. Download and install [Qt 5.15.2](https://www.qt.io/download-qt-installer). (Minimum required components are only `MSVC 2019 32-bit`)

4. Download and install [NSIS](https://sourceforge.net/projects/nsis/files/latest/download) if you want to generate the installer.

5. Open __Powershell__, go to ___BuildPath___.  
It is recommended to clean your environment (e.g., temporarily remove Anaconda from PATH) to avoid interference.

6. Replace the placeholders and run the following commands:

    ```powershell
    # Generate project
    cmake -S . -B Build -G "Visual Studio 18 2026" -A Win32 `
        -DCMAKE_PREFIX_PATH="<QtPath>/5.15.2/msvc2019" `
        -DCMAKE_TOOLCHAIN_FILE="<VcpkgPath>/scripts/buildsystems/vcpkg.cmake" `
        -DQt5_DIR="<QtPath>/5.15.2/msvc2019/lib/cmake/Qt5" `
        -DCMAKE_CXX_STANDARD=20 `
        -DAPD_GENERATE_INSTALLER=ON

    # Build Release version
    cmake --build Build --config Release
    ```

    - `<QtPath>`: Your Qt installation root (e.g., `C:/Qt`).
    - `<VcpkgPath>`: Your vcpkg repository path.
    - The binaries will be located in `Build/Binary` after a successful build.
    - For debugging, change `--config Release` to `--config Debug`.
    - See the [CMakeLists.txt](/CMakeLists.txt) `Build options` section for more options.
