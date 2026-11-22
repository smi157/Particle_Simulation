# Particle_Simulation


**Work In Progress**


AI did most of the math calculations for verlet integration and impulse calculations for particles after collision, other stuff like code structures, code logic, and optimizations are done by me.



## Optimizations
- SIMD vectorization 
- gpu compute path
- spatial grid




## TO-DO

- Collision, grid system(spatial grid) {X}
- Make a restitution slider. {}
- There can be sliders for the current particle that will be added(mass,charge, radius, velocity, acceleration, etc...) {}
- Maybe particles can also be created by using a slingshot feature, when left mouse button is down it will enter a slingshot state and it will determine the velcoity of the particle{}
- Add fluid and gas sim. {}
- Add 3D and 1D sim {}
- Normal kinematics equations, spring, centripedal force, gravity, electromagnetic {}




## COMPILE




Raygui source code can be found here: https://github.com/raysan5/raygui



**If the raylib library was installed gloablly and Raygui was compiled from the source code:**

- g++ main.cpp -o test -I"Raygui src folder path" -L"Raygui folder path" -lraylib

- If the raylib was also compiled from source you can link it to the end.
