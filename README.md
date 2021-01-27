# Voyelle Pendues Engine

## Dependencies

We use [vcpkg](https://github.com/Microsoft/vcpkg) to manage dependencies

The project depends on:
- [fmt](https://fmt.dev/latest/index.html): A modern formatting library.
- [spdlog](https://github.com/gabime/spdlog): Fast C++ logging library.
- [utfcpp](https://github.com/nemtrif/utfcpp): UTF-8 with C++ in a Portable Way.

```
./vcpkg install fmt spdlog utfcpp
```

## Build instruction

```bash
mdkir build
cd build
cmake -DCMAKE_TOOLCHAIN_FILE=${VCPKG_DIR}/scripts/buildsystems/vcpkg.cmake ..
cmake --build . --config Release
```

## Logger

```
// On windows
$Env:SPDLOG_LEVEL = "debug"
// On Linux
export SPDLOG_LEVEL=debug
```