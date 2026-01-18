#include "factory.h"

Factory::Factory(World& world, Cloth& cloth, std::vector<std::vector<int>>& graph, std::vector<Point>& points, 
            std::vector<Stick>& sticks, std::vector<Quad>& quads, std::vector<Light>& lights,
            std::vector<Point>& nodes, Stick& spring):
        world(world),
        cloth(cloth),
        graph(graph),
        points(points),
        sticks(sticks),
        quads(quads),
        lights(lights),
        nodes(nodes),
        spring(spring) {

}


Factory::~Factory() {

}



void Factory::init_body() {
    make_points();
    make_sticks();
    make_quads();
    
}

float Factory::find_max_xpos(std::vector<int> array) {
    int maxID = 0;
    for(int i = 0; i < array.size(); i++) {
        int currID = array[i];
        if(nodes[currID].position.x > nodes[maxID].position.x){
            maxID = currID;
        }
    }

    return nodes[maxID].position.x;
}

float Factory::find_max_ypos(std::vector<int> array) {
    int maxID = 0;
    for(int i = 0; i < array.size(); i++) {
        int currID = array[i];
        if(nodes[currID].position.y > nodes[maxID].position.y){
            maxID = currID;
        }
    }

    return nodes[maxID].position.y;
}

void Factory::connect_node(int fromNode, int toNode) {
    graph[fromNode].push_back(toNode);
    graph[toNode].push_back(fromNode); 

    nodes[fromNode].constraint.x = std::abs(nodes[fromNode].position.x - find_max_xpos(graph[fromNode]));
    nodes[fromNode].constraint.y = std::abs(nodes[fromNode].position.y - find_max_ypos(graph[fromNode]));

    nodes[toNode].constraint.x = std::abs(nodes[toNode].position.x - find_max_xpos(graph[toNode]));
    nodes[toNode].constraint.y = std::abs(nodes[toNode].position.y - find_max_ypos(graph[toNode]));
}

void Factory::add_node(float xpos, float ypos) {
    float mass = 10000.0f;
    Point node = make_node(xpos, ypos, 16, {0.7f, 0.2f, 0.1f}, mass, {0.0f, 0.5f, 0.0f});
    node.isPinned = false;

    nodes.push_back(node);
    graph.push_back({});

}

Point Factory::make_node(float xpos, float ypos, int scale, glm::vec3 color, int mass, glm::vec3 force) {
    Mesh nodeMesh = make_point_instance();
    Point node;
    node.mesh = nodeMesh;
    node.scale = scale;
    node.height = node.scale/ world.scrHeight;
    node.width = node.scale / world.scrWidth;
    node.mesh.color = color;
    node.mass = mass;
    node.force = {force.x, force.y, force.z};

    node.position.x = xpos;
    node.position.y = ypos;
    node.constraint.x = 0.0f;
    node.constraint.y = 0.0f;

    node.isPinned = false;
    
    return node;
}

void Factory::add_box_body(float xpos, float ypos) {
    float mass = 10000.0f;
    Point node1 = make_node(xpos, ypos, 16, {1.0f, 1.0f, 1.0f}, mass, {0.0f, 1.0f, 0.0f});
    Point node2 = make_node(xpos, ypos+1, 16, {1.0f, 1.0f, 1.0f}, mass, {0.0f, 1.0f, 0.0f});
    Point node3 = make_node(xpos+1, ypos+1, 16, {1.0f, 1.0f, 1.0f}, mass, {0.0f, 1.0f, 0.0f});
    Point node4 = make_node(xpos+1, ypos, 16, {1.0f, 1.0f, 1.0f}, mass, {0.0f, 1.0f, 0.0f});

    nodes.push_back(node1);
    nodes.push_back(node2);
    nodes.push_back(node3);
    nodes.push_back(node4); 

    int id1 = nodes.size()-4;
    int id2 = nodes.size()-3;
    int id3 = nodes.size()-2;
    int id4 = nodes.size()-1;

    graph.push_back({});
    graph.push_back({});
    graph.push_back({});
    graph.push_back({});

    graph[id1].push_back(id2);
    graph[id1].push_back(id4);
    graph[id1].push_back(id3);

    graph[id2].push_back(id1);
    graph[id2].push_back(id3);
    graph[id2].push_back(id4);
    
    graph[id3].push_back(id1);
    graph[id3].push_back(id2);
    graph[id3].push_back(id4);

    graph[id4].push_back(id1);
    graph[id4].push_back(id3);
    graph[id4].push_back(id2);

}

