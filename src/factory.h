#pragma once
#include "./config.h"
#include "./shader.h"
#include "./components/components.h"

class Factory {
    public:
        Factory(World& world, Cloth& cloth, std::vector<Point>& points, std::vector<Stick>& sticks, 
                std::vector<Quad>& quads, std::vector<Light>& lights, std::vector<Point>& balls,
                std::vector<Stick>& springs);
        ~Factory();
        void make_cloth();
        void make_points();

        void make_ball_spring(float xpos, float ypos);
        Point make_ball(float xpos, float ypos, int scale, int mass, glm::vec3 force);

    private:
        int pointLength;
        int betweenDistance;
        World& world;
        Cloth& cloth;
        std::vector<Point>& points;
        std::vector<Stick>& sticks;
        std::vector<Quad>& quads;
        std::vector<Light>& lights;
        std::vector<Point>& balls;
        std::vector<Stick>& springs;

        float normalize_position(float position, int cellLength, int scrLength);
        
        
        void make_sticks();
        void make_quads();
        void make_lights();

        unsigned int make_transform_buffer();
        unsigned int make_color_buffer();
        Mesh make_point_instance();
        Mesh make_stick_instance();
        Mesh make_quad_instance(std::vector<float> vertices);
        Mesh make_light_instance();
        


};