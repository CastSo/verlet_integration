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
    update_springs(deltaTime);
}

bool Particle::is_collided(Point p1, Point p2) {

    bool collisionX = p1.position.x + p1.scale >= p2.position.x && 
                    p2.position.x + p2.scale >= p1.position.x;
    bool collisionY = p1.position.y + p1.scale >= p2.position.y && 
                    p2.position.y + p2.scale >= p1.position.y;
    return collisionX && collisionY;
}

bool Particle::is_collided(Point point, int xBoxStart, int xBoxLen, int yBoxStart, int yBoxLen){
    bool collisionX = point.position.x + point.scale >= xBoxStart &&
                    xBoxStart + xBoxLen >= point.position.x;
    bool collisionY = point.position.y + point.scale >= yBoxStart &&
                    yBoxStart + yBoxLen >= point.position.y;
    return collisionX && collisionY;
    
}

bool Particle::is_collided(int xBoxStartN, int xBoxLenN, int yBoxStartN, int yBoxLenN,
                         int xBoxStartM, int xBoxLenM, int yBoxStartM, int yBoxLenM){
    bool collisionX = xBoxStartN + xBoxLenN >= xBoxStartM &&
                    xBoxStartM + xBoxLenM >= xBoxStartN;
    bool collisionY = yBoxStartN + yBoxLenN >= yBoxStartM &&
                    yBoxStartM + yBoxLenM >= yBoxStartN;
    return collisionX && collisionY;
    
}


bool Particle::is_connected(int id1, int id2) {
    for(int i = 0; i < graph[id1].size(); i++) {
        if(graph[id1][i] == id2)
            return true;
    }
    return false;
}

bool Particle::is_connected_graph(int srcID, int findID) {
    //Does bfs to see if within same connected graph
    int n = graph.size();
    std::vector<bool> visited(n, false);
    std::vector<int> found;
                    // n2.position.y = n1.position.y;
                    // n1.prevPosition.y = n1.prevPosition.y;
    std::queue<int> queue;

    visited[srcID] = true;
    queue.push(srcID);

    while (!queue.empty()) {
        int curr = queue.front();
        queue.pop();

        for (int x : graph[curr]) {
            if(x == findID)
                return true;

            if (!visited[x]) {
                visited[x] = true;
                queue.push(x);
            }
        }
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
        nodes[i].position.y = process_verlet(deltaTime, nodes[i].position.y, nodes[i].prevPosition.y, nodes[i].force.y, nodes[i].mass, .9f);
        nodes[i].prevPosition.y = yTmpPosition;

        // float xTmpPosition = nodes[i].position.x;
        // nodes[i].position.x = process_verlet(deltaTime, nodes[i].position.x, nodes[i].prevPosition.x, nodes[i].force.x, nodes[i].mass, .9f);
        // nodes[i].prevPosition.x = xTmpPosition;
        
    }
    
    for (int i = 1; i < nodes.size(); i++) {
        Point& n1 = nodes[i];

        for (int j = 1; j < nodes.size(); j++) {
            //Nodes connected to same graph does not collide
            if(i == j || is_connected_graph(i,j)) {
                continue;
            }
            Point& n2 = nodes[j];

            bool isCollided = is_collided(n1, n2); 
            if(isCollided) {
                //stay clamp to screen 
                float xmin = n1.scale*2.0f;
                float xmax = world.scrWidth - n1.scale*2.0f;
                float ymin = n1.scale*2.0f;
                float ymax = world.scrHeight - n1.scale*2.0f;

                
                if(n1.position.y <= ymin)
                {                
                    n1.position.y = ymin;
                } 
                float yTmpPosition = n1.position.y;
                n1.position.y = process_verlet(deltaTime, n1.position.y, n1.prevPosition.y, -n1.force.y, n1.mass, .25f);
                n1.prevPosition.y = yTmpPosition;

                float xTmpPosition = n1.position.x;
                n1.position.x = process_verlet(deltaTime, n1.position.x, n1.prevPosition.x, n1.force.x, n1.mass, .9f);
                n1.prevPosition.x = xTmpPosition;
                
                if(n2.position.y <= ymin)
                { 
                   n2.position.y = ymin;
                }
                yTmpPosition = n2.position.y;
                n2.position.y = process_verlet(deltaTime, n2.position.y, n2.prevPosition.y, -n2.force.y, n2.mass, .75f);
                n2.prevPosition.y = yTmpPosition;
                
                xTmpPosition = n2.position.x;
                n2.position.x = process_verlet(deltaTime, n2.position.x, n2.prevPosition.x, n2.force.x, n2.mass, .9f);
                n2.prevPosition.x = xTmpPosition;
            }
        }
    }

    for (int i = 1; i < graph.size(); i++) {
        Point& n1 = nodes[i];

        clamp_particles(n1, n1.constraint.x, n1.constraint.y);
        for (int j = 0; j < graph[i].size(); j++) {
            int currNode = graph[i][j];

            Point& n2 = nodes[currNode];

            if(currNode < i)
            {    
                clamp_particles(n2, n2.constraint.x, n2.constraint.y);
                satisfy_constraints(n1, n2, (10*n1.scale));
            }    
        }
    }


}

