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

#### Sphere


## Contributors

[Cesar Borges](https://github.com/CesarAugustoBorges)

[Guilherme Viveiros](https://github.com/GuilhermeViveiros)

[Luis Macedo](https://github.com/FallenFoil)
