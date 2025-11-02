#include "render.h"

Render::Render(World& world, std::vector<Point>& points, std::vector<Stick>& sticks):
        world(world),
        points(points),
        sticks(sticks) {

}

Render::~Render() {

}


void Render::update() {
    for (int i = 0; i < points.size(); i++) {
        render_point(points[i]);
        //std::cout << points[i].position.x << ", " << points[i].position.y << std::endl;
    }
    int k = 0;
    int i = 0;
    for(int y = 0; y < (world.pointLength*world.pointLength); y += world.pointLength) {
        for (int x = y; x < (y +world.pointLength); x++) {
            sticks[i].startPoint.x = points[y].position.x;
            sticks[i].startPoint.y = points[y].position.y;
            sticks[i].endPoint.x = points[x].position.x;
            sticks[i].endPoint.y = points[x].position.y;
            if(x != y +world.pointLength-1)
            {   
                i++; 
                sticks[i].startPoint.x = points[k].position.x;
                sticks[i].startPoint.y = points[k].position.y;
                sticks[i].endPoint.x = points[(k+world.pointLength)].position.x;
                sticks[i].endPoint.y = points[(k+world.pointLength)].position.y;

                k += 1;
            }
            i++;
        }


    }

    for (int i = 0; i < sticks.size(); i++) {
        render_stick(sticks[i]);
        //std::cout << points[i].position.x << ", " << points[i].position.y << std::endl;
    }

}

float Render::normalize_position(int position, int cell_length, int scr_length) {
    return -2.0f * (position + 0.5f) * (cell_length / (float)scr_length); 
}


void Render::render_point(Point point) {
    glUseProgram(point.mesh.shader);

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view  = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(point.position.x, point.position.y, 0.0f));
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

void Render::render_stick(Stick stick) {
    std::vector<float> vertices = {
        stick.startPoint.x,  stick.startPoint.y,   0.0f,
        stick.endPoint.x,  stick.endPoint.y,   0.0f, 
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