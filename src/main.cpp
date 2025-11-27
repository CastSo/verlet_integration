#include "./config.h"
#include "./factory.h"
#include "./render.h"
#include "./particle.h"
#include "./gui.h"

#include "./components/components.h"

GLFWwindow* window;


const unsigned int SCR_WIDTH = 1024;
const unsigned int SCR_HEIGHT = 768;

float xclicked = 0;
float yclicked = 0;

bool leftMouseFlag = false;

bool canAddnode = false;

World world;

void mouse_callback(GLFWwindow* window,int button, int action, int mods);

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{

    if (width != world.scrWidth || height != world.scrHeight) {
        world.scrWidth = width;
        world.scrHeight = height;

        glViewport(0, 0, width, height); 
    }

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

void process_keys(GLFWwindow *window, Cloth& cloth,  float deltaTime,
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

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        change_point_location(cloth, 0, -move, points);
   }
   if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        change_point_location(cloth, 0, move, points);
   }
   if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        change_point_location(cloth,-move, 0, points);
   }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        change_point_location(cloth, move, 0, points);
   }

}

void mouse_callback(GLFWwindow* window, int button, int action, int mods){
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        
        leftMouseFlag = !leftMouseFlag;
    } 


    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
        canAddnode = !canAddnode;
    }
}


void setup_glfw() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

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
    glfwSetMouseButtonCallback(window, mouse_callback);
   

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        //return -1;
    }

}

void setup_imgui(){
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();


    ImGui_ImplGlfw_InitForOpenGL(window, true);         
    ImGui_ImplOpenGL3_Init("#version 400");
}


int main() {
    setup_glfw();
    setup_imgui();

    glEnable(GL_DEPTH_TEST);


    
    
    Cloth cloth;
    std::vector<Point> points;
    std::vector<Stick> sticks;
    std::vector<Quad> quads;
    std::vector<Light> lights;
    std::vector<Point> nodes;
    Stick spring;
    std::vector<std::vector<int>> graph;
    //Node 0 is initialized
    graph.push_back({});



    world.scrHeight = SCR_HEIGHT;
    world.scrWidth = SCR_WIDTH;
    cloth.particleScale = 2;
    cloth.stickBaseLen = 6;
    cloth.clothPtWidth = 20;
    cloth.clothPtHeight = 10;



    Factory* factory = new Factory(world, cloth, graph, points, sticks, quads, lights, nodes, spring);
    Render* render = new Render(world, cloth, graph, points, sticks, quads, lights, nodes, spring);
    Particle* particle = new Particle(world, cloth, graph, points, nodes);
    Gui* gui = new Gui(window, world, cloth, points, sticks, nodes);

    factory->make_cloth();
    nodes.push_back(factory->make_node(0.0f, 0.0f, 8, {1.0f, 0.0f, 0.0f}, 100000, {0.0f, 0.5f, 0.0f}));
    spring.mesh = factory->make_stick_instance();
    spring.mesh.color = {1.0f, 1.0f, 1.0f};
    //std::cout << quads.mesh.VAO << std::endl;

    float lastFrame = glfwGetTime();
    float currentFrame;
    float deltaTime = 0;
    while(!glfwWindowShouldClose(window))
    {
        double xCursorPos, yCursorPos;
        glfwGetCursorPos(window, &xCursorPos, &yCursorPos);

        nodes[0].position.x = (double)xCursorPos;
        nodes[0].position.y = world.scrHeight-(double)yCursorPos;

        if (leftMouseFlag && canAddnode){
            factory->make_node_spring((double)xCursorPos, (double)yCursorPos);
            leftMouseFlag = false;
            
        }

        currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        process_keys(window, cloth, deltaTime, factory, points);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        if(leftMouseFlag && !canAddnode) {
            gui->update_input(leftMouseFlag);
        }

        render->update_nodes_springs(canAddnode);
        
        particle->update(deltaTime);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    delete factory;
    delete render;
    delete particle;
    delete gui;
    glfwTerminate();
    return 0;

}