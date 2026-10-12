#include <iostream>
#include <raylib.h>
#include <raymath.h>
#include <vector>
#include <cmath>



const float gravity = 500;


//SoA
// usage: 2D_particle_system.pos[0....n] to get the nth particle's variables.
struct particle_system_2D{
    std::vector<Vector2> pos_list;
    std::vector<Vector2> vel_list;
    std::vector<Vector2> acc_list;
    std::vector<int> rad_list;
    std::vector<int> mass_list;
    std::vector<int> charge_list;




   void add_particle(Vector2 pos, Vector2 vel={150,150}, Vector2 acc={0,0}, int rad=20, int mass=0, int charge=0){
    pos_list.push_back(pos);
    vel_list.push_back(vel);
    acc_list.push_back(acc);
    rad_list.push_back(rad);
    mass_list.push_back(mass);
    charge_list.push_back(charge);
   }

   int get_size(){
    return pos_list.size();
   }

   void update(float dt){
    int size = get_size();
    for (int i=0; i<size; i++){
        pos_list[i].x+= vel_list[i].x * dt + (0.5)*dt*dt*acc_list[i].x;
        pos_list[i].y+= vel_list[i].y * dt + (0.5)*dt*dt*acc_list[i].y;
    }
   }

   

   

};




void draw(particle_system_2D &system){
    for (int i=0; i<system.get_size(); i++){
        DrawCircle(system.pos_list[i].x,system.pos_list[i].y, system.rad_list[i], YELLOW);
    }
    
}



int main(){



    // get the coord of the mouse and emplace back with the coord. {}
    // there can be a stretchable line, it determines the velocity of the particle. {}
    // idk about acc right now to hwo to determine it. {}
    // can be fluid or air sim when launching the particles. {}
    // there can also be 3d sim {}
    // normal kinetmics equations, spring, centripedal force, gravity, electric, maybe nuclear {}
    // LAAMPS library implementation {}
    // optimizations {}
    



    
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
            test.update(delta_time);
            

        





        EndDrawing();

    }


    CloseWindow();
    return 0;
}