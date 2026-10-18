#include <iostream>
#include <raylib.h>
#include <raymath.h>
#include <vector>
#include <unordered_map>
#include "include/particle.hpp"
#include "include/grid.hpp"




int main(){


    // collision, grid system, make a restitution slider {}
    // there can be a stretchable line, it determines the velocity of the particle. {}
    // idk about acc right now to hwo to determine it. {}
    // can be fluid or air sim when launching the particles. {}
    // there can also be 3d and 1d sim {}
    // normal kinetmics equations, spring, centripedal force, gravity, electric, maybe nuclear {}
    // optimizations(SIMD vectorizartion, gpu compute path) {}
    



    
    particle_system_2D test;

    InitWindow(1280, 720, "Particle_Physics");

    int monitor = GetCurrentMonitor();
    float width = GetMonitorWidth(monitor);
    float height = GetMonitorHeight(monitor);


    SetTargetFPS(60);

   

    SetWindowSize(width, height);
    ToggleFullscreen();


    


   
    Rectangle button = {0, 0, width, height};



    
    while (!WindowShouldClose()){


        float delta_time = GetFrameTime();

        Vector2 mousePos = GetMousePosition();



        
        if (CheckCollisionPointRec(mousePos,button)){
            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
                test.add_particle(mousePos);
            }
        }



        BeginDrawing();

            DrawFPS(0,0);
            ClearBackground(BLACK);

            draw(test);
            update_pos(test,delta_time);
            collision_to_wall(test,delta_time,width,height);
        





        EndDrawing();

    }


    CloseWindow();
    return 0;
}