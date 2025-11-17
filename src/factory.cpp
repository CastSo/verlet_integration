#include "factory.h"

Factory::Factory(World& world, Cloth& cloth, std::vector<Point>& points, 
            std::vector<Stick>& sticks, std::vector<Quad>& quads, std::vector<Light>& lights,
            std::vector<Point>& balls):
        world(world),
        cloth(cloth),
        points(points),
        sticks(sticks),
        quads(quads),
        lights(lights),
        balls(balls) {

}


Factory::~Factory() {

}



void Factory::make_cloth() {
    make_points();
    make_sticks();
    make_quads();
    
}

void Factory::make_ball(float xpos, float ypos) {


    Mesh pointMesh = make_point_instance();
    Point point;
    point.mesh = pointMesh;
    point.scale = 12;
    point.height = point.scale/ world.scrHeight;
    point.width = point.scale / world.scrWidth;
    point.mass = 100000;
    point.force = {point.mass, point.mass, 0.0f};

    point.position.x = xpos;
    point.position.y = ypos;

    balls.push_back(point);
}

void Factory::make_points() {
    Mesh pointMesh = make_point_instance();
    Point point;
    point.mesh = pointMesh;
    point.scale = 8;
    point.height = point.scale/ world.scrHeight;
    point.width = point.scale / world.scrWidth;
    point.mass = 100000;
    point.force = {0.0f, 0.5f, 0.0f};

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
         1.0f,  1.0f,  0.0f,  1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  0.0f,  1.0f,  1.0f,  1.0f,  
        -1.0f, -1.0f,  0.0f,  1.0f,  1.0f,  1.0f,   
        -1.0f,  1.0f,  0.0f,  1.0f,  1.0f,  1.0f   
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float))); 
    glEnableVertexAttribArray(1);

    unsigned int shader = make_shader(
        "../src/view/shader.vert",
        "../src/view/shader.frag"
    );
    Mesh mesh;
    mesh.VAO = VAO;
    mesh.VBO = VBO;
    mesh.EBO = EBO;
    mesh.shader = shader;
    //mesh.transformVBO = make_transform_buffer();
    // mesh.colorVBO = make_color_buffer();

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

    std::vector<float> colors = {
        1.0f,  1.0f,   1.0f,
        1.0f,  1.0f,   1.0f
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

    //color attribute
    glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
    glEnableVertexAttribArray(1);

    glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(float), colors.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(0 * sizeof(float))); 


    unsigned int shader = make_shader(
        "../src/view/shader.vert",
        "../src/view/shader.frag"
    );

    Mesh mesh;
    mesh.VAO = VAO;
    mesh.VBO = VBO;
    mesh.colorVBO = colorVBO;
    mesh.shader = shader;

    return mesh;
}

