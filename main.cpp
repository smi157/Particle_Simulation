#include <raylib.h>
#include <raymath.h>
#include "include/particle_system.hpp"
#include "include/grid.hpp"

#define RAYGUI_IMPLEMENTATION
#include <raygui.h>





int main(){

    InitWindow(1280, 720, "Particle_Physics");

    int monitor = GetCurrentMonitor();
    float width = GetMonitorWidth(monitor);
    float height = GetMonitorHeight(monitor);


    SetTargetFPS(60);

   

    SetWindowSize(width, height);
    ToggleFullscreen();


    

    particle_system_2D test;

    const int substeps =4;
    const int iterations = 3;
    const float vel_damp = 0.999;


    
    float curr_rad = 20;
    float true_max_rad = curr_rad;

    float curr_mass=20;


    double cell_size = 2.0 * curr_rad;
    grid Grid{cell_size, width, height};
   
    Rectangle button_to_spawn_particle = {0, 0, width, height};
    Rectangle slider_hitbox_rad = { 200, 170, 400, 30 };
    Rectangle slider_hitbox_mass = {250, 210, 400, 30};



    
    while (!WindowShouldClose()){


        float delta_time = GetFrameTime();

        float substep_dt = delta_time/substeps;

        Vector2 mousePos = GetMousePosition();



        
        if (CheckCollisionPointRec(mousePos,button_to_spawn_particle) && !CheckCollisionPointRec(mousePos,slider_hitbox_rad) && !CheckCollisionPointRec(mousePos,slider_hitbox_mass)){
            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
                test.add_particle(mousePos,curr_rad,curr_mass);

                if (curr_rad>true_max_rad){
                    true_max_rad= curr_rad;
                }
            }
        }



        

        double desired_cell = 2.0 * true_max_rad;
        
        if (desired_cell != Grid.cell_size) {
            cell_size = desired_cell;
            Grid = grid(cell_size, width, height);
        }


        

        for (int s = 0; s < substeps; ++s) {
            update_pos(test, substep_dt,vel_damp);
            collision_to_wall(test, substep_dt, width, height);

            for (int it = 0; it < iterations; ++it) {
                Grid.build(test);
                collision_to_particle(test, Grid);
                
            }
        }

        

        BeginDrawing();

            DrawFPS(0,0);
            ClearBackground(BLACK);

            draw(test);

            GuiSlider(slider_hitbox_rad, "0.0", "200.0", &curr_rad, 0.0f, 200.0f);
            GuiSlider(slider_hitbox_mass, "0.0", "200.0", &curr_mass, 0.0f, 200.0f);


        EndDrawing();

    }


    CloseWindow();
    return 0;
}