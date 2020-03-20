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
    - **scale**
    - **models**
      - **model**

Each **scene** can have 0 or 1 **lights**.

Each **lights** can have 0 or many **light**.

Each **group** can hava 0 or many **group** childs. 

Each **group** can have 0 or 1 **models**. 

The order of the geometric transformations isn't fixed.

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
