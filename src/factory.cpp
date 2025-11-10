#include "factory.h"

Factory::Factory(World& world, Cloth& cloth, std::vector<Point>& points, 
            std::vector<Stick>& sticks, std::vector<Quad>& quads, std::vector<Light>& lights):
        world(world),
        cloth(cloth),
        points(points),
        sticks(sticks),
        quads(quads),
        lights(lights) {

}


Factory::~Factory() {

}

float Factory::normalize_position(float position, int particleScale, int scrLength) {

    return 2.0f * (position ) * (particleScale / (float)scrLength); 
}

void Factory::make_points() {
    Mesh pointMesh = make_point_instance();
    Point point;
    point.mesh = pointMesh;
    point.scale = 8;
    point.height = point.scale/ world.scrHeight;
    point.width = point.scale / world.scrWidth;
    point.mass = 100000;

    int xoffset = 0;
    int yoffset = -20;
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

    //std::cout << points.size() << std::endl;

    points[cloth.clothPtHeight*cloth.clothPtWidth-1].height = 6.0f / world.scrHeight;
    points[cloth.clothPtHeight*cloth.clothPtWidth-1].width = 6.0f / world.scrWidth;
    points[(cloth.clothPtHeight*cloth.clothPtWidth)-cloth.clothPtWidth].height = 6.0f / world.scrHeight;
    points[(cloth.clothPtHeight*cloth.clothPtWidth)-cloth.clothPtWidth].width = 6.0f / world.scrWidth;

    points[cloth.clothPtHeight*cloth.clothPtWidth-1].position.x += 5.0f;
    points[cloth.clothPtHeight*cloth.clothPtWidth-1].position.y += 5.0f;
    points[cloth.clothPtHeight*cloth.clothPtWidth-1].isPinned = true;
    points[(cloth.clothPtHeight*cloth.clothPtWidth)-cloth.clothPtWidth].position.x += 5.0f;
    points[(cloth.clothPtHeight*cloth.clothPtWidth)-cloth.clothPtWidth].position.y += 5.0f;
    points[(cloth.clothPtHeight*cloth.clothPtWidth)-cloth.clothPtWidth].isPinned = true;

    cloth.leftPin = cloth.clothPtHeight*cloth.clothPtWidth-1;
    cloth.rightPin = (cloth.clothPtHeight*cloth.clothPtWidth)-cloth.clothPtWidth;

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
            
            vertices.push_back(normalize_position(points[j+cloth.clothPtWidth].position.x, cloth.particleScale, world.scrWidth));
            vertices.push_back(normalize_position(points[j+cloth.clothPtWidth].position.y, cloth.particleScale, world.scrHeight));
            vertices.push_back(0.0f);

            vertices.push_back(normalize_position(points[j].position.x, cloth.particleScale, world.scrWidth));
            vertices.push_back(normalize_position(points[j].position.y, cloth.particleScale, world.scrHeight));
            vertices.push_back(0.0f);

            vertices.push_back(normalize_position(points[j+cloth.clothPtWidth+1].position.x, cloth.particleScale, world.scrWidth));
            vertices.push_back(normalize_position(points[j+cloth.clothPtWidth+1].position.y, cloth.particleScale, world.scrHeight));
            vertices.push_back(0.0f);

            vertices.push_back(normalize_position(points[j+1].position.x, cloth.particleScale, world.scrWidth));
            vertices.push_back(normalize_position(points[j+1].position.y, cloth.particleScale, world.scrHeight));
            vertices.push_back(0.0f);



            // std::cout << "j " << j << ": " << points[j].position.x << ", " << points[j].position.y << ", " << points[j].position.z << std::endl;

            // std::cout << "j+row: "<< j+cloth.clothPtDimension << ": "  << points[j+cloth.clothPtDimension].position.x << ", " << points[j+cloth.clothPtDimension].position.y << ", " << points[j+cloth.clothPtDimension].position.z << std::endl;
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

   // std::cout << vertices.size() << std::endl;

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

