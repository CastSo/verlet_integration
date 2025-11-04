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


void process_input(GLFWwindow *window, std::vector<Point>& points)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){   
        glfwSetWindowShouldClose(window, true);
   }

}


float normalize_position(int position, int cell_length, int scr_length) {

    return 2.0f * (position + 0.5f) * (cell_length / (float)scr_length); 
}

int main() {
    setup_glfw();

    glEnable(GL_DEPTH_TEST);

    
    World world;
    std::vector<Point> points;
    std::vector<Stick> sticks;


    world.scrHeight = SCR_HEIGHT;
    world.scrWidth = SCR_WIDTH;
    world.cellLength = 8;
    world.stickBaseLen = 6;



    Factory* factory = new Factory(world, points, sticks);
    Render* render = new Render(world, points, sticks);
    Particle* particle = new Particle(world, points, sticks);

    factory->make_points();
    factory->make_sticks();
    //std::cout << sticks.size() << std::endl;

    float lastFrame = glfwGetTime();
    float currentFrame;
    float deltaTime;
    while(!glfwWindowShouldClose(window))
    {
        currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame= currentFrame;

        process_input(window, points);

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