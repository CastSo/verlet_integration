#include "render.h"

Render::Render(World& world, std::vector<Point>& points, std::vector<Stick>& sticks):
        world(world),
        points(points),
        sticks(sticks) {

}

Render::~Render() {

}

float Render::normalize_position(float position, int cellLength, int scrLength) {

    return 2.0f * (position ) * (cellLength / (float)scrLength); 
}

void Render::update() {
    for (int i = 0; i < points.size(); i++) {
        float xn = normalize_position(points[i].position.x, world.cellLength, world.scrWidth);
        float yn = normalize_position(points[i].position.y, world.cellLength, world.scrHeight);
        render_point(points[i], xn, yn);
        //std::cout << points[i].position.x << ", " << points[i].position.y << std::endl;
    }
    int k = 0;
    int i = 0;
    for(int y = 0; y < (world.stickBaseLen*world.stickBaseLen); y += world.stickBaseLen) {
        for (int x = y; x < (y +world.stickBaseLen)-1; x++) {
            //Horizontal sticks    
            float xnStartPt = normalize_position(points[x].position.x, world.cellLength, world.scrWidth);
            float ynStartPt = normalize_position(points[x].position.y, world.cellLength, world.scrHeight);
            float xnEndPt = normalize_position(points[x+1].position.x, world.cellLength, world.scrWidth);
            float ynEndPt = normalize_position(points[x+1].position.y, world.cellLength, world.scrHeight);
            render_stick(sticks[i], xnStartPt, ynStartPt, xnEndPt, ynEndPt);

            //Vertical sticks
            float xnStart = normalize_position(points[i].position.x, world.cellLength, world.scrWidth);
            float ynStart = normalize_position(points[i].position.y, world.cellLength, world.scrHeight);
            float xnEnd = normalize_position(points[i+world.stickBaseLen].position.x, world.cellLength, world.scrWidth);
            float ynEnd = normalize_position(points[i+world.stickBaseLen].position.y, world.cellLength, world.scrHeight);
            render_stick(sticks[i], xnStart, ynStart, xnEnd, ynEnd);
            k += 1;
            
            i++;
        }


    }

    for (int i = 0; i < sticks.size(); i++) {
        // render_stick(sticks[i]);
        //std::cout << points[i].position.x << ", " << points[i].position.y << std::endl;
    }

}


void Render::render_point(Point point, float xn, float yn) {
    glUseProgram(point.mesh.shader);

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view  = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(xn, yn, 0.0f));
    model = glm::scale(model, glm::vec3(point.width, point.height, 1.0f));
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, 0.0f));
    glm::mat4 proj  = glm::ortho( -1.0f, 1.0f,   -1.0f, 1.0f,   -1.0f, 1.0f ); // or pixels: glm::ortho(0, W, 0, H, -1, 1)

    unsigned int modelLoc = glGetUniformLocation(point.mesh.shader, "model");
    unsigned int viewLoc = glGetUniformLocation(point.mesh.shader, "view");
    unsigned int projLoc  = glGetUniformLocation(point.mesh.shader, "projection");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc,  1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc,  1, GL_FALSE, glm::value_ptr(proj));
    
    
    glBindVertexArray(point.mesh.VAO);
    
   
   
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    //glBindVertexArray(0);
}

void Render::render_stick(Stick stick, float xnStart, float ynStart, float xnEnd, float ynEnd) {
    std::vector<float> vertices = {
        xnStart,  ynStart,   0.0f,
        xnEnd,  ynEnd,   0.0f, 
    };

    glUseProgram(stick.mesh.shader);
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view  = glm::mat4(1.0f);
    glm::mat4 proj  = glm::ortho( -1.0f, 1.0f,   -1.0f, 1.0f,   -1.0f, 1.0f );

    unsigned int modelLoc = glGetUniformLocation(stick.mesh.shader, "model");
    unsigned int viewLoc = glGetUniformLocation(stick.mesh.shader, "view");
    unsigned int projLoc  = glGetUniformLocation(stick.mesh.shader, "projection");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc,  1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc,  1, GL_FALSE, glm::value_ptr(proj));    

    glBindBuffer(GL_ARRAY_BUFFER, stick.mesh.VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(float), vertices.data());

    glBindVertexArray(stick.mesh.VAO);

    glDrawArrays(GL_LINES, 0, 2);
    glBindVertexArray(0);

}