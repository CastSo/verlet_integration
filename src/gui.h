#pragma once
#include "./config.h"
#include "./shader.h"
#include "./components/components.h"

class Gui {
    public:
        Gui(World& world,  Cloth& cloth, std::vector<Point>& points, 
            std::vector<Stick>& sticks, std::vector<Quad>& quads, std::vector<Light>& lights);
        ~Gui();
        void update();
        bool get_show_sticks();
        bool get_show_points();
        bool get_clear_cloth();

    private:
        bool showPoints;
        bool showSticks;
        bool clearCloth;

        World& world;
        Cloth& cloth;
        std::vector<Point>& points;
        std::vector<Stick>& sticks;
        std::vector<Quad>& quads;
        std::vector<Light>& lights;

};