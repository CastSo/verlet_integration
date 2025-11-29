#include "./particle.h"

Particle::Particle(World& world, Cloth& cloth, std::vector<std::vector<int>>& graph, std::vector<Point>& points, std::vector<Point>& nodes):
        world(world),
        cloth(cloth),
        graph(graph),
        points(points),
        nodes(nodes) {

}

Particle::~Particle() {

}



void Particle::update(float deltaTime) {
    update_points(deltaTime);
    update_nodes(deltaTime);
}

bool Particle::check_collision(Point p1, Point p2) {
    int size = p1.scale;
    int xlen = p1.position.x-p2.position.x;
    int ylen = p1.position.y-p2.position.y;
    bool collisionX = p1.position.x + size >= p2.position.x && 
                    p2.position.x + size >= p1.position.x;
    bool collisionY = p1.position.y + size >= p2.position.y && 
                    p2.position.y + size >= p1.position.y;
    return collisionX && collisionY;
}

bool Particle::is_connected(int id1, int id2) {
    for(int i = 0; i < graph[id1].size(); i++) {
        if(graph[id1][i] == id2)
            return true;
    }
    return false;
}

void Particle::update_nodes(float deltaTime) {
    float xoffset = world.scrWidth/2;
    for (int i = 1; i < nodes.size(); i++) {
        if(nodes[i].isPinned)
            continue;
        clamp_to_screen(nodes[i], deltaTime);
        float yTmpPosition = nodes[i].position.y;
        nodes[i].position.y = process_verlet(deltaTime, nodes[i].position.y, nodes[i].prevPosition.y, nodes[i].force.y, nodes[i].mass);
        nodes[i].prevPosition.y = yTmpPosition;

        // float xTmpPosition = nodes[i].position.x;
        // nodes[i].position.x = process_verlet(deltaTime, nodes[i].position.x, nodes[i].prevPosition.x, nodes[i].force.x, nodes[i].mass);
        // nodes[i].prevPosition.x = xTmpPosition;
        
    }
    
    for (int i = 1; i < nodes.size(); i++) {
        Point& n1 = nodes[i];
        for (int j = i + 2; j < nodes.size(); j++) {
            if(i == j) {
                continue;
            }
            Point& n2 = nodes[j];

            bool isCollided = check_collision(n1, n2); 
            if(isCollided) {
                satisfy_constraints(n1, n2, (n1.scale*6));
            }
        }
    }

    for (int i = 1; i < graph.size(); i++) {
        Point& n1 = nodes[i];

        clamp_particles(n1, (6*n1.scale), (6*n1.scale));
        for (int j = 0; j < graph[i].size(); j++) {
            int currNode = graph[i][j];

            Point& n2 = nodes[currNode];

            if(currNode < i)
            {    
                clamp_particles(n2, (6*n2.scale), (6*n2.scale));
                satisfy_constraints(n1, n2, (6*n1.scale));
            }    
        }
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
    float xmin = point.scale*2.0f;
    float xmax = world.scrWidth - point.scale*2.0f;
    float ymin = point.scale*2.0f;
    float ymax = world.scrHeight - point.scale*2.0f;

    if (point.position.y <= ymin) {
        point.position.y = ymin;
        point.prevPosition.y = process_verlet(deltaTime, point.position.y, point.prevPosition.y, point.force.y, point.mass);

    }

    if (point.position.y >= ymax) {
        point.position.y = ymax;
        point.prevPosition.y = process_verlet(deltaTime, point.position.y, point.prevPosition.y, point.force.y, point.mass);
        
    }

    if (point.position.x <= xmin) {
        point.position.x = xmin;
        point.prevPosition.x = process_verlet(deltaTime, point.position.x, point.prevPosition.x, point.force.x, point.mass);
    }

    if (point.position.x >= xmax) {
        point.position.x = xmax;
        point.prevPosition.x = process_verlet(deltaTime, point.position.x, point.prevPosition.x, point.force.x, point.mass);
    }
}



void Particle::satisfy_constraints(Point& p1, Point& p2, float restLength) {

        glm::vec3 delta = p2.position - p1.position; 
        float deltaLength = glm::length(delta);
        float invmass1 = 1/p1.mass;
        float invmass2 = 1/p2.mass;
        float diff = (restLength - deltaLength) /  ((deltaLength)*(invmass1+invmass2));  

        float stiffness = 0.0f;

        if(!p1.isPinned)
        {   
            p1.position -=  (1.f - stiffness) *  invmass1 * delta * diff;
        }
        if(!p2.isPinned)
        {
            p2.position += (1.f - stiffness) * invmass2 * delta * diff;
        }
}

float Particle::process_verlet(float deltaTime, float position, float prevPosition, float force, float mass) {
    float acceleration = force/mass;
    float damp = .9f;
    //(dt*dt) should be smaller than 1/k
    float newPosition =  (2 * position - prevPosition) * damp +  acceleration * (deltaTime * deltaTime);
    
    return newPosition;

}




