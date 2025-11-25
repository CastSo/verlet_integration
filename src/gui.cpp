#include "./gui.h"

Gui::Gui(GLFWwindow *window, World& world, Cloth& cloth, std::vector<Point>& points, 
            std::vector<Stick>& sticks, std::vector<Point>& nodes):
        window(window),
        world(world),
        cloth(cloth),
        points(points),
        sticks(sticks),
        nodes(nodes){
        showPoints = true;
        showSticks = true;
        clearCloth = true;
}

Gui::~Gui(){

}

void Gui::update_imgui(){
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Verlet");
    
    ImGui::Checkbox("Show points", &showPoints);
    ImGui::Checkbox("Show sticks", &showSticks);
    ImGui::Checkbox("Clear cloth", &clearCloth);

    ImGui::End();
        
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

}

void Gui::update_input(bool mouseDown) {
    detect_mouse(mouseDown);
}


bool Gui::get_show_points() {
    return showPoints;
}

bool Gui::get_show_sticks() {
    return showSticks;
}

bool Gui::get_clear_cloth() {
    return clearCloth;
}

void Gui::detect_mouse(bool mouseDown) {
    double xmouse, ymouse;
    glfwGetCursorPos(window, &xmouse, &ymouse);
    glm::vec3 collideColor = {0.0f, 1.0f, 0.0f};
    for (int i = 1; i < nodes.size(); i++) {
        int size = nodes[i].scale;
        bool collisionX = nodes[i].position.x + size >= xmouse && 
                        xmouse + size >= nodes[i].position.x;
        bool collisionY = nodes[i].position.y + size >= ymouse && 
                        xmouse + size >= nodes[i].position.y;
        bool mouseCollides = collisionX && collisionY;

        if(mouseCollides) {
            nodes[i].mesh.color = collideColor;
            if(mouseDown) {
                nodes[i].isPinned = true;
                nodes[i].position.x = xmouse;
                nodes[i].position.y = ymouse;

                // std::cout << nodes[i].position.x << ", " << nodes[i].position.y << 
                //     ", cursor" << xmouse << ", " << ymouse << std::endl;
            }
        } else if (nodes[i].mesh.color.x != 1.0f && nodes[i].mesh.color.x != 1.0f && nodes[i].mesh.color.x != 1.0f) {
            nodes[i].mesh.color = {1.0f, 1.0f, 1.0f};
            nodes[i].isPinned = false;
        }
    }

    for (int i = 1; i < points.size(); i++) {
        int size = points[i].scale * 4;
        bool collisionX = points[i].position.x + size >= xmouse && 
                        xmouse + size >= points[i].position.x;
        bool collisionY = points[i].position.y + size >= ymouse && 
                        xmouse + size >= points[i].position.y;
        bool mouseCollides = collisionX && collisionY;

        if(mouseCollides) {
            points[i].mesh.color = collideColor;
            if(mouseDown) {
                points[i].isPinned = true;
                points[i].position.x = xmouse;
                points[i].position.y = ymouse;

            }
        } else if (points[i].mesh.color.x != 1.0f && points[i].mesh.color.x != 1.0f && points[i].mesh.color.x != 1.0f) {
            points[i].mesh.color = {1.0f, 1.0f, 1.0f};
            points[i].isPinned = false;
        }
    }

}