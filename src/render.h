#pragma once
#include "./config.h"
#include "./shader.h"
#include "./components/components.h"

class Render {
    public:
        Render(World& world,  Cloth& cloth, std::vector<Point>& points, std::vector<Stick>& sticks, std::vector<Quad>& quads); 
        ~Render();
        void update();

    private:
        World& world;
        Cloth& cloth;
        std::vector<Point>& points;
        std::vector<Stick>& sticks;
        std::vector<Quad>& quads;


        int pointCount;
        void render_point(Point point, float xn, float yn);
        void render_stick(Stick stick, float xnStart, float ynStart, float xnEnd, float ynEnd);
        void render_quads(Quad& quad, std::vector<float> vertices);
        float normalize_position(float position, int cell_length, int scr_length);

};