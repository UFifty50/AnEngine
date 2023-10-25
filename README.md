[![wakatime](https://wakatime.com/badge/github/UFifty50/AnEngine.svg)](https://wakatime.com/badge/github/UFifty50/AnEngine)
## AnEngine
Learning gameEngine-dev using [TheCherno's Game Engine series](https://youtube.com/playlist?list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT)

- [AnEngine](#AnEngine)
  - [Setup](#setup)
    - [Windows](#windows)
    - [Linux](#linux)

- **AnEngine/**: The core engine.
- **Crank/**: The engine UI.
- **Sandbox/**: Used for in-place testing.

## Setup
First clone the repository with `git clone --recursive <URL>`.
> Note: If the repository has already been cloned non-recursively, use `git submodule update --init` to clone the necessary submodules.
AnEngine uses the `Premake 5` build system. Follow [these](https://premake.github.io/download.html) instructions to install it.

###### _hint, there are x64 linux and windows binaries in the vendor/premake folder._

## Windows

AnEngine works best using Visual Studio 2022. To generate the `.sln` and `.vcxproj` files, run `premake vs2022` in the root directory of the repository.

## Linux

AnEngine has extra dependencies on Linux. These development packages are needed:
- `libxcursor`
- `libxrandr`
- `libxinerama`
- `libxi`
- `libglew`

AnEngine also requires versions of gcc and g++ that support C++20 (gcc and g++ >= 10).

On apt-based distros, these can be installed by running:

`sudo apt install libxcursor-dev libxrandr-dev libxinerama-dev libxi-dev libglew-dev`

AnEngine can then be configured and compiled with:
```bash
premake5 gmake2
make
```
