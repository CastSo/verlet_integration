#include "./config.h"
#include "./factory.h"
#include "./render.h"
#include "./particle.h"

#include "./components/components.h"

GLFWwindow* window;

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 640;


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void change_point_location(Cloth& cloth, int x, int y, std::vector<Point>& points) {
    points[cloth.leftPin].position.x += x;
    points[cloth.leftPin].position.y += y;

    points[cloth.rightPin].position.x += x;
    points[cloth.rightPin].position.y += y;
    for (int i = 0; i < points.size(); i++) {
        points[i].constraint.x += x;
        points[i].constraint.y += y;
    }
}

void process_keys(GLFWwindow *window, Cloth& cloth, Factory* factory, std::vector<Point>& points)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){   
        glfwSetWindowShouldClose(window, true);
   }
   if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        points.clear();
        factory->make_points();
   }

   int move = 6;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        change_point_location(cloth, 0, move, points);
   }
   if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        change_point_location(cloth, 0, -move, points);
   }
   if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        change_point_location(cloth,-move, 0, points);
   }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        change_point_location(cloth, move, 0, points);
   }



}




void setup_glfw() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
   // glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    // glfw window creation
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Verlet Integration", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        //return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    

     // tell GLFW to capture our mouse
    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    // glfwSetMouseButtonCallback(window, process_mouse);

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        //return -1;
    }

}


int main() {
    setup_glfw();

    glEnable(GL_DEPTH_TEST);

    
    World world;
    Cloth cloth;
    std::vector<Point> points;
    std::vector<Stick> sticks;
    std::vector<Quad> quads;
    std::vector<Light> lights;


    world.scrHeight = SCR_HEIGHT;
    world.scrWidth = SCR_WIDTH;
    cloth.particleLen = 4;
    cloth.stickBaseLen = 6;
    cloth.clothPtDimension = 10;



    Factory* factory = new Factory(world, cloth, points, sticks, quads,lights);
    Render* render = new Render(world, cloth, points, sticks, quads,lights);
    Particle* particle = new Particle(world, cloth, points, sticks);

    factory->make_points();
    factory->make_sticks();
    factory->make_quads();
    factory->make_lights();
    //std::cout << quads.mesh.VAO << std::endl;

    float lastFrame = glfwGetTime();
    float currentFrame;
    float deltaTime;
    while(!glfwWindowShouldClose(window))
    {
        currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame= currentFrame;

        process_keys(window, cloth, factory, points);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);




        render->update();
  
        particle->update(deltaTime);


        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    delete factory;
    delete render;
    delete particle;
    glfwTerminate();
    return 0;

}