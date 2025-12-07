#include "./gui.h"

const char* Gui::nodeStateSelect[4] = {
    "Spawn Node",
    "Connect Node",
    "Pin Node"
};

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
        particleOn = false;
        fromNodeID = -1;
        toNodeID = -1;

        nodeStateIndex = 0;
}

Gui::~Gui(){

}

void Gui::update_imgui(){
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Verlet");
    
    ImGui::Checkbox("Physics On", &particleOn);
    
    ImGui::Combo("Node state", &nodeStateIndex, nodeStateSelect, IM_ARRAYSIZE(nodeStateSelect));
    ImGui::End();
        
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

}

void Gui::update_input(bool leftMouseFlag, bool rightMouseFlag, bool pinFlag) {
    detect_mouse(leftMouseFlag, rightMouseFlag, pinFlag);
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
        int size = 2*nodes[i].scale;
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

void Gui::detect_mouse(bool leftMouseFlag, bool rightMouseFlag, bool pinFlag) {
    double xmouse, ymouse;
    glfwGetCursorPos(window, &xmouse, &ymouse);
    glm::vec3 collideColor = {0.0f, 1.0f, 0.0f};
    glm::vec3 particleOnColor = {1.0f, 1.0f, 1.0f};
    glm::vec3 particleOffColor = {0.7f, 0.2f, 0.1f};
    for (int i = 1; i < nodes.size(); i++) {
        int size = 2*nodes[i].scale;
        bool collisionX = nodes[i].position.x + size >= xmouse && 
                        xmouse + size >= nodes[i].position.x;
        bool collisionY = nodes[i].position.y + size >= (world.scrHeight-ymouse) && 
                        (world.scrHeight-ymouse) + size  >= nodes[i].position.y;
        bool mouseCollides = collisionX && collisionY;

        if(mouseCollides) {
            
            if(leftMouseFlag && nodeStateIndex == 1) {
                nodes[i].mesh.color = collideColor;
                fromNodeID = i;
                
            } else if (!leftMouseFlag && nodeStateSelect[1]){
                toNodeID = i;
               
            }

            if(leftMouseFlag && nodeStateIndex == 2) {
                
                nodes[i].isPinned = !nodes[i].isPinned;
            } 
            
           if(nodes[i].isPinned && !leftMouseFlag && nodeStateIndex == 2){
                nodes[i].isPinned = true;
                nodes[i].position.x = xmouse;
                nodes[i].position.y = world.scrHeight-ymouse;
            }
        } 
        if(nodes[i].isPinned) {
            nodes[i].mesh.color = {1.0f, 1.0f, 0.0f};
        } else if ( (nodes[i].mesh.color.x == 0.0f && nodes[i].mesh.color.y == 1.0f && nodes[i].mesh.color.z == 0.0f) ||
            nodes[i].mesh.color == particleOnColor || nodes[i].mesh.color == particleOffColor ) {
            nodes[i].isPinned = false; 
            if(fromNodeID != i )
            {   
                if(particleOn) 
                    nodes[i].mesh.color = particleOnColor;
                else
                    nodes[i].mesh.color = particleOffColor;
            }
        } 

        
    }
    

   
}