# Voyelles Pendues Engine

This project [voyelles pendues engine](https://github.com/edmBernard/voyelles-pendues-engine) regroup the logic part for a game between hangman game and word search where letter are vowels have been remove from word. The ui part is [voyelles pendues ui](https://github.com/edmBernard/voyelles-pendues-ui)

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

```bash
// On windows
$Env:SPDLOG_LEVEL = "debug"
// On Linux
export SPDLOG_LEVEL=debug
```

## Disclaimer

It's a toy project. Mainly used to learn Qt Quick and Android development.
Your comments are welcome, if you spot error or improvement.
The original idea was taken from [vowels.io](vowels.io) but I would like to have french dictionnary.
