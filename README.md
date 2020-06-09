# Voxelization Using Optimal Scanline 
Code and thesis as part of my masters at Linköping University.

## Dependencies 
To run the code a OpenGL shader version of 4.5 is required. 
If this version is not met, nothing will be shown on screen and error messages will be printed. 

The system also needs CUDA version 10.2 installed.
Depending on where CUDA is installed the Makefile might need to be modified.
The default CUDA location is in `/opt/cuda/`, to modify this change the `INCLUDES`, `LIBDIR` and `LDFLAGS` variables inside `Code/Makefile` to your install location. 
To find your installation directory run:
```
which nvcc
```
and remove the `bin/nvcc` ending, for example `/opt/cuda/bin/nvcc` turns into `/opt/cuda/`.

### Ubuntu
Not entirely sure if these work anymore, was a long time since I used Ubuntu. But it atleast gives some hint as to what is required.
```
  sudo add-apt-repository ppa:glasen/freetype2
  sudo apt update
  sudo apt install freetype2-demos libfreeimage3 libfreeimage-dev xorg-dev libglew-dev libglfw3-dev
```

### Manjaro 
```
  pamac install freeimage freetype2 glew glfw-x11
```

## Running
To compile and run the application go into the `Code` directory and simply enter the following command:
```
make run
```

You can also run it through the command line after compiling it with `make`.
```
./bin/Voxelizer.a
```
Note that the project needs to be run in the `Code` directory, otherwise the resources won't load correctly.
There are multiple command line arguments you can provide to the program to change its behaviour.
To see all enter the command:
```
./bin/Voxelizer.a -h
```

## Thesis 
The master thesis can be found in the `Thesis` folder as a pdf file. 
The LaTeX files are also present there.
