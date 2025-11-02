#include "./particle.h"

Particle::Particle(World& world, std::vector<Point>& points, std::vector<Stick>& sticks):
        world(world),
        points(points),
        sticks(sticks) {

}

Particle::~Particle() {

}


void Particle::update(float deltaTime) {

    for (int i = 0; i < points.size(); i++){
        process_verlet(deltaTime, points[i]);
        
    }

    //update_sticks();

}

void Particle::process_verlet(float deltaTime, Point& point) {
    glm::vec2 force = {0.0f, 0.5f};

    glm::vec2 acceleration = {force.x/point.mass, force.y/point.mass};

    glm::vec2 prevPos = {point.position.x, point.position.y};

    float xpos = (2.0f * point.position.x) - point.prevPosition.x + acceleration.x * (deltaTime * deltaTime);
    
    float ypos = (2.0f * point.position.y) - point.prevPosition.y + acceleration.y * (deltaTime * deltaTime);

    point.position.x = xpos;
    point.position.y = ypos;


    point.prevPosition.x = prevPos.x;
    point.prevPosition.y = prevPos.y;



    float restitution = 0.7f; 

     //x range: -50 to 50 
    //y range: -40 to 40
    if (point.position.y < -0.9f) {
        point.position.y = -0.9f;
        point.prevPosition.y = point.position.y + (point.position.y - point.prevPosition.y) * restitution;
    }

    if (point.position.y > 0.9f) {
        point.position.y = 0.9f;
        point.prevPosition.y = point.position.y + (point.position.y - point.prevPosition.y) * restitution;
    }

    if (point.position.x < -0.9f) {
        point.position.x = -0.9f;
        point.prevPosition.x = point.position.x + (point.position.x - point.prevPosition.x) * restitution;
    }

    if (point.position.x > 0.9f) {
        point.position.x = 0.9f;
        point.prevPosition.x = point.position.x + (point.position.x - point.prevPosition.x) * restitution;
    }
}

void Particle::update_sticks() {
    sticks[0].startPoint = {points[0].position.x, points[0].position.y, 0.0f};
    sticks[0].endPoint = {points[1].position.x, points[1].position.y, 0.0f};

    sticks[1].startPoint = {points[0].position.x, points[0].position.y, 0.0f};
    sticks[1].endPoint = {points[2].position.x, points[2].position.y, 0.0f};


    sticks[2].startPoint = {points[1].position.x, points[1].position.y, 0.0f};
    sticks[2].endPoint = {points[3].position.x, points[3].position.y, 0.0f};


    sticks[3].startPoint = {points[2].position.x, points[2].position.y, 0.0f};
    sticks[3].endPoint = {points[3].position.x, points[3].position.y, 0.0f};
}