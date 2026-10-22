#pragma once

#include <raylib.h>
#include <vector>

inline const float gravity = 500;
inline const float restitution_particle_particle = 0.75;
inline const float restitution_particle_wall = 0.75;



//SoA (Struct of Arrays)
// usage: particle_system_2D.pos_list[0....n] to get the nth particle's posiiton.
// Same for other variables.
struct particle_system_2D{
    std::vector<Vector2> pos_list;
    std::vector<Vector2> prev_pos_list; // for verlet integration
    std::vector<Vector2> vel_list;
    std::vector<Vector2> acc_list;
    std::vector<int> rad_list;
    std::vector<float> mass_list;
    std::vector<int> charge_list;
    

    std::vector<bool> just_added;


    


   void add_particle(Vector2 pos, Vector2 vel={500,150}, Vector2 acc={0,gravity}, int rad=20, int mass=20, int charge=0){
    pos_list.push_back(pos);
    prev_pos_list.push_back(pos);
    vel_list.push_back(vel);
    acc_list.push_back(acc);
    rad_list.push_back(rad);
    mass_list.push_back(mass);
    charge_list.push_back(charge);
    
    


    just_added.push_back(true);
   }

   int get_size() const{
    return pos_list.size();
   }
};




void update_pos(particle_system_2D &system, float dt){
    int size = system.get_size();

    for (int i = 0; i < size; i++){
        

        // Making this change because otherwise prev_pos and current pos becomes the same and the velocity equals to zero all the time.
        // the particle will not have a prev position when first added.
        if (system.just_added[i]){
            system.prev_pos_list[i].x = system.pos_list[i].x - system.vel_list[i].x * dt;
            system.prev_pos_list[i].y = system.pos_list[i].y - system.vel_list[i].y * dt;
            system.just_added[i] = false;
        }

        

        Vector2 next;
        next.x = 2.0f * system.pos_list[i].x - system.prev_pos_list[i].x + system.acc_list[i].x * dt * dt;
        next.y = 2.0f * system.pos_list[i].y - system.prev_pos_list[i].y + system.acc_list[i].y * dt * dt;

        system.prev_pos_list[i] = system.pos_list[i];


        system.vel_list[i].x = (next.x - system.pos_list[i].x) / dt;
        system.vel_list[i].y = (next.y - system.pos_list[i].y) / dt;


        system.pos_list[i] = next;
        
    }

    
}

// using both "const" and "&" to pass it like read-only and no-copy.
void draw(const particle_system_2D &system){
    for (int i=0; i<system.get_size(); i++){
        DrawCircle(system.pos_list[i].x,system.pos_list[i].y, system.rad_list[i], YELLOW);
    }
    
}

void collision_to_wall(particle_system_2D &system, float dt,float width,float height){
    int size = system.get_size();

    for (int i = 0; i < size; i++){
        //right wall
        if (system.pos_list[i].x + system.rad_list[i] >= width){
            float vx = system.pos_list[i].x - system.prev_pos_list[i].x;
            system.pos_list[i].x = width - system.rad_list[i];
            system.prev_pos_list[i].x = system.pos_list[i].x + vx * restitution_particle_wall;
        }
        // left wall
        if (system.pos_list[i].x - system.rad_list[i] <= 0){
            float vx = system.pos_list[i].x - system.prev_pos_list[i].x;
            system.pos_list[i].x = system.rad_list[i];
            system.prev_pos_list[i].x = system.pos_list[i].x + vx * restitution_particle_wall;
        }
        // bottom
        if (system.pos_list[i].y + system.rad_list[i] >= height){
            float vy = system.pos_list[i].y - system.prev_pos_list[i].y;
            system.pos_list[i].y = height - system.rad_list[i];
            system.prev_pos_list[i].y = system.pos_list[i].y + vy * restitution_particle_wall;
        }
        // top
        if (system.pos_list[i].y - system.rad_list[i] <= 0){
            float vy = system.pos_list[i].y - system.prev_pos_list[i].y;
            system.pos_list[i].y = system.rad_list[i];
            system.prev_pos_list[i].y = system.pos_list[i].y + vy * restitution_particle_wall;
        }
    }
}




inline void resolve_pair(particle_system_2D &system, int i, int j) {
    Vector2 d = {
        system.pos_list[j].x - system.pos_list[i].x,
        system.pos_list[j].y - system.pos_list[i].y
    };


    float r_sum = system.rad_list[i] + system.rad_list[j];
    float dist2 = d.x*d.x + d.y*d.y;

    if (dist2 >= r_sum*r_sum || dist2 < 1e-6f) return;

    float dist = std::sqrt(dist2);

    Vector2 nrm = { d.x / dist, d.y / dist };
    float overlap = r_sum - dist;

    float m_i = system.mass_list[i];
    float m_j = system.mass_list[j];
    float total = m_i + m_j;
    float corr_i = overlap * (m_j / total);
    float corr_j = overlap * (m_i / total);

    system.pos_list[i].x -= nrm.x * corr_i;
    system.pos_list[i].y -= nrm.y * corr_i;
    system.pos_list[j].x += nrm.x * corr_j;
    system.pos_list[j].y += nrm.y * corr_j;

    Vector2 dp_i = {
        system.pos_list[i].x - system.prev_pos_list[i].x,
        system.pos_list[i].y - system.prev_pos_list[i].y
    };
    Vector2 dp_j = {
        system.pos_list[j].x - system.prev_pos_list[j].x,
        system.pos_list[j].y - system.prev_pos_list[j].y
    };
    float vn = (dp_j.x - dp_i.x) * nrm.x + (dp_j.y - dp_i.y) * nrm.y;
    if (vn > 0) return;
    float impulse = -(1.0f + restitution_particle_particle) * vn / (1.0f/m_i + 1.0f/m_j);
    dp_i.x -= (impulse / m_i) * nrm.x;
    dp_i.y -= (impulse / m_i) * nrm.y;
    dp_j.x += (impulse / m_j) * nrm.x;
    dp_j.y += (impulse / m_j) * nrm.y;

    system.prev_pos_list[i] = {
        system.pos_list[i].x - dp_i.x,
        system.pos_list[i].y - dp_i.y
    };
    system.prev_pos_list[j] = {
        system.pos_list[j].x - dp_j.x,
        system.pos_list[j].y - dp_j.y
    };
}