[![wakatime](https://wakatime.com/badge/github/UFifty50/AnEngine.svg)](https://wakatime.com/badge/github/UFifty50/AnEngine)
# AnEngine
Learning gameEngine-dev, roughly following [TheCherno's Game Engine series](https://youtube.com/playlist?list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT) with my own modifications and extensions.

- [AnEngine](#AnEngine)
  - [Setup](#setup)
    - [Windows](#windows)
    - [Linux](#linux)

- **AnEngine/**: The core engine.
- **Crank/**: The engine UI.

## Requirements
- gcc and g++ with versions >= 10
- Premake v5 (see [Setup](#Setup))
- Vulkan (The correct version will be installed by the Setup script)

## Setup
First clone the repository with `git clone --recursive https://github.com/UFifty50/AnEngine`. (If you want the dev branch don't forget the `-b dev`!)
> Note: If the repository has already been cloned non-recursively, use `git submodule update --init` to clone the necessary submodules.
AnEngine uses the `Premake 5` build system. Follow [these](https://premake.github.io/download.html) instructions to install it.

###### _hint, there are x64 linux and windows binaries in the vendor/premake folder._

## Windows
AnEngine works best using Visual Studio 2022. To generate the `.sln` and `.vcxproj` files, run the `Setup.py` script in the "scripts" directory.


## Linux
# Linux is not currently working! The below instructions are for if you want to try anyways.

AnEngine has extra dependencies on Linux. These development packages are needed:
- `libxcursor`
- `libxrandr`
- `libxinerama`
- `libxi`
- `libglew`

On apt-based distros, these can be installed by running:
`sudo apt install libxcursor-dev libxrandr-dev libxinerama-dev libxi-dev libglew-dev`

AnEngine can then be configured and compiled with:
```bash
python scripts/Setup.py
make
```
