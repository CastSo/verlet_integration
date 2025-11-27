#pragma once
#include "./config.h"
#include "./components/components.h"

class Particle {
    public:
        Particle(World& world,  Cloth& cloth, std::vector<std::vector<int>>& graph, std::vector<Point>& points, std::vector<Point>& nodes);
        ~Particle();
        void update(float deltaTime);
        void check_all_collisions();

    private:
        World& world;
        Cloth& cloth;
        std::vector<std::vector<int>>& graph;
        std::vector<Point>& points;
        std::vector<Point>& nodes;
        
        void update_points(float deltaTime);
        void update_nodes(float deltaTime);

        bool check_collision(Point p1, Point p2);
        void process_force(Point p1, Point p2, float deltaTime);
        void process_collision(Point p1, Point p2, float deltaTime);

        float process_verlet(float deltaTime, float position, float prevPosition, float force, float mass);
        void clamp_particles(Point& point, float xBaseLen, float yBaseLen);
        void clamp_to_screen(Point& point, float delaTime);
        void satisfy_constraints(Point& p1, Point& p2, float restLength);  
        bool is_connected(int id1, int id2);

       

};