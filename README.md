# Height Field Fluid Fountain

Project Idea: Implement a realistic garden/park scene of a fountain with a 3D model, several textures including an environmental cubemap, and a height field particle system to control the ebb and flow of the water in the fountain.

![rep](https://user-images.githubusercontent.com/60118290/234731914-f8d3ba78-2efe-4eb9-900e-85f1167efcfb.png)

## How to build

*Windows*

Open git bash to the directory containing this repository.

```
project-template $ mkdir build
project-template $ cd build
project-template/build $ cmake ..
project-template/build $ start project-template.sln
```

Your solution file should contain four projects.
To run from the git bash command shell, 

```
project-template/build $ ../bin/Debug/demo.exe
```

*macOS*

Open terminal to the directory containing this repository.

```
project-template $ mkdir build
project-template $ cd build
project-template/build $ cmake ..
project-template/build $ make
```

To run each program from build, you would type

```
project-template/build $ ../bin/demo
```


## Demo of basic features

A height field is just a function that changes the height of particles that move around or a stationary grid of blocks  to mimic the laws of fluid dynamics found in physics. The particular data structure I used in this project was a 2D plane mesh extended to 3D with the height field function controlling the y-coordinates since the y axis is "up" in my simulation (Normoyle, Aline 2020). Because the fountain basin is circular, my implementation also "pulled in" the corners of the plane by changing the (x,y) coordinates of points on the plane further than a given radius away from the center of the plane to the positions at the same angle from the center but on the perimeter of the circle.

When the mesh updates each frame, the height function is called on each point with the elapsed time since the simulation started acting as an angle for a sine function later. The function is heightFn(p) = (elapsedTime + frequency * p.x * frequency * p.z) where the frequency represents how often the water undulates, which I just made a constant. The y-coordinate can then be calculated as p.y = amplitude * sin(heightFn(p)), where amplitude is also a constant representing the maximum and minimum height differentials of the undulations.

Since vertex normals are so integral to the shading of meshes, a separate buffer of normal vectors is also updated alongside the positions. The updates basically look near each point and calculate the x and z axis relative to each point to be the change in those values around the point, respectively. The y axis relative to the point is then computed to be the cross product of the z and x axis, which when normalized gives us a realistic approximation of the normal at a point.

I didn't feel a need to shade the water any more than just applying a texture and calculating a reflective component, but a phong shader or some other shader could have easily been added with the setting sun in the cubemap acting as a light source. The fountain itself however was shaded and textured in a phong pixel shader.

## Unique features 

![demo10mb](https://user-images.githubusercontent.com/60118290/234721164-c930bc46-7913-4a7f-86de-4b02ad11588d.gif)
