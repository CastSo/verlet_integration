#pragma once
#include "./config.h"
#include "./components/components.h"

class Particle {
    public:
        Particle(World& world,  Cloth& cloth, std::vector<Point>& points, std::vector<Point>& balls);
        ~Particle();
        void update(float deltaTime);
        

    private:
        World& world;
        Cloth& cloth;
        std::vector<Point>& points;
        std::vector<Point>& balls;
        
        float process_verlet(float deltaTime, float position, float prevPosition, float force, float mass);
        void clamp_particles(Point& point);
        void clamp_to_screen(Point& point);
        void satisfy_constraints(Point& p1, Point& p2);

        void update_points(float deltaTime);
        void update_balls(float deltaTime);

};