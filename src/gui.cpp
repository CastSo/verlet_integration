#include "./gui.h"

Gui::Gui(World& world, Cloth& cloth, std::vector<Point>& points, 
            std::vector<Stick>& sticks, std::vector<Quad>& quads, std::vector<Light>& lights):
        world(world),
        cloth(cloth),
        points(points),
        sticks(sticks),
        quads(quads),
        lights(lights) {
        showPoints = true;
        showSticks = true;
        clearCloth = true;
}

Gui::~Gui(){

}

void Gui::update(){
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

bool Gui::get_show_points() {
    return showPoints;
}

bool Gui::get_show_sticks() {
    return showSticks;
}

bool Gui::get_clear_cloth() {
    return clearCloth;
}