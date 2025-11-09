#pragma once
#include "./config.h"
#include "./shader.h"
#include "./components/components.h"

class Render {
    public:
        Render(World& world,  Cloth& cloth, Camera& camera, std::vector<Point>& points, 
            std::vector<Stick>& sticks, std::vector<Quad>& quads, std::vector<Light>& lights); 
        ~Render();
        void update();

    private:
        World& world;
        Cloth& cloth;
        Camera& camera;
        std::vector<Point>& points;
        std::vector<Stick>& sticks;
        std::vector<Quad>& quads;
        std::vector<Light>& lights;

        glm::vec3 lightPos;
        

        void update_points();
        void update_sticks();
        void update_quads();

        void render_point(Point point, float xn, float yn);
        void render_stick(Stick stick, float xnStart, float ynStart, float xnEnd, float ynEnd);
        void render_quads(Quad& quad, std::vector<float> vertices);
        void render_light(Light light);
        float normalize_position(float position, int cell_length, int scr_length);

};