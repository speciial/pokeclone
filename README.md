# POKECLONE

Pokeclone is a small project where I implement a Pokemon (specifically the FireRed/LeafGreen versions) clone in C. This project will make use of a couple libraries to create windows and set up a rendering context for OpenGL. Other than that though, there won't be many dependencies since this project is meant as a programming exercise. 

## About the Clone

The original resolution of the GBA is 240x160 pixels. For a regular HD screen, this is a little to small so the clone will be scaled up by a factore of 4. This means the window is now 960x640 pixel, a block is 64x64 pixel, and a tile is 32x32 pixel. 

Currently, the goal is to rebuild the game, so other than upscaling the resolution, things should look and feel like in the original game. For recreating the textures and maps [Serebii.net's Pokearth](https://www.serebii.net/pokearth/) seems like a great resource. For character animation and other sprites and images [the spriters resource](https://www.spriters-resource.com/game_boy_advance/pokemonfireredleafgreen) has a lot of stuff.

## About the Code

As of writing this, I don't really have a "style" of programming in C that I'm comfortable with. In part, this is why I'm working on this project. 

## Building and Running the Program

### Clone the Repo

Before you can build the actual program, you have to make sure GLFW is included as a submodule in the `third_party` directory. So, make sure to either clone this repository recursively like this:

```bash
git clone --recursive git@github.com:speciial/pokeclone.git
```

Or (if you already cloned the repo without --recursive) load the submodules with:

```bash
git submodule update --init
```

### Build the Program

The project come pre-configured with a CMakeLists.txt which you can just use to build and run the application. All the created binaries are placed into the `build/bin` directory along with all the resource files. 