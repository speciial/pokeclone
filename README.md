# POKECLONE

Pokeclone is a small project where I implement a Pokemon (specifically the FireRed/LeafGreen versions) clone in C. This project will make use of a couple libraries to create windows and set up a rendering context for OpenGL. Other than that though, there won't be many dependencies since this project is meant as a programming exercise. 

## About the Clone

The original resolution of the GBA is 240x160 pixels. For a regular HD screen, this is a little to small so the clone will be scaled up by a factore of 4. This means the window is now 960x640 pixel, a block is 64x64 pixel, and a tile is 32x32 pixel.

## About the Code

As of writing this, I don't really have a "style" of programming in C that I'm comfortable with. In part, this is why I'm working on this project. 

## Building and Running the Program

### Clone the Repo

Before you can build the actual program, you have to build GLFW which is included as a submodule in the `third_party` directory. So, make sure to either clone this repository recursively like this:

```bash
git clone --recursive git@github.com:speciial/pokeclone.git
```

Or (if the you already clone the repo non-recursively) load the submodules with:

```bash
git submodule update --init
```

### Build the Libraries

To build GLFW open the `third_party` directory in a terminal (with MSVC developer tools enabled so we can use cmake) and navigate to the `GLFW` folder. Here, generate a Visual Studio Solution using CMake with the following command:

```bash
cmake -S . -B build
```

In the `build` folder generated inside the `GLFW` directory, there you should find a `GLFW.sln` which you open with Visual Studio. In there set the configuration to `Release` and the Platform to `x64` and build the solution. With this, you should be able to build the application as described in the next section.

### Build the Program

To keep things simple, this project will only target Windows and use the MSVC compiler. To build the application, simply run the build.bat script in a terminal with the MSVC developer tools enabled (x64 Native Command Tools). This will generate a build folder where the executable (application.exe) can be found.