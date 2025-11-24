#include "./gui.h"

Gui::Gui(GLFWwindow *window, World& world, Cloth& cloth, std::vector<Point>& points, 
            std::vector<Stick>& sticks, std::vector<Point>& balls):
        window(window),
        world(world),
        cloth(cloth),
        points(points),
        sticks(sticks),
        balls(balls){
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
    for (int i = 1; i < balls.size(); i++) {
        int size = balls[i].scale;
        bool collisionX = balls[i].position.x + size >= xmouse && 
                        xmouse + size >= balls[i].position.x;
        bool collisionY = balls[i].position.y + size >= ymouse && 
                        xmouse + size >= balls[i].position.y;
        bool mouseCollides = collisionX && collisionY;

        if(mouseCollides) {
            balls[i].mesh.color = collideColor;
            balls[i].isPinned = true;
            if(mouseDown) {
                balls[i].position.x = xmouse;
                balls[i].position.y = ymouse;
            }
        } else if (balls[i].mesh.color.x != 1.0f && balls[i].mesh.color.x != 1.0f && balls[i].mesh.color.x != 1.0f) {
            balls[i].mesh.color = {1.0f, 1.0f, 1.0f};
            balls[i].isPinned = false;
        }
    }

}