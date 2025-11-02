#pragma once
#include "./config.h"
#include "./components/components.h"

class Particle {
    public:
        Particle(World& world, std::vector<Point>& points, std::vector<Stick>& sticks);
        ~Particle();
        void update(float deltaTime);
        

    private:
        World& world;
        std::vector<Point>& points;
        std::vector<Stick>& sticks;
        
        float normalize_position(float position, int cell_length, int scr_length);
        void process_verlet(float deltaTime, Point& point);
        void update_sticks();

};