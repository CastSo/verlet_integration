#pragma once 
#include "../config.h"

struct Mesh {

    unsigned int VAO, VBO, EBO;
    unsigned int colorVBO;
    unsigned int normalVBO;
    unsigned int shader;

};


struct Point {
    Mesh mesh;
    float width;
    float height;
    float scale;
    glm::vec3 force;
    glm::vec3 position;
    glm::vec3 constraint;
    glm::vec3 prevPosition;
    glm::vec3 acceleration;
    glm::vec3 velocity;
    float mass;
    float timeStep;
    bool isPinned;


};

struct Stick {
    Mesh mesh;
    // Point *pointStart;
    // Point *pointEnd;
    int ptStartIndex;
    int ptEndIndex;
    float length;
    glm::vec3 startPoint;
    glm::vec3 endPoint;
    float rotation;
};

struct Quad {
    Mesh mesh;

};

struct World {
    unsigned int scrWidth;
    unsigned int scrHeight;
};

struct Cloth {
    int particleScale;
    int stickBaseLen;
    int clothPtWidth;
    int clothPtHeight;
    int leftPin;
    int rightPin;
};

struct Camera {
    glm::vec3 position;
    glm::vec3 up;
    glm::vec3 forwards;
};

struct Light {
    Mesh mesh;
};