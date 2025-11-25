#pragma once
#include "./config.h"
#include "./shader.h"
#include "./components/components.h"

class Gui {
    public:
        Gui(GLFWwindow *window, World& world,  Cloth& cloth, std::vector<Point>& points, 
            std::vector<Stick>& sticks, std::vector<Point>& nodes);
        ~Gui();
        void update_imgui();
        void update_input(bool mouseDown);
        bool get_show_sticks();
        bool get_show_points();
        bool get_clear_cloth();
        

    private:
        bool showPoints;
        bool showSticks;
        bool clearCloth;
        
        GLFWwindow *window;
        World& world;
        Cloth& cloth;
        std::vector<Point>& points;
        std::vector<Stick>& sticks;
        std::vector<Point>& nodes;

        void detect_mouse(bool mouseDown);

};