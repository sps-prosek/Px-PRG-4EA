# Repository for Study Materials for Programming Course Px (PRG) 4EA

This repository contains projects written for the RPi Pico using the Pico C/C++ SDK.

## Project Structure

- `.devcontainer/`: Folder with dev container source files (docker files)
- `pico-examples/`: Folder with pico-sdk examples
- `presentations/`: Folder with presentations used in lectures
- `student-packages/`: Folder with base materials for assigments
- `teacher-packages/`: Folder with finished assigments

## Getting Started

### Prerequisites

- Docker
- Git
- Visual Studio Code
  - [Dev Containers](https://marketplace.visualstudio.com/items?itemName=ms-vscode-remote.remote-containers) extension (optional for building through docker)
  - [CMake Tools](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cmake-tools) extension
  - [C/C++](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools) extension

### Clone this repo and open it in VS Code (optional dev environment)

1. Clone this repository:

   ```sh
   git clone https://github.com/sps-prosek/Px-PRG-4EA.git
   ```

2. Open the repository folder in VS code.

3. (only for docker users) Reopen the repository folder inside the dev container. VS code will ask you or you can open command pallet (ctrl + shift + P) and find command `Dev Containers: Reopen in container`.

### Building the Project with CMake extension

1. After opening this folder in VS Code (Pico VS Code for school computers) choose the CMakeLists.txt file located in the root of this repository when asked so.

2. Build the project with build button on the bottom left corner of the VS Code

### Building the Project manually

1. Open terminal in this folder.

2. Create a build directory and navigate into it:

   ```sh
   mkdir build
   cd build
   ```

3. Configure the project with CMake:

   ```sh
   cmake ..
   ```

4. Build the project:
   ```sh
   cmake --build .
   ```

### Flashing the Firmware to the RP2040 Board

1. Connect the RP2040 board to your computer.
2. Press and hold the BOOTSEL button while plugging the board into USB.
3. Copy the generated `.uf2` file from the `build` directory to the RP2040 USB mass storage device.
