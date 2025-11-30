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
        fromNodeID = -1;
        toNodeID = -1;
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

void Gui::update_input(bool leftMouseFlag, bool rightMouseFlag, bool particleFlag) {
    detect_mouse(leftMouseFlag, rightMouseFlag, particleFlag);
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


int Gui::get_from_nodeID() {
    return fromNodeID;
}

int Gui::get_to_nodeID() {
    return toNodeID;
}

void Gui::set_from_nodeID(int id) {
    fromNodeID = id;
}


void Gui::set_to_nodeID(int id) {
    toNodeID = id;
}

int Gui::find_mouse_collision() {
    int nodeID = -1;

    double xmouse, ymouse;
    glfwGetCursorPos(window, &xmouse, &ymouse);

    for (int i = 1; i < nodes.size(); i++) {
        int size = 4*nodes[i].scale;
        bool collisionX = nodes[i].position.x + size >= xmouse && 
                        xmouse + size >= nodes[i].position.x;
        bool collisionY = nodes[i].position.y + size >= (world.scrHeight-ymouse) && 
                        (world.scrHeight-ymouse) + size  >= nodes[i].position.y;
        bool mouseCollides = collisionX && collisionY;

        if(mouseCollides){
            nodeID = i;
            break;
        }
    }

    return nodeID;
}

void Gui::detect_mouse(bool leftMouseFlag, bool rightMouseFlag, bool particleFlag) {
    double xmouse, ymouse;
    glfwGetCursorPos(window, &xmouse, &ymouse);
    glm::vec3 collideColor = {0.0f, 1.0f, 0.0f};
    for (int i = 1; i < nodes.size(); i++) {
        int size = nodes[i].scale;
        bool collisionX = nodes[i].position.x + size >= xmouse && 
                        xmouse + size >= nodes[i].position.x;
        bool collisionY = nodes[i].position.y + size >= (world.scrHeight-ymouse) && 
                        (world.scrHeight-ymouse) + size  >= nodes[i].position.y;
        bool mouseCollides = collisionX && collisionY;

        if(mouseCollides) {
            
            if(leftMouseFlag && !rightMouseFlag) {
                nodes[i].mesh.color = collideColor;
                fromNodeID = i;
            } else if (!leftMouseFlag &&  !rightMouseFlag){
                toNodeID = i;
                
            }


            nodes[i].mesh.color = collideColor;
            
            nodes[i].isPinned = true;
        } else if ((nodes[i].mesh.color.x != 1.0f && nodes[i].mesh.color.x != 1.0f && nodes[i].mesh.color.x != 1.0f) ) {
            nodes[i].isPinned = false; 
            if(fromNodeID != i)
                nodes[i].mesh.color = {1.0f, 1.0f, 1.0f};
        } 

        if(nodes[i].isPinned && !rightMouseFlag && particleFlag){
            nodes[i].isPinned = true;
            nodes[i].position.x = xmouse;
            nodes[i].position.y = world.scrHeight-ymouse;
        }
    }
    

   
}