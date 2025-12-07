#pragma once
#include "./config.h"
#include "./shader.h"
#include "./components/components.h"

class Gui {
    public:
        Gui(GLFWwindow *window, World& world,  Cloth& cloth, std::vector<Point>& points, 
            std::vector<Stick>& sticks, std::vector<Point>& nodes);
        ~Gui();
        bool particleOn;
        static const char* nodeStateSelect[4]; 
        int nodeStateIndex;

        void update_imgui();
        void update_input(bool leftMouseFlag, bool rightMouseFlag, bool pinFlag);

        int find_mouse_collision();

        bool get_show_sticks();
        bool get_show_points();
        bool get_clear_cloth();
        int get_from_nodeID();
        int get_to_nodeID();
        void set_from_nodeID(int id);
        void set_to_nodeID(int id);
            

    private:
        bool showPoints;
        bool showSticks;
        bool clearCloth;
        
        

        int fromNodeID;
        int toNodeID;
        
        GLFWwindow *window;
        World& world;
        Cloth& cloth;
        std::vector<Point>& points;
        std::vector<Stick>& sticks;
        std::vector<Point>& nodes;

        void detect_mouse(bool leftMouseFlag, bool rightMouseFlag, bool pinFlag);
};