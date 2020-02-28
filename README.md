# Daft-Engine

Render Engine made in C++/OpenGL/Qt.

## Dependencies

 - [GLM](https://github.com/g-truc/glm) (As submodule in repository)
 - [Eigen](https://gitlab.com/libeigen/eigen) (As submodule in repository)
 - GLSL/OpenGL 4.10+
 - Qt

## Downloading
Run the following command :
```txt
# git clone --recurse-submodules https://github.com/DaftMat/Daft-Engine.git
```

## Compiling
Run the following commands from the root directory of the downloaded repository :
```txt
# mkdir build
# cd build
# cmake ..
# make
```

## Executing
Run the following commands from the recently created `build` directory
```
# cd ../bin
# ./Daft-Engine
```
## Using
 
![Annoted App](https://imgur.com/L0lIBdU.png)

 * You can switch cameras using `p` (no button implemented on the GUI yet...)
 * Creation/Deletion management **(1)**
   * Create an new object **(1.1)** (only spheres for now)
   * Delete the selected object **(1.2)**
 * Change shader **(2)**
 * Transformation management **(3)**
   * Edit selected object's position **(3.1)**
   * Edit selected object's rotations **(3.2)**
   * Edit selected object's scale **(3.3)**
 * Object's settings management **(4)**
   * Depends on the selected object
 * You can select an object by clicking on it