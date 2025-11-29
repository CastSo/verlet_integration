#pragma once
#include "./config.h"
#include "./shader.h"
#include "./components/components.h"

class Factory {
    public:
        Factory(World& world, Cloth& cloth, std::vector<std::vector<int>>& graph, std::vector<Point>& points,
                 std::vector<Stick>& sticks, std::vector<Quad>& quads, std::vector<Light>& lights, 
                std::vector<Point>& nodes, Stick& spring);
        ~Factory();
        void make_cloth();
        void make_points();
        Mesh make_stick_instance();

        Point make_node(float xpos, float ypos, int scale, glm::vec3 color, int mass, glm::vec3 force);
        void connect_node(int fromNode, int toNode);

        void add_box_body(float xpos, float ypos);
        void add_node(float xpos, float ypos);
    private:
        int pointLength;
        int betweenDistance;
        World& world;
        Cloth& cloth;
        std::vector<std::vector<int>>& graph;
        std::vector<Point>& points;
        std::vector<Stick>& sticks;
        std::vector<Quad>& quads;
        std::vector<Light>& lights;
        std::vector<Point>& nodes;
        Stick& spring;

        float normalize_position(float position, int cellLength, int scrLength);
        
        
        void make_sticks();
        void make_quads();
        void make_lights();

        unsigned int make_transform_buffer();
        unsigned int make_color_buffer();
        Mesh make_point_instance();
        Mesh make_quad_instance(std::vector<float> vertices);
        Mesh make_light_instance();
        


};