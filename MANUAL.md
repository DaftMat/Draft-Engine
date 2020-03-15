# Daft-Engine Manual

![Edition Mode](https://imgur.com/puto7YX.png)

## Object management

### Creation
You can create Lights or Models by picking one on the top left corner of the engine.
 - Lights
   - Point light
   - Directional light
   - Spot light
 - Models
   - Cube
   - Sphere (UV, Ico or Cube)

### Edition
You can then select any object by clicking on it. A lot of settings are now available :
 - Transformation settings :
   - When an object is selected a `Gizmo` appears. You can translate or scale the object using this. 
   Select what gizmo you need with the top middle group box
   - Aswell, spinners are on the middle bottom of the engine for each possible transformation.
 - Object's settings : 
   - Each object have particular settings (e.g. UVSphere : meridians and parallels), 
    you can find them on the bottom right corner on the engine.
 - Material settings (Models rendered with PBR shader only) :
   - Each model have its own material, it can be found on the bottom left corner of the engine.

### Deletion
Click `Delete` button on the top 
left corner of the engine to delete the selected object.

## Shaders and Edition mode

### Shaders
You can set the shader you want by choosing it using the top right corner group box.
Here are the available shaders :
 - Phong
 - Blinn-Phong
 - PBR
 - Error (for spheres)
 - Normal (print normals as color)
 
### Edition Mode
The edition Mode is the way you see the Engine on the first picture of this Manual. 
Toggling rendering mode will make it look like it :

![Rendering Mode](https://imgur.com/e1s2Wiq.png)

No Gizmos, no edges on Models, no visual models for Lights and a black sky, 
you can just enjoy the rendering part of the Engine.