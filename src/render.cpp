#include "render.h"

Render::Render(World& world, Cloth& cloth, std::vector<Point>& points, 
            std::vector<Stick>& sticks, std::vector<Quad>& quads, std::vector<Light>& lights):
        world(world),
        cloth(cloth),
        points(points),
        sticks(sticks),
        quads(quads),
        lights(lights) {
        lightPos = glm::vec3(1.2f, 1.0f, -0.5f);
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
            //render_stick(sticks[i], xnStartPt, ynStartPt, xnEndPt, ynEndPt);

            //Vertical sticks
            float xnStart = normalize_position(points[i].position.x, cloth.particleLen, world.scrWidth);
            float ynStart = normalize_position(points[i].position.y, cloth.particleLen, world.scrHeight);
            float xnEnd = normalize_position(points[i+cloth.clothPtDimension].position.x, cloth.particleLen, world.scrWidth);
            float ynEnd = normalize_position(points[i+cloth.clothPtDimension].position.y, cloth.particleLen, world.scrHeight);
            //render_stick(sticks[i], xnStart, ynStart, xnEnd, ynEnd);
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


    render_light(lights[0]);

    

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

    std::vector<float> normals;

    for (int i = 0; i < vertices.size(); i += 9) {
        glm::vec3 v1 = {vertices[i], vertices[i+1], vertices[i+2]};
        glm::vec3 v2 = {vertices[i+3], vertices[i+4], vertices[i+5]};
        glm::vec3 v3 = {vertices[i+6], vertices[i+7], vertices[i+8]};

        glm::vec3 edge1 = v2 - v1;
        glm::vec3 edge2 = v3 - v1;

        glm::vec3 normal = glm::cross(edge1, edge2);
        
        v1 += normal;
        v2 += normal;
        v3 += normal;

        normals.push_back(v1.x);
        normals.push_back(v1.y);
        normals.push_back(v1.z);
        normals.push_back(v2.x);
        normals.push_back(v2.y);
        normals.push_back(v2.z);
        normals.push_back(v3.x);
        normals.push_back(v3.y);
        normals.push_back(v3.z);
    }


    glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3 objColor = glm::vec3(1.0f, 0.5f, 0.31f);

    glm::vec3 viewPos = glm::vec3 (1.0f, 1.0f, 1.5f);

    glm::mat4 model = glm::mat4(1.0f);

    glm::mat4 view  = glm::mat4(1.0f);
    glm::mat4 proj  = glm::ortho( -1.0f, 1.0f,   -1.0f, 1.0f,   -1.0f, 1.0f );


    unsigned int lightColorLoc = glGetUniformLocation(quad.mesh.shader, "lightColor");
    unsigned int objColorLoc = glGetUniformLocation(quad.mesh.shader, "objectColor");
    unsigned int lightPosLoc = glGetUniformLocation(quad.mesh.shader, "lightPos");
    unsigned int viewPosLoc = glGetUniformLocation(quad.mesh.shader, "viewPosLoc");

    unsigned int modelLoc = glGetUniformLocation(quad.mesh.shader, "model");
    unsigned int viewLoc = glGetUniformLocation(quad.mesh.shader, "view");
    unsigned int projLoc  = glGetUniformLocation(quad.mesh.shader, "projection");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc,  1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc,  1, GL_FALSE, glm::value_ptr(proj)); 
    
    glUniform3fv(lightColorLoc, 1,  glm::value_ptr(lightColor));
    glUniform3fv(objColorLoc, 1,  glm::value_ptr(objColor));
    glUniform3fv(lightPosLoc, 1, glm::value_ptr(lightPos));
    glUniform3fv(viewPosLoc, 1, glm::value_ptr(viewPos));

    glBindBuffer(GL_ARRAY_BUFFER, quad.mesh.VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(float), vertices.data());

    glBindBuffer(GL_ARRAY_BUFFER, quad.mesh.normalVBO);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, normals.size() * sizeof(float), normals.data());

    glBindVertexArray(quad.mesh.VAO);


    glDrawArrays(GL_TRIANGLE_STRIP, 0,
           (int)(vertices.size()/3));
}

void Render::render_light(Light light) {
    glUseProgram(light.mesh.shader);
    glm::mat4 model = glm::mat4(1.0f);
    
    model = glm::scale(model, glm::vec3(0.2f));
    model = glm::translate(model, lightPos);

    glm::mat4 view  = glm::mat4(1.0f);
    glm::mat4 proj  = glm::ortho( -1.0f, 1.0f,   -1.0f, 1.0f,   -1.0f, 1.0f );

    unsigned int modelLoc = glGetUniformLocation(light.mesh.shader, "model");
    unsigned int viewLoc = glGetUniformLocation(light.mesh.shader, "view");
    unsigned int projLoc  = glGetUniformLocation(light.mesh.shader, "projection");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc,  1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc,  1, GL_FALSE, glm::value_ptr(proj));  

    glBindVertexArray(light.mesh.VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
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