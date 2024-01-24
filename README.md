# SplayTree

[![Build](https://github.com/xorz57/SplayTree/actions/workflows/Build.yml/badge.svg)](https://github.com/xorz57/SplayTree/actions/workflows/Build.yml)
[![Quality Gate Status](https://sonarcloud.io/api/project_badges/measure?project=xorz57_SplayTree&metric=alert_status)](https://sonarcloud.io/summary/new_code?id=xorz57_SplayTree)

## How to Build

#### Linux & macOS

```bash
git clone https://github.com/microsoft/vcpkg.git ~/vcpkg
~/vcpkg/bootstrap-vcpkg.sh

git clone https://github.com/xorz57/SplayTree.git
cd SplayTree
cmake -B build -DCMAKE_BUILD_TYPE=Release -S . -DCMAKE_TOOLCHAIN_FILE=~/vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build build --config Release
ctest --build-config Release
```

#### Windows

```powershell
git clone https://github.com/microsoft/vcpkg.git C:/vcpkg
C:/vcpkg/bootstrap-vcpkg.bat
C:/vcpkg/vcpkg.exe integrate install

git clone https://github.com/xorz57/SplayTree.git
cd SplayTree
cmake -B build -DCMAKE_BUILD_TYPE=Release -S . -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build build --config Release
ctest --build-config Release
```
