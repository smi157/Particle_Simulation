#include <iostream>
#include <raylib.h>
#include <raymath.h>
#include <vector>
#include <cmath>











//SoA
// usage: particle_system.pos[0....n] to get the nth particle's variables.
struct 2D_particle_system{
    std::vector<Vector2> pos_list;
    std::vector<Vector2> vel_list;
    std::vector<Vector2> acc_list;
    std::vector<Vector2> rad_list;
    std::vector<Vector2> mass_list;
    std::vector<Vector2> charge_list;


    Vector2 *pos;
    Vector2 *vel;
    Vector2 *acc;
    Vector2 *rad;
    Vector2 *mass;
    Vector2 *charge;

};

// struct Particle{
//     Vector2 pos;

//     Vector2 vel;

//     Vector2 acc;

//     int mass= 20;

//     int radius =20;

//     float charge=0;
// };


void draw(){
    for (int i=0; i<pos_list.size(); i++){
        DrawCircle(particle_system.pos[i].x,particle_system.pos[i].y);
    }
}

// void update_pos(){

// }


int main(){

    // get the coord of the mouse and emplace back with the coord. {}
    // there can be a stretchable line, it determines the velocity of the particle. {}
    // idk about acc right now to hwo to determine it. {}
    // can be fluid or air sim when launching the particles. {}
    // there can also be 3d sim {}
    // normal kinetmics equations, spring, centripedal force, gravity, electric, maybe nuclear {}
    // LAAMPS library implementation {}
    // optimizations {}
    




    InitWindow(1280, 720, "Particle_Physics");

    int monitor = GetCurrentMonitor();
    float width = GetMonitorWidth(monitor);
    float height = GetMonitorHeight(monitor);


    SetTargetFPS(60);

   

    SetWindowSize(width, height);
    ToggleFullscreen();


    //std::vector<Particle> particles;


    //Color button_color{0,0,0,0};
    Rectangle button = {0, 0, width, height};



    
    while (!WindowShouldClose()){


        float delta_time = GetFrameTime();

        Vector2 mousePos = GetMousePosition();



        
        if (CheckCollisionPointRec(mousePos,button)){
            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
                particle_system a = {&mousePos,Vector2{0,0}};
            }
        }



        BeginDrawing();

            DrawFPS(0,0);
            ClearBackground(BLACK);

            draw();
            //update_pos();
            

        





        EndDrawing();

    }


    CloseWindow();
    return 0;
}