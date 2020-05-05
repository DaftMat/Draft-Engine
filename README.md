# Daft-Engine

Render Engine made in C++/OpenGL/Qt.

## Dependencies

 - [GLM](https://github.com/g-truc/glm) (As submodule in repository)
 - [Eigen](https://gitlab.com/libeigen/eigen) (As submodule in repository)
 - GLSL/OpenGL 4.10+
 - Qt5

## Downloading
Run the following command :
```txt
$ git clone --recurse-submodules https://github.com/DaftMat/Daft-Engine.git
```

## Compiling
Run the following commands from the root directory of the downloaded repository :
```txt
$ mkdir build
$ cd build
$ cmake ..
$ make
```

## Executing
Run the following commands from the recently created `build` directory
```
$ cd ../bin
$ ./Daft-Engine
```

## Documentation
Go to the directory `doc`. From here, you just have to type this command :
```
$ ./generate.sh
```
Make sure you have installed `graphviz` (for uml diagrams) and made `generate.sh` executable :
```
$ sudo apt install graphviz
$ chmod +x generate.sh
```
Finally, you can open the documentation in a webpage by opening the generated file `docs.html`.