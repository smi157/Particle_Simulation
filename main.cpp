#include <iostream>
#include <raylib.h>
#include <raymath.h>
#include <vector>
#include "include/particle_system.hpp"
#include "include/grid.hpp"





int main(){

    InitWindow(1280, 720, "Particle_Physics");

    int monitor = GetCurrentMonitor();
    float width = GetMonitorWidth(monitor);
    float height = GetMonitorHeight(monitor);


    SetTargetFPS(60);

   

    SetWindowSize(width, height);
    ToggleFullscreen();


    

    particle_system_2D test;

    
    int max_rad = 20;
    double cell_size = 2.0 * max_rad;
    grid Grid{cell_size, width, height};
   
    Rectangle button = {0, 0, width, height};



    
    while (!WindowShouldClose()){


        float delta_time = GetFrameTime();

        Vector2 mousePos = GetMousePosition();



        
        if (CheckCollisionPointRec(mousePos,button)){
            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)){        
                test.add_particle(mousePos);

                int new_rad = test.rad_list.back();
                if (new_rad > max_rad) {
                    max_rad = new_rad;
                    cell_size = 2.0 * max_rad;
                    Grid = grid(cell_size, width, height);
                }
                

            }
        }

        update_pos(test,delta_time);
        collision_to_wall(test,delta_time,width,height);
        

        Grid.build(test);

        collision_to_particle(test,Grid);


        BeginDrawing();

            DrawFPS(0,0);
            ClearBackground(BLACK);

            draw(test);

        EndDrawing();

    }


    CloseWindow();
    return 0;
}