#include "./particle.h"

Particle::Particle(World& world, Cloth& cloth, std::vector<Point>& points, std::vector<Point>& balls):
        world(world),
        cloth(cloth),
        points(points),
        balls(balls) {

}

Particle::~Particle() {

}



void Particle::update(float deltaTime) {
    update_points(deltaTime);
    update_balls(deltaTime);
}

void Particle::update_balls(float deltaTime) {
    for (int i = 1; i < balls.size(); i++) {
        
        float yTmpPosition = balls[i].position.y;
        balls[i].position.y = process_verlet(deltaTime, balls[i].position.y, balls[i].prevPosition.y, 0.5f, balls[i].mass);
        balls[i].prevPosition.y = yTmpPosition;

        
        clamp_to_screen(balls[i]);
    }
}

void Particle::update_points(float deltaTime) {


    for(int i = 0; i < points.size(); i++) {
        if(points[i].isPinned){
            continue;
        }

        float xTmpPosition = points[i].position.x;
        points[i].position.x = process_verlet(deltaTime, points[i].position.x, points[i].prevPosition.x, 0.0f, points[i].mass);
        points[i].prevPosition.x = xTmpPosition;

        float yTmpPosition = points[i].position.y;
        points[i].position.y = process_verlet(deltaTime, points[i].position.y, points[i].prevPosition.y, 0.5f, points[i].mass);
        points[i].prevPosition.y = yTmpPosition;

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

void Particle::clamp_to_screen(Point& point) {
    float xmin = 10;
    float xmax = world.scrWidth-10;
    float ymin = 10;
    float ymax = world.scrHeight-10;

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
}

float Particle::process_verlet(float deltaTime, float position, float prevPosition, float force, float mass) {
    

    float acceleration = force/mass;

    float newPosition = (2*position) - prevPosition + acceleration * (deltaTime * deltaTime);
    

    return newPosition;

   

}

