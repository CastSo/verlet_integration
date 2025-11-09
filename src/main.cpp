#include "./config.h"
#include "./factory.h"
#include "./render.h"
#include "./particle.h"

#include "./components/components.h"

GLFWwindow* window;
Camera camera;

const unsigned int SCR_WIDTH = 1024;
const unsigned int SCR_HEIGHT = 768;

bool firstMouse = true;
float yaw   = -90.0f;	 
float pitch =  0.0f;
float lastX =  800.0f / 2.0;
float lastY =  600.0 / 2.0;
float fov   =  45.0f;

void mouse_callback(GLFWwindow* window, double xpos, double ypos);

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

void process_keys(GLFWwindow *window, Cloth& cloth,  Camera& camera, float deltaTime,
         Factory* factory, std::vector<Point>& points)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){   
        glfwSetWindowShouldClose(window, true);
   }
   if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        points.clear();
        factory->make_points();
   }

    int move = 6;

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        change_point_location(cloth, 0, move, points);
   }
   if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        change_point_location(cloth, 0, -move, points);
   }
   if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        change_point_location(cloth,-move, 0, points);
   }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        change_point_location(cloth, move, 0, points);
   }
   float cameraSpeed = static_cast<float>(2.0 * deltaTime);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.position += cameraSpeed * camera.forwards;
   }
   if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.position -= cameraSpeed * camera.forwards;
   }
   if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.position -= glm::normalize(glm::cross(camera.forwards, camera.up)*cameraSpeed);
   }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.position += glm::normalize(glm::cross(camera.forwards, camera.up)*cameraSpeed);
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
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);  

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    glfwSetCursorPosCallback(window, mouse_callback);
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

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    camera.forwards = glm::normalize(front);
    
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


    camera.position = glm::vec3(0.0f, 1.0f, 1.0f);
    camera.up = glm::vec3(0.0f, 1.0f, 0.0f);
    camera.forwards = glm::vec3(0.0f, 0.0f, -1.0f);


    world.scrHeight = SCR_HEIGHT;
    world.scrWidth = SCR_WIDTH;
    cloth.particleLen = 4;
    cloth.stickBaseLen = 6;
    cloth.clothPtWidth = 20;
    cloth.clothPtHeight = 10;



    Factory* factory = new Factory(world, cloth, points, sticks, quads,lights);
    Render* render = new Render(world, cloth, camera, points, sticks, quads,lights);
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

        process_keys(window, cloth, camera, deltaTime, factory, points);

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