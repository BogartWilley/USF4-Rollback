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
   * You won't need to manually run `vcpkg install` because proper CMake
     integration will download the dependencies for you.
3. Configure the VS2019 CMake settings to build with a 32-bit toolset
   (`msvc_x86`).
   - Since SF4 is a 32-bit x86 executable, `sf4e` also needs to be built with
     32-bit Detours and targeting 32-bit output to properly hook SF4's
     instructions.
   - Ensure that you have set the CMake Toolchain File to
     `scripts/buildsystems/vcpkg.cmake` in the `vcpkg` checkout.
4. Run `Build All`. Confirm that `Launcher.exe` and `Sidecar.dll` are in
   the build output.
5. Run `Launcher.exe`.

To build sf4e with CMake and `vcpkg`:

1. Install the project dependencies with `vcpkg install --overlay-ports=./vcpkg-ports --overlay-triplets=./vcpkg-triplets --triplet=x86-windows-wchar-filenames`.
  * This must be run from the root of the `sf4e` repo, so that `vcpkg` installs
    from the manifest file.
  * The `--overlay-ports` argument extends `vcpkg`'s internal registry with
    dependencies that `sf4e` uses that are not in `vcpkg`'s main port list.
  * The custom triplet argument must be used to ensure Unicode support, which
    is not optional with newer methods in the Win32 APIs.
2. Use `cmake` or `cmake-gui` to generate a new project.
  * Take care to ensure that the generated project will build a 32-bit x86
    binary. `cmake-gui` on Windows refers to it as "Win32".
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
* [spdlog](https://github.com/gabime/spdlog). spdlog is used to provide
  durable file logging, both at development time and in release builds.
* [asio](https://think-async.com/Asio/). asio is used to provide
  cross-platform asynchronous networking with a
  [built-in duration-bound event processing loop method](https://think-async.com/Asio/asio-1.24.0/doc/asio/reference/io_context/run_for.html).

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

Asio
Copyright © 2003-2022 Christopher M. Kohlhoff