#include "factory.h"

Factory::Factory(World& world, Cloth& cloth, std::vector<Point>& points, std::vector<Stick>& sticks, std::vector<Quad>& quads):
        world(world),
        cloth(cloth),
        points(points),
        sticks(sticks),
        quads(quads) {
        betweenDistance = 6;
}


Factory::~Factory() {

}

float Factory::normalize_position(float position, int particleLen, int scrLength) {

    return 2.0f * (position ) * (particleLen / (float)scrLength); 
}

void Factory::make_points() {
    Mesh pointMesh = make_point_instance();
    Point point;
    point.mesh = pointMesh;
    point.height = cloth.particleLen/ world.scrHeight;
    point.width = cloth.particleLen / world.scrWidth;
    point.mass = 10000;

    int xoffset = -60;
    int yoffset = -60;
    int i = 0;
    for(int y = 0; y < cloth.clothPtDimension; y++)
    {

        point.position.y = (y*cloth.stickBaseLen)+yoffset;
        point.constraint.y = (y*cloth.stickBaseLen)+yoffset;
        for(int x = 0; x < cloth.clothPtDimension; x++)
        {
            point.position.x = (x*cloth.stickBaseLen)+xoffset;
            point.constraint.x = (x*cloth.stickBaseLen)+xoffset;
            points.push_back(point);
            //std::cout << "i: " << points[i].position.x << ", " << points[i].position.y << ", " << points[i].position.z << std::endl;
            i++;

        }

    }

    //std::cout << points.size() << std::endl;

    points[cloth.clothPtDimension*cloth.clothPtDimension-1].height = 6.0f / world.scrHeight;
    points[cloth.clothPtDimension*cloth.clothPtDimension-1].width = 6.0f / world.scrWidth;
    points[(cloth.clothPtDimension*cloth.clothPtDimension)-cloth.clothPtDimension].height = 6.0f / world.scrHeight;
    points[(cloth.clothPtDimension*cloth.clothPtDimension)-cloth.clothPtDimension].width = 6.0f / world.scrWidth;

    points[cloth.clothPtDimension*cloth.clothPtDimension-1].position.x += 5.0f;
    points[cloth.clothPtDimension*cloth.clothPtDimension-1].position.y += 5.0f;
    points[cloth.clothPtDimension*cloth.clothPtDimension-1].isPinned = true;
    points[(cloth.clothPtDimension*cloth.clothPtDimension)-cloth.clothPtDimension].position.x += 5.0f;
    points[(cloth.clothPtDimension*cloth.clothPtDimension)-cloth.clothPtDimension].position.y += 5.0f;
    points[(cloth.clothPtDimension*cloth.clothPtDimension)-cloth.clothPtDimension].isPinned = true;

    cloth.leftPin = cloth.clothPtDimension*cloth.clothPtDimension-1;
    cloth.rightPin = (cloth.clothPtDimension*cloth.clothPtDimension)-cloth.clothPtDimension;

}

void Factory::make_sticks() {
    Stick stick;
    int k = 0;
    for(int y = 0; y < points.size(); y++) { 

        stick.mesh = make_stick_instance();
        sticks.push_back(stick);

        
    }

}

void Factory::make_quads() {

    int quadCount = (cloth.clothPtDimension)*(cloth.clothPtDimension)-cloth.clothPtDimension;

    for (int i = 0; i < quadCount; i += cloth.clothPtDimension) {
        std::vector<float> vertices;
        for (int j = i; j < i+cloth.clothPtDimension-1; j++)
        {   
            
            vertices.push_back(normalize_position(points[j+cloth.clothPtDimension].position.x, cloth.particleLen, world.scrWidth));
            vertices.push_back(normalize_position(points[j+cloth.clothPtDimension].position.y, cloth.particleLen, world.scrHeight));
            vertices.push_back(0.0f);

            vertices.push_back(normalize_position(points[j].position.x, cloth.particleLen, world.scrWidth));
            vertices.push_back(normalize_position(points[j].position.y, cloth.particleLen, world.scrHeight));
            vertices.push_back(0.0f);

            vertices.push_back(normalize_position(points[j+cloth.clothPtDimension+1].position.x, cloth.particleLen, world.scrWidth));
            vertices.push_back(normalize_position(points[j+cloth.clothPtDimension+1].position.y, cloth.particleLen, world.scrHeight));
            vertices.push_back(0.0f);

            vertices.push_back(normalize_position(points[j+1].position.x, cloth.particleLen, world.scrWidth));
            vertices.push_back(normalize_position(points[j+1].position.y, cloth.particleLen, world.scrHeight));
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
    unsigned int VBO, VAO, EBO;
    



    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

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