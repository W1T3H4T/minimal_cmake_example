# Minimal CMake Example
This project is a minimal, self-contained CMake example. It demonstrates
how to structure sources, resources, optional dependencies, unit tests,
and packaging — all from a single `CMakeLists.txt` file.

It was created because the official CMake documentation tends to focus on
peripheral concerns — such as generating configuration headers — rather
than the practical essentials of setting up a real project. This example
takes the opposite approach: compile the source cleanly, with no generated
magic, and keep all build logic visible in one place.

A configuration header is something this project deliberately avoids. In my
opinion, generating one makes the build unnecessarily complicated and
harder for tools that work on source code. Similarly, one `CMakeLists.txt`
at the root should be sufficient for most projects. Scattering build
instructions across multiple subdirectories adds friction without adding
clarity.

## note on boost

Boost filesystem/system is used as an optional example of an external
dependency. If CMake cannot find Boost on your system, the project falls
back to `std::filesystem` automatically. The build will succeed either way.
You can also point CMake to a custom Boost installation using `ccmake` or
`cmake-gui`.

## requirements

- CMake (3.16 or newer)
- A C++ compiler with C++17 support (AppleClang, Clang, GCC, or MSVC)

### optional dependencies

- Boost filesystem/system (optional)
	- If not found, this project falls back to `std::filesystem`.
- GoogleTest (optional, only needed to build and run unit tests)

  **macOS (Homebrew)**
  ```bash
  brew install googletest
  ```

  **Ubuntu / Debian**
  ```bash
  sudo apt install libgtest-dev
  ```

  **Windows (vcpkg)**
  ```powershell
  vcpkg install gtest
  cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=<path\to\vcpkg>\scripts\buildsystems\vcpkg.cmake
  ```
  Alternatively, with [vcpkg integrated](https://learn.microsoft.com/vcpkg/get_started/overview):
  ```powershell
  vcpkg integrate install
  ```

## build

Configure and build from the repository root:

```bash
cmake -S . -B build
cmake --build build -j
```

## run

Run the sample executable:

```bash
./build/example
```

## test

If GoogleTest is installed and found by CMake, the `unit_tests` target
is built and tests are registered with CTest.

Run tests with:

```bash
ctest --test-dir build --output-on-failure
```

If GoogleTest is not installed, the project still configures and builds,
but no tests are discovered.

## package

Create distributable packages with CPack:

```bash
cpack --config build/CPackConfig.cmake
```
