#pragma once
#include "./config.h"
#include "./shader.h"
#include "./components/components.h"


class Render {
    public:
        Render(World& world,  Cloth& cloth, std::vector<std::vector<int>>& graph, std::vector<Point>& points, 
            std::vector<Stick>& sticks, std::vector<Quad>& quads, std::vector<Light>& lights,
            std::vector<Point>& nodes, Stick& spring); 
        ~Render();
        void update_cloth(bool showPoints, bool showSticks);
        void update_nodes_springs();
        void update_preview_node();
        void update_preview_spring(int activeNodeID);
    private:
        World& world;
        Cloth& cloth;
        
        std::vector<std::vector<int>>& graph;
        std::vector<Point>& points;
        std::vector<Stick>& sticks;
        std::vector<Quad>& quads;
        std::vector<Light>& lights;
        std::vector<Point>& nodes;
        Stick& spring;

        glm::vec3 lightPos;
        

        void update_points();
        void update_sticks();
        void update_quads();
        

        void render_point(Point point, float xn, float yn, float zn);
        void render_stick(Stick stick, float xnStart, float ynStart, float xnEnd, float ynEnd);
        void render_quads(Quad& quad, std::vector<float> vertices);
        void render_light(Light light);
        float normalize_xposition(float position, int cell_length, int scr_length);
        float normalize_yposition(float position, int cell_length, int scr_length);
        float normalize_cursor_position(float position);

};