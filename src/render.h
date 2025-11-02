#pragma once
#include "./config.h"
#include "./shader.h"
#include "./components/components.h"

class Render {
    public:
        Render(World& world, std::vector<Point>& points, std::vector<Stick>& sticks); 
        ~Render();
        void update();

    private:
        World& world;
        std::vector<Point>& points;
        std::vector<Stick>& sticks;
        int pointCount;
        void render_point(Point point);
        void render_stick(Stick stick);
        float normalize_position(int position, int cell_length, int scr_length);

};