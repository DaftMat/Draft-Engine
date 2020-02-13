# Daft-Engine

Render Engine made in C++/OpenGL/Qt.

## Dependencies

 - [GLM](https://github.com/g-truc/glm) (As submodule in repository)
 - GLSL 4.10+
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
 * You can switch scenes using numbers (0 to 3)
   * Use the 1st scene (0) to clear OpenGL context
   * The 2 nexts scenes were provided by teachers
   * My work is on the 4th scene, from now I assume you're using the engine from this scene.
 * You can print wireframed object using `w` (same key to get triangles back)
 * You can switch shaders using `s`
   * A Blinn-Phong shaders with grey default materials
   * An Error shader that shows the spheres with green for parts close to the perfect sphere, and red for errors parts
   * A normal shader that make the spheres look like rainbow
 * You can switch cameras using `p`
   * A standard camera that you can move using arrow-keys and mouse-click
   * A TrackBall camera the cant move except around the center
 * You can switch selected object using `o` (Yellow wireframed object is the selected one)
 * You can add/delete vertices on the selected spheres
   * `+` to add vertices
   * `-` to delete vertices