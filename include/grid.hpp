#pragma once

#include "particle_system.hpp"
#include <cmath>
#include <vector>
#include <array>
#include <algorithm>



struct grid{
    double cell_size;
    int num_rows;
    int num_cols;
    int num_cells;

    // bucket[i] = list of particle indices in cell i
    //indexed by row * num_cols + col
    // can be look like this : [[1,3,5],[6,7,2],...]
    std::vector<std::vector<int>> bucket;

    std::vector<std::array<int,5>> neighbors;   // num_cells entries, -1 = unused

    grid(double cell_size, float width, float height)
        : cell_size(cell_size),
          num_rows((int)std::ceil(height / cell_size)),
          num_cols((int)std::ceil(width  / cell_size)),
          num_cells(num_rows * num_cols)
    {
        resize();
        build_neighbors();
    }


    void clear() {
        for (auto &b : bucket) b.clear();
    }

    void resize(){
        bucket.assign(num_cells, {});

        neighbors.assign(num_cells, {-1, -1, -1, -1, -1});

    }

    int cell_index(int row, int col) const {
        row = std::clamp(row, 0, num_rows - 1);
        col = std::clamp(col, 0, num_cols - 1);
        return row * num_cols + col;
    }

    int cell_of(float x, float y) const {
        int col = (int)std::floor(x / cell_size);
        int row = (int)std::floor(y / cell_size);
        return cell_index(row, col);
    }


    void build(particle_system_2D &system) {
        clear();
        int n = system.get_size();
        for (int i = 0; i < n; ++i) {
            int c = cell_of(system.pos_list[i].x, system.pos_list[i].y);
            bucket[c].push_back(i);
        }
    }

    

    //half neighbor checking, puts the current cell and the 4 adjacent ones to the neighbor list. total 5 entries.
    void build_neighbors() {
        



        for (int c = 0; c < num_cells; ++c) {
            int row = c / num_cols, col = c % num_cols;
            int n = 0;
            //right of the current cell
            neighbors[c][n] = c;
            if (col + 1 < num_cols) {
                neighbors[c][n++] = c + 1;
            }
            
            if (row + 1 < num_rows) {
                int below = c + num_cols;

                if (col - 1 >= 0){
                    //bottom-left of the current cell
                    neighbors[c][n++] = below - 1;
                }
                //below of the current cell
                neighbors[c][n++] = below;
                //bottom-right of the current cell

                if (col + 1 < num_cols) {
                    neighbors[c][n++] = below + 1;
                }
                
            }

            
        }
    }

};





// we will get which particle index is stored in which grid cell from the Grid's bucket vector.
inline void collision_to_particle(particle_system_2D &system, const grid &Grid) {
    for (int c = 0; c < Grid.num_cells; ++c) {
        const std::vector<int> &B = Grid.bucket[c];

        // same cell
        for (size_t a = 0; a < B.size(); ++a){
            for (size_t b = a + 1; b < B.size(); ++b){
                resolve_pair(system, B[a], B[b]);
            }
                
        }
            

        //neighbors
        for (int q = 0; q < 4; ++q) {
            int nc = Grid.neighbors[c][q];

            if (nc ==-1) continue;  // unused(no neighbor)

            const std::vector<int> &N = Grid.bucket[nc];
            for (int i : B){
                for (int j : N){
                    resolve_pair(system, i, j);
                }
            }
        }
    }
}