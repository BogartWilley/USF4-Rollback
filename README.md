# sf4e

A process-inspection and modification tool for the Steam release of _Ultra Street Fighter 4_.

## Table of contents
  * [Building](#building)
  * [License](#license)
  * [External Licenses and Copyright Information](#external-licenses-and-copyright-information)

## Building

`sf4e` is built primarily with [Visual Studio](https://visualstudio.microsoft.com/)
2019 or later with Visual C++. Other development environments will need support for
installing dependencies, ideally via [vcpkg](https://vcpkg.io/en/index.html), and
build file generation via [CMake](https://cmake.org/).

To build sf4e with VS2019:

1. Open `CMakeLists.txt` with VS2019's native CMake integration.
2. Follow the steps in [`vcpkg`'s Getting Started guide](https://vcpkg.io/en/getting-started.html)',
   stopping after `vcpkg` has been bootstrapped.
   * You won't need to manually run `vcpkg install`: vcpkg CMake integration will download the dependencies for you.
3. Configure the VS2019 CMake settings to build with a 32-bit toolset (`msvc_x86`).
   - Since SF4 is a 32-bit x86 executable, `sf4e` also needs to be built with
     32-bit Detours and targeting 32-bit output to properly hook SF4's
     instructions.
   - Ensure that you have set the CMake Toolchain File to
     `scripts/buildsystems/vcpkg.cmake` in the `vcpkg` checkout.
4. Run `Build All`. Confirm that `Launcher.exe` and `Sidecar.dll` are in
   the build output.
5. Run `Launcher.exe`.

To build sf4e with CMake and `vcpkg`:

1. Follow the steps in [`vcpkg`'s Getting Started guide](https://vcpkg.io/en/getting-started.html)',
   stopping after `vcpkg` has been bootstrapped.
2. Use `cmake` or `cmake-gui` to generate a new project.
   * Take care to ensure that the generated project will build a 32-bit x86
     binary. `cmake-gui` on Windows refers to it as "Win32".
   * Also, when generating the project, take care to ensure that the
     `CMAKE_TOOLCHAIN_VARIABLE` is in fact defined and pointing at your
     `vcpkg` checkout as listed under the "Using vcpkg with CMake" heading
     of `vcpkg`'s documentation.
3. Apply your build system to the generated project output. Confirm that
   `Launcher.exe` and `Sidecar.dll` are in the build output.
4. Run `Launcher.exe`.

Builds generated with CMake that cannot take advantage of `vcpkg` will need to
provide the following dependencies:

* [Detours](https://github.com/microsoft/Detours). Detours is used to install
  custom netplay hooks at runtime.
* [ValveFileVDF](https://github.com/TinyTinni/ValveFileVDF). ValveFileVDF
  is used to parse Steam's configuration files, to automatically detect
  your installation of SF4.
* [Dear Imgui](https://github.com/ocornut/imgui). Dear Imgui is used to
  provide custom overlays for new features and non-durable,
  development-time debugging.
* [spdlog](https://github.com/gabime/spdlog). `spdlog` is used to provide
  durable file logging, both at development time and in release builds.
* [nlohmann/json](https://github.com/nlohmann/json). `json` is used for
  message serialization.
* [GameNetworkingSockets](https://github.com/ValveSoftware/GameNetworkingSockets/).
  `GameNetworkingSockets` provides a helpful high-level API on top
  of message passing, and additionally supports NAT hole punching if
  a signalling server is run.
* [GGPO](https://github.com/pond3r/ggpo), used to provide rollback.

## License

This project is licensed under the MIT License - see the LICENSE.md file for details.

## External Licenses and Copyright Information

Street Fighter, Street Fighter 4, Ultra Street Fighter 4, and all related software
Copyright © CAPCOM.

Steam
Copyright © Valve Corporation.

Visual Studio, Visual Studio 2019, vcpkg, and Detours
Copyright © Microsoft Corporation.

CMake - Cross Platform Makefile Generator
Copyright © Kitware, Inc. and Contributors.

ValveFileVDF
Copyright © Matthias Möller.

Dear Imgui
Copyright © Omar Cornut

spdlog
Copyright © 2016 Gabi Melman.

nlohmann/json
Copyright © 2013-2022 Niels Lohmann

GameNetworkingSockets
Copyright © 2018, Valve Corporation

GGPO (Good Game Peace Out)
Copyright © GroundStorm Studios, LLC.