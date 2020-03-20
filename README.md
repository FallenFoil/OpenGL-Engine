# OpenGL-Engine

Two programs that create and visualize objects using math and **OpenGL**.

## Engine

An engine that uses **OpenGL** and **C++** to draw scenes.

A scenes are descibed by an XML file, containing the path for the models, the lightining, translations, rotations, scale, and movement using Catmull-Rom curves. These scenes have the following hierarchy:

- **scene**
  - **lights**
    - **light**
  - **group**
    - **rotate**
    - **translate**
      - **point**
    - **scale**
    - **models**
      - **model**

Each **scene** can have 0 or 1 **lights**.

Each **lights** can have 0 or many **light**.

Each **group** can hava 0 or many **group** childs. 

Each **group** can have 0 or 1 **models**. 

The order of the geometric transformations isn't fixed.

The **scene** tag can have the following attributes:
- **CamX**, x position of the camera;
- **CamY**, y position of the camera;
- **CamZ**, z position of the camera;
- **LookAtX**, x positition of the point that the camera is looking at;
- **LookAtY**, y positition of the point that the camera is looking at;
- **LookAtZ**, z positition of the point that the camera is looking at;

The **light** tag can have the following attributes:
- **ambR**, red value of the ambient color between 0 and 255;
- **ambG**, green value of the ambient color between 0 and 255;
- **ambB**, blue value of the ambient color between 0 and 255;
- **diffR**, red value of the diffuse color between 0 and 255;
- **diffG**, green value of the diffuse color between 0 and 255;
- **diffB**, blue value of the diffuse color between 0 and 255;
- **Type**, that can be:
  - DIRECTIONAL;
  - POINT;
  - SPOT.
- **dirX**, x direction of the DIRECTIONAL and SPOT light;
- **dirY**, y direction of the DIRECTIONAL and SPOT light;
- **dirZ**, z direction of the DIRECTIONAL and SPOT light;
- **posX**, x position of the POINT and SPOT light;
- **posY**, Y position of the POINT and SPOT light;
- **posZ**, Z position of the POINT and SPOT light;

The **rotate** tag can have the following attributes:
- **axisX**, can be 0 or 1. If 1 the rotation is centered in the X axis;
- **axisY**, can be 0 or 1. If 1 the rotation is centered in the Y axis;
- **axisZ**, can be 0 or 1. If 1 the rotation is centered in the Z axis;
- **ang**, for the angle to rotate. Or **time** to rotate 360 degrees in the given seconds

The **translate** tag can have the following attributes:
- **X**, how many to move in the x axis;
- **Y**, how many to move in the y axis;
- **Z**, how many to move in the z axis;
- **time**, time make a catmull-Rom curve. If time is present, the **X**, **Y**, **Z** are ignored and is expected to exists at least four children **points**;
- **drawOrbit**, can be *true* or *false*, prints the orbit of the movement.

The **point** tag can have the following attributes:
- **X**, how many to move in the x axis;
- **Y**, how many to move in the y axis;
- **Z**, how many to move in the z axis;

The **scale** tag can have the following attributes:
- **X**, how many to scale in the x axis;
- **Y**, how many to scale in the y axis;
- **Z**, how many to scale in the z axis;

The **model** tag can have the following attributes:
- **file**, path to the shape file;
- **texture**, path to the texture file
- **R**, red value for the color of the shape;
- **G**, green value for the color of the shape;
- **B**, blue value for the color of the shape;
- **diffR**, red value for the diffuse color of the shape;
- **diffG**, green value for the diffuse color of the shape;
- **diffB**, blue value for the diffuse color of the shape;
- **specR**, red value for the specular color of the shape;
- **specG**, green value for the specular color of the shape;
- **specB**, blue value for the specular color of the shape;
- **emissR**, red value for the emissive color of the shape;
- **emissG**, green value for the emissive color of the shape;
- **emissB**, blue value for the emissive color of the shape;
- **ambiR**, red value for the ambiance color of the shape;
- **ambiG**, green value for the ambiance color of the shape;
- **ambiB**, blue value for the ambiance color of the shape;

## Generator

The **Generator** creates geometric shape using triangles. The first line of the shape file represents the number of points that makes the shape. For each point it is calculates its coordiantes, normal and texture coordinates.

### Instructions


#### Plane
Origin centered Plane with **width** (x) and **height** (z) equal to 10 and the **output file** is **plane.3d**.   
`./Generator plane 10 plane.3d`

#### Box
Box with **length** (x) equal to 2, **height** (y) equal to 3, **width** (z) equal to 4, 1 slice, 1 stack, 1 depthDivision and the **output file** is **box.3d**.   
`./Generator box 2 3 4 box.3d`   
or   
Box with **length** (x) equal to 2, **height** (y) equal to 3, **width** (z) equal to 4, 5 slice, 6 stack, 7 depthDivision and the **output file** is **box.3d**.   
`./Generator box 2 3 4 5 6 7 box.3d`

#### Sphere
Sphere with **radius** equal to 2, 20 **slices**, 30 **stacks** and the **output file** is **sphere.3d**.   
`./Generator sphere 2 20 30 sphere.3d`

#### Reverse Sphere
Reverse sphere with **radius** equal to 2, 20 **slices**, 30 **stacks** and the **output file** is **reversesphere.3d**.   
`./Generator reversesphere 2 20 30 reversesphere.3d`

#### Cone
Cone with **radius** equal to 2, **height** equal to 5, 20 **slices**, 3 **stacks** and the **output file** is **cone.3d**.   
`./Generator cone 2 5 20 3 cone.3d`

#### Bezier Patches
Bezier Patch in file **bezier.patch** and with **tessellation level** equal to 10 and the **output file** is **bezier.3d**.   
`./Generator bezier bezier.patch 10 bezier.3d`

#### Ring
Ring with inner **radius** equal to 2, outter **radius** equal to 5, 20 **slices** and the **output file** is **ring.3d**.   
`./Generator ring 2 5 20 ring.3d`

## Contributors

[Cesar Borges](https://github.com/CesarAugustoBorges)

[Guilherme Viveiros](https://github.com/GuilhermeViveiros)

[Luis Macedo](https://github.com/FallenFoil)
