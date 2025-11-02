#pragma once 
#include "../config.h"

struct Mesh {

    unsigned int VAO, VBO, EBO;
    unsigned int colorVBO;
    unsigned int shader;

};


struct Point {
    Mesh mesh;
    float width;
    float height;
    glm::vec3 position;
    glm::vec3 prevPosition;
    glm::vec3 acceleration;
    glm::vec3 velocity;
    float mass;
    float timeStep;


};

struct Stick {
    Mesh mesh;
    float length;
    glm::vec3 startPoint;
    glm::vec3 endPoint;
    float rotation;
};

struct World {
    unsigned int scrWidth;
    unsigned int scrHeight;
    int cellLength;
    int pointLength;

};