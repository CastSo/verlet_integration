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

bool Particle::check_collision(Point p1, Point p2) {
    int size = p1.scale;
    bool collisionX = p1.position.x + size >= p2.position.x && 
                    p2.position.x + size >= p1.position.x;
    bool collisionY = p1.position.y + size >= p2.position.y && 
                    p2.position.y + size >= p1.position.y;
    return collisionX && collisionY;
}

void Particle::update_balls(float deltaTime) {
    float xoffset = world.scrWidth/2;
    for (int i = 1; i < balls.size(); i++) {
        
        float yTmpPosition = balls[i].position.y;
        balls[i].position.y = process_verlet(deltaTime, balls[i].position.y, balls[i].prevPosition.y, balls[i].force.y, balls[i].mass);
        balls[i].prevPosition.y = yTmpPosition;

        float xTmpPosition = balls[i].position.x;
        balls[i].position.x = process_verlet(deltaTime, balls[i].position.x, balls[i].prevPosition.x, balls[i].force.x, balls[i].mass);
        balls[i].prevPosition.x = xTmpPosition;

        
        clamp_to_screen(balls[i], deltaTime);
    }

    int ballCount = balls.size();
    for (int i = 1; i < ballCount; i++) {
        Point& b1 = balls[i];
        for (int j = i + 2; j < ballCount; j++) {
            if(i == j) {
                continue;
            }
            Point& b2 = balls[j];
            bool isCollided = check_collision(b1, b2); 
            if(isCollided) {
                satisfy_constraints(b1, b2, (b1.scale*4));
            }
        }
    }

    for (int i = 1; i < balls.size()-1; i += 2){
        Point& b1 = balls[i];
        Point& b2 = balls[i+1];

        
        clamp_particles(b1, (4*b1.scale), (4*b1.scale));
        clamp_particles(b2, (4*b2.scale), (4*b2.scale));
        satisfy_constraints(b1, b2, (4*b1.scale));
    }
}

void Particle::update_points(float deltaTime) {


    for(int i = 0; i < points.size(); i++) {
        if(points[i].isPinned){
            continue;
        }

        float xTmpPosition = points[i].position.x;
        points[i].position.x = process_verlet(deltaTime, points[i].position.x, points[i].prevPosition.x, points[i].force.x, points[i].mass);
        points[i].prevPosition.x = xTmpPosition;

        float yTmpPosition = points[i].position.y;
        points[i].position.y = process_verlet(deltaTime, points[i].position.y, points[i].prevPosition.y, points[i].force.y, points[i].mass);
        points[i].prevPosition.y = yTmpPosition;

        clamp_particles(points[i], cloth.stickBaseLen, cloth.stickBaseLen);

    }

    for (int y = 0; y < points.size()-cloth.clothPtWidth; y += cloth.clothPtWidth){
        
        for (int x = y; x < y+cloth.clothPtWidth; x++)
        {
            //Point from p1's top
            Point& p1 = points[x];
            Point&  p2 = points[x+cloth.clothPtWidth];
            satisfy_constraints(p1, p2, cloth.stickBaseLen);

            if(x < y + cloth.clothPtWidth-1){
                Point& p1 = points[x];
                //Point from p1's right
                Point& p2 = points[x+1];
                satisfy_constraints(p1, p2, cloth.stickBaseLen);
            }

        }
    }
}

void Particle::clamp_particles(Point& point, float xBaseLen, float yBaseLen) {
        float xmin = point.position.x-xBaseLen;
        float xmax = point.position.x+xBaseLen;
        float ymin = point.position.y-yBaseLen;
        float ymax = point.position.y+yBaseLen;

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

void Particle::clamp_to_screen(Point& point, float deltaTime) {
    float xmin = point.scale;
    float xmax = world.scrWidth - point.scale;
    float ymin = point.scale;
    float ymax = world.scrHeight - point.scale;

    if (point.position.y <= ymin) {
        point.position.y = ymin;
        point.position.y = process_verlet(deltaTime, point.position.y, point.prevPosition.y, -point.force.y, point.mass);
        
    }

    if (point.position.y >= ymax) {
        point.position.y = ymax;
        point.position.y = process_verlet(deltaTime, point.position.y, point.prevPosition.y, -point.force.y, point.mass);
        
    }

    if (point.position.x <= xmin) {
        point.position.x = xmin;
        point.position.x = process_verlet(deltaTime, point.position.x, point.prevPosition.x, -point.force.x, point.mass);
    }

    if (point.position.x >= xmax) {
        point.position.x = xmax;
        point.position.x = process_verlet(deltaTime, point.position.x, point.prevPosition.x, -point.force.x, point.mass);
    }
}



void Particle::satisfy_constraints(Point& p1, Point& p2, float restLength) {

        glm::vec3 delta = p2.position - p1.position; 
       // delta *= restLength*restLength / (delta * delta + restLength * restLength)-0.5f;
        float deltaLength = glm::length(delta);


        float invmass1 = 1/p1.mass;
        float invmass2 = 1/p2.mass;

        float diff = (restLength - deltaLength) /  ((deltaLength)*(invmass1+invmass2));  
        float k = 1.0f;
        if(!p1.isPinned)
        {    
            p1.position -= k * invmass1 * delta * diff;
        }
        if(!p2.isPinned)
        {
            p2.position += k * invmass2 * delta * diff;
        }
}

float Particle::process_verlet(float deltaTime, float position, float prevPosition, float force, float mass) {
    float acceleration = force/mass;

    float newPosition = (2*position) - prevPosition + acceleration * (deltaTime * deltaTime);
    
    return newPosition;

}

void Particle::process_collision(Point p1, Point p2, float deltaTime) {
    glm::vec3 delta = p2.position - p1.position;
    float sumRadii = (p1.scale/2) + (p2.scale/2);
    float distance = glm::length(delta);
    int K = 1;
    if (distance < sumRadii) {
        glm::vec3 force = K * (sumRadii - distance) * (delta / distance);
        p1.velocity -= K * deltaTime / p1.mass;
        p2.velocity += K * deltaTime / p2.mass;
    }

    if(!p1.isPinned)
    {    
        p1.position += p1.velocity * deltaTime;
    }
    if(!p2.isPinned)
    {
        p2.position += p1.velocity * deltaTime;
    }
}

void Particle::process_force(Point p1, Point p2, float deltaTime){
    glm::vec3 delta = p2.position - p1.position;
    float distance = glm::length(delta);
    glm::vec3 direction = delta / distance;
    
    float gravity = 6.67*pow(10, -11);

    glm::vec3 force = direction * gravity * p1.mass * p2.mass;
    force /= pow(distance, 2.0f);

    p1.velocity += force * deltaTime / p1.mass;
    p2.velocity -= force * deltaTime / p2.mass;


}
