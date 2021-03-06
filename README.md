# Global Game Jam 2017

## Development Setup

### 0. Basics

Use a Linux-based machine. Preferrably Arch Linux.
The game requires pulseaudio.

### 1. Tools

You need to have some basic build-tools installed (C Compiler, Make, C-Make, etc...)

On Arch Linux you could run: ```sudo pacman -S base-devel cmake clang git```.


### 2. 3rd party libraries

Some libraries need to be installed using your favorite systems package manager:

- SDL
- SDL_image
- SDL_ttf
- SDL_mixer
- libc++

On Arch Linux you could run: ```sudo pacman -S sdl2 sdl2_image sdl2_mixer sdl2_ttf libc++```.

### 3. Clone this repository

You need to clone this repository somewhere onto your laptop: ```git clone --recursive git@github.com:Prior99/ggj2017.git```.
Please note the ```--recursive``` which will also initialize the neccessary git submodules. If you did not use ```---recursive```, please initialize them manually.

### 4. Build the sources

There is a Makefile included for your convenience.

 - Build the sources: ```make```
 - Run the game: ```make run```
 - Clean up: ```make clean```

# Attribution

Sven-Hendrik Haase of which the [boilerplate](https://github.com/svenstaro/innojam9) was used.
