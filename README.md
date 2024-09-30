# Repository for study materials for lecture Px (PRG) 4EA

Repository with projects written for RPi pico in Pico C/C++ sdk.

## Project Structure

- `.devcointainer/`: Folder with dev container source files (docker files)

## Getting Started

### Prerequisites

- Docker
- Visual Studio Code
  - [Dev Containers](https://marketplace.visualstudio.com/items?itemName=ms-vscode-remote.remote-containers) extension

### Clone this repo and open it in dev container

1. Clone this repository:

   ```sh
   git clone https://github.com/sps-prosek/Px-PRG-4EA.git
   ```

2. Open the repository folder in VS code.

3. Reopen the repository folder inside of dev container. VS code will ask you or you can open command pallet (ctrl + shift + P) and find command `Dev Containers: Reopen in container`.

### Building the Project

1. Open terminal.

2. Navigate to project folder that you want to build:

   ```sh
   cd <FOLDER NAME>
   ```

3. Create a build directory and navigate into it:

   ```sh
   mkdir build
   cd build
   ```

4. Configure the project with CMake:

   ```sh
   cmake ..
   ```

5. Build the project:
   ```sh
   make
   ```

### Flashing the Firmware

1. Connect the RP2040 board to your computer.
2. Press and hold the BOOTSEL button while plugging the board into USB.
3. Copy the generated `.uf2` file from the `build` directory to the RP2040 USB mass storage device.