void Particle::update_springs(float deltaTime) {
    for (int i = 1; i < graph.size(); i++) {
        if(nodes[i].isPinned)
            continue;
        Point& n1 = nodes[i];
        for (int j = 0; j < graph[i].size(); j++) {

            //Skips repeating nodes
            if(i > graph[i][j] || nodes[graph[i][j]].isPinned)
                continue;
            
            Point& n2 = nodes[graph[i][j]];
            
            //bool isCollided = is_collided(n1, n2, n1.scale, n1.scale, n2.scale, n2.scale); 
            float xBoxStartN;
            float xBoxLenN;
            float yBoxStartN;
            float yBoxLenN;

            
            //Uses the lowest position as the start
            if(n1.position.x < n2.position.x)
            {
                xBoxStartN = n1.position.x-n1.scale;
            } else {
                xBoxStartN = n2.position.x-n2.scale;
            }
            
            
            if(n1.position.y < n2.position.y)
            {
                yBoxStartN = n1.position.y-n1.scale;
            } else {
                yBoxStartN = n2.position.y-n1.scale;
            }
            
            
            xBoxLenN = std::abs((n1.position.x-n1.scale) - (n2.position.x+n2.scale) );
            yBoxLenN = std::abs((n1.position.y-n1.scale)  - (n2.position.y+n2.scale) );
            float yBoxEndN = yBoxStartN + yBoxLenN;
            float xBoxEndN = xBoxStartN + xBoxLenN;
          
            //Checks if collides with other nodes
            for(int n = 1; n < nodes.size(); n++) {
                Point& currNode = nodes[n];
                //Ignores collision with start and end nodes, also collision with nodes that are part of a connected graph
                if(i == n || graph[i][j] == n || is_connected_graph(n, i) || is_connected_graph(n, graph[i][j]))
                    continue;

                bool isCollided = is_collided(currNode, xBoxStartN, xBoxLenN, yBoxStartN, yBoxLenN);
                
                if(isCollided) {
                    //std::cout << "colllided with stick" << std::endl;
                    float yNodeStart = currNode.position.y - currNode.scale;
                    float yNodeEnd = currNode.position.y + currNode.scale;
                    float xNodeStart = currNode.position.x - currNode.scale ;
                    float xNodeEnd = currNode.position.x + currNode.scale;

                    if(yBoxEndN > yNodeEnd)
                    {    
                        currNode.position.y = yBoxStartN;
                        currNode.prevPosition.y = process_verlet(deltaTime, currNode.position.y, currNode.prevPosition.y, currNode.force.y, currNode.mass, .9f);
                    }else {    
                        currNode.position.y = yBoxEndN;
                        currNode.prevPosition.y = process_verlet(deltaTime, currNode.position.y, currNode.prevPosition.y, currNode.force.y, currNode.mass, .9f);
                    }
                        
                    
                    if(xBoxEndN > xNodeEnd)
                    {    
                        currNode.position.x = xBoxStartN;
                        //currNode.prevPosition.x = process_verlet(deltaTime, currNode.position.x, currNode.prevPosition.x, currNode.force.x, currNode.mass, .9f);

                    }else {   
                        currNode.position.x = xBoxEndN;
                        //currNode.prevPosition.x = process_verlet(deltaTime, currNode.position.x, currNode.prevPosition.x, currNode.force.x, currNode.mass, .9f);
                    }

                }
            }


            for (int g = i+1; g < graph.size(); g++) {
                if( is_connected_graph(g, i) || is_connected_graph(g, graph[i][j]) || nodes[g].isPinned) 
                    continue;
                Point& m1 = nodes[g];
                for(int c = 0; c < graph[g].size(); c++) {
                    //Skips the values already compare with g and if the box equals the one that it's compared to
                     if(g > graph[g][c] ||  is_connected_graph(c, i) || is_connected_graph(g, graph[i][j]) || nodes[graph[g][c]].isPinned)
                        continue;

                    Point& m2 = nodes[graph[g][c]];
                    //bool isCollided = is_collided(n1, n2, n1.scale, n1.scale, n2.scale, n2.scale); 
                    float xBoxStartM;
                    float xBoxLenM;
                    float yBoxStartM;
                    float yBoxLenM;
                    
                    if(m1.position.x < m2.position.x)
                    {
                        xBoxStartM = m1.position.x-m1.scale;
                    } else {
                        xBoxStartM = m2.position.x-m2.scale;
                    }
                    
                    
                    if(m1.position.y < m2.position.y)
                    {
                        yBoxStartM = m1.position.y-m1.scale;
                    } else {
                        yBoxStartM = m2.position.y-m2.scale;
                    }
                    
                    
                    xBoxLenM = std::abs((m1.position.x-m1.scale) - (m2.position.x+m2.scale) );
                    yBoxLenM = std::abs((m1.position.y-m1.scale)  - (m2.position.y+m2.scale) );

                    float yBoxEndM = yBoxStartM + yBoxLenM;
                    float xBoxEndM = xBoxStartM + xBoxLenM;

                    bool isCollided = is_collided( xBoxStartN, xBoxLenN, yBoxStartN, yBoxLenN,
                                         xBoxStartM, xBoxLenM, yBoxStartM, yBoxLenM);
                    if(isCollided) {
                        //std::cout << "colllided with stick" << std::endl;

                        //Moves box1 according to positions between box1 and box2
                        if(yBoxEndN > yBoxEndM)
                        {    
                            //Pushes top most box up 
                            m1.position.y = yBoxStartN;
                            m1.prevPosition.y = process_verlet(deltaTime, m1.position.y, m1.prevPosition.y, m1.force.y, m1.mass, .9f);
                            m2.position.y = yBoxStartN;
                            m2.prevPosition.y = process_verlet(deltaTime, m2.position.y, m2.prevPosition.y, m2.force.y, m2.mass, .75f);
                        }else {    
                            n1.position.y = yBoxStartM;
                            n1.prevPosition.y = process_verlet(deltaTime, n1.position.y, n1.prevPosition.y, n1.force.y, n1.mass, .9f);
                            n2.position.y = yBoxStartM;
                            n2.prevPosition.y = process_verlet(deltaTime, n2.position.y, n2.prevPosition.y, n2.force.y, n2.mass, .75f);
                        }
                            
                        
                        if(xBoxEndN > xBoxEndM)
                        {    
                            m1.position.x = xBoxStartN;
                            m1.prevPosition.x = process_verlet(deltaTime, m1.position.x, m1.prevPosition.x, m1.force.x, m1.mass, .9f);
                            m2.position.x = xBoxStartN;
                            m2.prevPosition.x = process_verlet(deltaTime, m2.position.x, m2.prevPosition.x, m2.force.x, m2.mass, .75f);

                        }else {   
                            n1.position.x = xBoxStartM;
                            n1.prevPosition.x = process_verlet(deltaTime, n1.position.x, n1.prevPosition.x, n1.force.x, n1.mass, .9f);
                            n2.position.x = xBoxStartM;
                            n2.prevPosition.x = process_verlet(deltaTime, n2.position.x, n2.prevPosition.x, n2.force.x, n2.mass, .75f);
                        }


                        

                    }
                    
                }
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
        points[i].position.x = process_verlet(deltaTime, points[i].position.x, points[i].prevPosition.x, points[i].force.x, points[i].mass, .9f);
        points[i].prevPosition.x = xTmpPosition;

        float yTmpPosition = points[i].position.y;
        points[i].position.y = process_verlet(deltaTime, points[i].position.y, points[i].prevPosition.y, points[i].force.y, points[i].mass, .9f);
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
        point.prevPosition.y = process_verlet(deltaTime, point.position.y, point.prevPosition.y, point.force.y, point.mass, .9f);

    }

    if (point.position.y >= ymax) {
        point.position.y = ymax;
        point.prevPosition.y = process_verlet(deltaTime, point.position.y, point.prevPosition.y, point.force.y, point.mass, .9f);
        
    }

    if (point.position.x <= xmin) {
        point.position.x = xmin;
        point.prevPosition.x = process_verlet(deltaTime, point.position.x, point.prevPosition.x, point.force.x, point.mass, .9f);
    }

    if (point.position.x >= xmax) {
        point.position.x = xmax;
        point.prevPosition.x = process_verlet(deltaTime, point.position.x, point.prevPosition.x, point.force.x, point.mass, .9f);
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

float Particle::process_verlet(float deltaTime, float position, float prevPosition, float force, float mass, float damp) {
    float acceleration = force/mass;

    //(dt*dt) should be smaller than 1/k
    float newPosition =  (2 * position - prevPosition) * damp +  acceleration * (deltaTime * deltaTime);
    
    return newPosition;

}




