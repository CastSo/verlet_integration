#include "render.h"

Render::Render(World& world, Cloth& cloth, std::vector<Point>& points, std::vector<Stick>& sticks, std::vector<Quad>& quads):
        world(world),
        cloth(cloth),
        points(points),
        sticks(sticks),
        quads(quads) {

}

Render::~Render() {

}

float Render::normalize_position(float position, int particleLen, int scrLength) {

    return 2.0f * (position ) * (particleLen / (float)scrLength); 
}

void Render::update() {
    for (int i = 0; i < points.size(); i++) {
        float xn = normalize_position(points[i].position.x, cloth.particleLen, world.scrWidth);
        float yn = normalize_position(points[i].position.y, cloth.particleLen, world.scrHeight);
        render_point(points[i], xn, yn);
        //std::cout << points[i].position.x << ", " << points[i].position.y << std::endl;
    }
    int k = 0;
    int i = 0;
    for(int y = 0; y < (cloth.clothPtDimension*cloth.clothPtDimension); y += cloth.clothPtDimension) {
        for (int x = y; x < (y +cloth.clothPtDimension)-1; x++) {
            //Horizontal sticks    
            float xnStartPt = normalize_position(points[x].position.x, cloth.particleLen, world.scrWidth);
            float ynStartPt = normalize_position(points[x].position.y, cloth.particleLen, world.scrHeight);
            float xnEndPt = normalize_position(points[x+1].position.x, cloth.particleLen, world.scrWidth);
            float ynEndPt = normalize_position(points[x+1].position.y, cloth.particleLen, world.scrHeight);
            render_stick(sticks[i], xnStartPt, ynStartPt, xnEndPt, ynEndPt);

            //Vertical sticks
            float xnStart = normalize_position(points[i].position.x, cloth.particleLen, world.scrWidth);
            float ynStart = normalize_position(points[i].position.y, cloth.particleLen, world.scrHeight);
            float xnEnd = normalize_position(points[i+cloth.clothPtDimension].position.x, cloth.particleLen, world.scrWidth);
            float ynEnd = normalize_position(points[i+cloth.clothPtDimension].position.y, cloth.particleLen, world.scrHeight);
            render_stick(sticks[i], xnStart, ynStart, xnEnd, ynEnd);
            k += 1;
            
            i++;
        }


    }

    int quadCount = (cloth.clothPtDimension)*(cloth.clothPtDimension)-cloth.clothPtDimension;
    int q = 0;

    for (int i = 0; i < quadCount; i += cloth.clothPtDimension) {
        std::vector<float> vertices;
        for (int j = i; j < i+cloth.clothPtDimension-1; j++)
        {   
            
            vertices.push_back(normalize_position(points[j+cloth.clothPtDimension].position.x, cloth.particleLen, world.scrWidth));
            vertices.push_back(normalize_position(points[j+cloth.clothPtDimension].position.y, cloth.particleLen, world.scrHeight));
            vertices.push_back(0.0f);

            vertices.push_back(normalize_position(points[j].position.x, cloth.particleLen, world.scrWidth));
            vertices.push_back(normalize_position(points[j].position.y, cloth.particleLen, world.scrHeight));
            vertices.push_back(0.0f);

            vertices.push_back(normalize_position(points[j+cloth.clothPtDimension+1].position.x, cloth.particleLen, world.scrWidth));
            vertices.push_back(normalize_position(points[j+cloth.clothPtDimension+1].position.y, cloth.particleLen, world.scrHeight));
            vertices.push_back(0.0f);

            vertices.push_back(normalize_position(points[j+1].position.x, cloth.particleLen, world.scrWidth));
            vertices.push_back(normalize_position(points[j+1].position.y, cloth.particleLen, world.scrHeight));
            vertices.push_back(0.0f);



            // std::cout << "j " << j << ": " << points[j].position.x << ", " << points[j].position.y << ", " << points[j].position.z << std::endl;

            // std::cout << "j+row: "<< j+cloth.clothPtDimension << ": "  << points[j+cloth.clothPtDimension].position.x << ", " << points[j+cloth.clothPtDimension].position.y << ", " << points[j+cloth.clothPtDimension].position.z << std::endl;
        }

        render_quads(quads[q], vertices);
        q++;

    }

    

}


void Render::render_point(Point point, float xn, float yn) {
    glUseProgram(point.mesh.shader);

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view  = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(xn, yn, 0.0f));
    model = glm::scale(model, glm::vec3(point.width, point.height, 1.0f));
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, 0.0f));
    glm::mat4 proj  = glm::ortho( -1.0f, 1.0f,   -1.0f, 1.0f,   -1.0f, 1.0f );

    unsigned int modelLoc = glGetUniformLocation(point.mesh.shader, "model");
    unsigned int viewLoc = glGetUniformLocation(point.mesh.shader, "view");
    unsigned int projLoc  = glGetUniformLocation(point.mesh.shader, "projection");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc,  1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc,  1, GL_FALSE, glm::value_ptr(proj));
    
    
    glBindVertexArray(point.mesh.VAO);
    
   
   
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Render::render_quads(Quad& quad, std::vector<float> vertices) {
    glUseProgram(quad.mesh.shader);
   

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view  = glm::mat4(1.0f);
    glm::mat4 proj  = glm::ortho( -1.0f, 1.0f,   -1.0f, 1.0f,   -1.0f, 1.0f );

    unsigned int modelLoc = glGetUniformLocation(quad.mesh.shader, "model");
    unsigned int viewLoc = glGetUniformLocation(quad.mesh.shader, "view");
    unsigned int projLoc  = glGetUniformLocation(quad.mesh.shader, "projection");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc,  1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc,  1, GL_FALSE, glm::value_ptr(proj));  

    glBindBuffer(GL_ARRAY_BUFFER, quad.mesh.VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(float), vertices.data());

    glBindVertexArray(quad.mesh.VAO);

    glDrawArrays(GL_TRIANGLE_STRIP, 0,
            4*(cloth.clothPtDimension-1));
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
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(float), vertices.data());

    glBindVertexArray(stick.mesh.VAO);

    glDrawArrays(GL_LINES, 0, 2);
    glBindVertexArray(0);

}