void Factory::make_points() {
    Mesh pointMesh = make_point_instance();
    Point point;
    point.mesh = pointMesh;
    point.scale = 8;
    point.height = point.scale/ world.scrHeight;
    point.width = point.scale / world.scrWidth;
    point.mesh.color = {1.0f, 1.0f, 1.0f};
    point.mass = 100000;
    point.force = {5*point.mass, 5*point.mass, 0.0f};

    int xoffset = 0;
    int yoffset = 0;
    int i = 0;
    for(int y = 0; y < cloth.clothPtHeight; y++)
    {

        point.position.y = (y*cloth.stickBaseLen)+yoffset;
        point.constraint.y = (y*cloth.stickBaseLen)+yoffset;
        for(int x = 0; x < cloth.clothPtWidth; x++)
        {
            point.position.x = (x*cloth.stickBaseLen)+xoffset;
            point.constraint.x = (x*cloth.stickBaseLen)+xoffset;
            points.push_back(point);
        }

    }


    points[cloth.clothPtWidth-1].height = 8.0f / world.scrHeight;
    points[cloth.clothPtWidth-1].width = 8.0f / world.scrWidth;
    points[0].height = 8.0f / world.scrHeight;
    points[0].width = 8.0f / world.scrWidth;

    points[cloth.clothPtWidth-1].position.x += 10.0f;
    points[cloth.clothPtWidth-1].position.y -= 10.0f;
    points[cloth.clothPtWidth-1].isPinned = true;
    points[0].position.x -= 10.0f;
    points[0].position.y -= 10.0f;
    points[0].isPinned = true;

    cloth.leftPin = cloth.clothPtWidth-1;
    cloth.rightPin = 0;

}

void Factory::make_sticks() {
    Stick stick;
    int k = 0;
    for(int y = 0; y < points.size(); y++) { 

        stick.mesh = make_stick_instance();
        stick.mesh.color = {1.0f, 1.0f, 1.0f};
        sticks.push_back(stick);

        
    }

}

void Factory::make_lights() {
    Light light;
    light.mesh = make_light_instance();
    lights.push_back(light);
}

void Factory::make_quads() {

    int quadCount = ((cloth.clothPtHeight)*(cloth.clothPtWidth))-cloth.clothPtWidth;

    for (int i = 0; i < quadCount; i += cloth.clothPtWidth) {
        std::vector<float> vertices;
        for (int j = i; j < i+cloth.clothPtWidth-1; j++)
        {   
            
            vertices.push_back(0.0f);
            vertices.push_back(0.0f);
            vertices.push_back(0.0f);

            vertices.push_back(0.0f);
            vertices.push_back(0.0f);
            vertices.push_back(0.0f);

            vertices.push_back(0.0f);
            vertices.push_back(0.0f);
            vertices.push_back(0.0f);

            vertices.push_back(0.0f);
            vertices.push_back(0.0f);
            vertices.push_back(0.0f);



        }

        Quad quad;
        quad.mesh = make_quad_instance(vertices);
        quads.push_back(quad);

    }
 
}

Mesh Factory::make_point_instance() {
    unsigned int VBO, VAO, EBO;
    std::vector<float> vertices = {
         1.0f,  1.0f,  0.0f,  
         1.0f, -1.0f,  0.0f,   
        -1.0f, -1.0f,  0.0f,     
        -1.0f,  1.0f,  0.0f
    };

    

    std::vector<unsigned int> indices = {
        3, 1, 2,
        3, 0, 1
    };



    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);


    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    //position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    unsigned int shader = make_shader(
        "../src/view/shader.vert",
        "../src/view/shader.frag"
    );
    Mesh mesh;
    mesh.VAO = VAO;
    mesh.VBO = VBO;
    mesh.EBO = EBO;
    mesh.shader = shader;

    return mesh;

}

Mesh Factory::make_quad_instance(std::vector<float> vertices) {
    unsigned int VBO, VAO, normalVBO, EBO;
    std::vector<float> normals;

    int pointCount = vertices.size()-(vertices.size() % 9);
    for (int i = 0; i < pointCount; i += 9) {
        glm::vec3 v1 = {vertices[i], vertices[i+1], vertices[i+2]};
        glm::vec3 v2 = {vertices[i+3], vertices[i+4], vertices[i+5]};
        glm::vec3 v3 = {vertices[i+6], vertices[i+7], vertices[i+8]};

        glm::vec3 edge1 = v2 - v1;
        glm::vec3 edge2 = v3 - v1;

        glm::vec3 normal = glm::cross(edge1, edge2);
        normal = glm::normalize(normal);
        
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


    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &normalVBO);

    glBindVertexArray(VAO);

    //position attribute
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    

    //normal attribute
    glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float), normals.data(), GL_STATIC_DRAW);
   
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);    

    unsigned int shader = make_shader(
        "../src/view/light_shader.vert",
        "../src/view/light_shader.frag"
    );


    Mesh mesh;
    mesh.VAO = VAO;
    mesh.VBO = VBO;
    mesh.normalVBO = normalVBO;
    mesh.shader = shader;

    return mesh;
}

Mesh Factory::make_light_instance() {
    unsigned int VAO, VBO;
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glBindVertexArray(VAO);

    //position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    unsigned int shader = make_shader(
        "../src/view/shader.vert",
        "../src/view/shader.frag"
    );

    Mesh mesh;
    mesh.VAO = VAO;
    mesh.VBO = VBO;
    mesh.shader = shader;
    return mesh;
}


Mesh Factory::make_stick_instance() {
    unsigned int VAO, VBO, colorVBO;
    std::vector<float> vertices = {
        0.0f,  0.0f,   0.0f,
        0.0f,  0.0f,   0.0f, 
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &colorVBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    //position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);



    unsigned int shader = make_shader(
        "../src/view/shader.vert",
        "../src/view/shader.frag"
    );

    Mesh mesh;
    mesh.VAO = VAO;
    mesh.VBO = VBO;
    mesh.shader = shader;

    return mesh;
}

