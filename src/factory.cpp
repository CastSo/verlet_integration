#include "factory.h"

Factory::Factory(World& world, std::vector<Point>& points, std::vector<Stick>& sticks):
        world(world),
        points(points),
        sticks(sticks) {
        betweenDistance = 6;
}


Factory::~Factory() {

}

float Factory::normalize_position(float position, int cellLength, int scrLength) {

    return 2.0f * (position ) * (cellLength / (float)scrLength); 
}

void Factory::make_points() {
    Mesh pointMesh = make_point_instance();
    Point point;
    point.mesh = pointMesh;
    point.height = 8.0f / world.scrHeight;
    point.width = 8.0f / world.scrWidth;
    point.mass = 10000;

    for(int y = 0; y < world.stickBaseLen; y++)
    {

        point.position.y = y*betweenDistance;
        point.constraint.y = y*betweenDistance;
        for(int x = 0; x < world.stickBaseLen; x++)
        {

            point.position.x = x*betweenDistance;
            point.constraint.x = x*betweenDistance;
            points.push_back(point);

        }
        
    }


}

void Factory::make_sticks() {
    Stick stick;
    int k = 0;
    for(int y = 0; y < (world.stickBaseLen*world.stickBaseLen); y += world.stickBaseLen) { 
        //k = 0;
        for (int x = y; x < (y +world.stickBaseLen)-1; x++) {
            stick.mesh = make_stick_instance();
            sticks.push_back(stick);
            

            
        }

        
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