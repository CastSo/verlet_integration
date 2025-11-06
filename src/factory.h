#pragma once
#include "./config.h"
#include "./shader.h"
#include "./components/components.h"

class Factory {
    public:
        Factory(World& world, Cloth& cloth, std::vector<Point>& points, std::vector<Stick>& sticks, std::vector<Quad>& quads);
        ~Factory();
        void make_points();
        void make_sticks();
        void make_quads();

    private:
        int pointLength;
        int betweenDistance;
        World& world;
        Cloth& cloth;
        std::vector<Point>& points;
        std::vector<Stick>& sticks;
        std::vector<Quad>& quads;

        float normalize_position(float position, int cellLength, int scrLength);

        unsigned int make_transform_buffer();
        unsigned int make_color_buffer();
        Mesh make_point_instance();
        Mesh make_stick_instance();
        Mesh make_quad_instance(std::vector<float> vertices);


};