# Voyelle Pendues Engine

## Dependencies

We use [vcpkg](https://github.com/Microsoft/vcpkg) to manage dependencies

KoTH-snake depend on:
- [fmt](https://fmt.dev/latest/index.html): A modern formatting library

```
./vcpkg install fmt
```

## Build instruction

```bash
mdkir build
cd build
cmake -DCMAKE_TOOLCHAIN_FILE=${VCPKG_DIR}/scripts/buildsystems/vcpkg.cmake ..
cmake --build . --config Release
```
