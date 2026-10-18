const float gravity = 500;
const float restitution_particle_particle = 1;
const float restitution_particle_wall = 0.9;



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

// using both const and & to pass it like no-copy and read-only.
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