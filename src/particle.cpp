#include "./particle.h"

Particle::Particle(World& world, Cloth& cloth, std::vector<Point>& points, std::vector<Stick>& sticks):
        world(world),
        cloth(cloth),
        points(points),
        sticks(sticks) {

}

Particle::~Particle() {

}

float Particle::normalize_position(float position, int particleLen, int scrLength) {

    return 2.0f * (position ) * (particleLen / (float)scrLength); 
}

void Particle::update(float deltaTime) {


    for(int i = 0; i < points.size(); i++) {
        if(points[i].isPinned){
            continue;
        }
        process_verlet(deltaTime, points[i]);
        clamp_particles(points[i]);
    }

    for (int y = 0; y < points.size()-cloth.clothPtWidth; y += cloth.clothPtWidth){
        
        for (int x = y; x < y+cloth.clothPtWidth; x++)
        {

            Point& p1 = points[x];
            Point&  p2 = points[x+cloth.clothPtWidth];
            satisfy_constraints(p1, p2);

            if(x < y + cloth.clothPtWidth-1){

                Point& p1 = points[x];
                Point& p2 = points[x+1];
                satisfy_constraints(p1, p2);
            }

        }
    }




}

void Particle::clamp_particles(Point& point) {
        float xmin = point.constraint.x-cloth.stickBaseLen;
        float xmax = point.constraint.x+cloth.stickBaseLen;
        float ymin = point.constraint.y-cloth.stickBaseLen;
        float ymax = point.constraint.y+cloth.stickBaseLen;

        if (point.position.y <= ymin) {
            point.position.y = ymin;
        
        }

        if (point.position.y >= ymax) {
            point.position.y = ymax;
            

        }

        if (point.position.x <= xmin) {
            point.position.x = xmin;
            

        }

        if (point.position.x >= xmax) {
            point.position.x = xmax;
            
        }

}

void Particle::satisfy_constraints(Point& p1, Point& p2) {

        
        float restLength = cloth.stickBaseLen;
        //float restLength = 0.1f;
        glm::vec3 delta = p2.position - p1.position; 
        float deltaLength = glm::length(delta);

        //Ensures particles not outside of box 
        if (deltaLength < restLength) 
        {    
            return;
        }

        float invmass1 = 1/p1.mass;
        float invmass2 = 1/p2.mass;

        float diff = (restLength - deltaLength) /  ((deltaLength)*(invmass1+invmass2));  

        if(!p1.isPinned)
        {    
            p1.position -= invmass1 * delta * diff;
        }
        if(!p2.isPinned)
        {
            p2.position += invmass2 * delta * diff;
        }

        // p1.position -= invmass1 * delta * diff;
        // p2.position += invmass2 * delta * diff;
}

void Particle::process_verlet(float deltaTime, Point& point) {
    

    glm::vec2 force = {0.0f, 0.5f};

    glm::vec2 acceleration = {force.x/point.mass, force.y/point.mass};

    glm::vec2 prevPos = {point.position.x, point.position.y};

    float xpos = (2*point.position.x) - point.prevPosition.x + acceleration.x * (deltaTime * deltaTime);
    
    float ypos = (2*point.position.y) - point.prevPosition.y + acceleration.y * (deltaTime * deltaTime);

    point.position.x = xpos;
    point.position.y = ypos;


    point.prevPosition.x = prevPos.x;
    point.prevPosition.y = prevPos.y;
    

   
}

