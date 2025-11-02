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

    for(int y = 0; y < world.pointLength; y++)
    {
        point.position.y = normalize_position(y*betweenDistance, world.cellLength, world.scrHeight);
        for(int x = 0; x < world.pointLength; x++)
        {
            point.position.x = normalize_position(x*betweenDistance, world.cellLength, world.scrWidth);
            points.push_back(point);
        }
        
        
    }



}

void Factory::make_sticks() {
    Stick stick;
    int k = 0;
    for(int y = 0; y < (world.pointLength*world.pointLength); y += world.pointLength) { 
        //k = 0;
        for (int x = y; x < (y +world.pointLength); x++) {
            stick.mesh = make_stick_instance(points[y].position.x, points[y].position.y, points[x].position.x, points[x].position.y);
            sticks.push_back(stick);
            
            if(x != y +world.pointLength-1)
            {    
                stick.mesh = make_stick_instance(points[k].position.x, points[k].position.y, 
                    points[(k+world.pointLength)].position.x, points[k+world.pointLength].position.y);
                sticks.push_back(stick);
                k += 1;
            }
            
            
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




Mesh Factory::make_stick_instance(float xStartPoint, float yStartPoint, float xEndPoint, float yEndPoint) {
    unsigned int VAO, VBO, colorVBO;
    std::vector<float> vertices = {
        xStartPoint,  yStartPoint,   0.0f,
        xEndPoint,  yEndPoint,   0.0f, 
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