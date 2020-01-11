#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <tools/filesystem.h>
#include <tools/my_shader_loader.h>
#include <model/camera.h>
#include <model/model.h>
#include <cmath>

#include <iostream>
#include <string>
#include <assimp/scene.h>
#include <sstream>
#include <tools/syntax_sugar.h>

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_MULTISAMPLE);


    // Model ourModel(FileSystem::getPath("resources/objects/lj/WhaleAnim03.fbx"));
    // Model ourModel(FileSystem::getPath("resources/objects/mg/source/MANTA2.fbx"));
    // Model ourModel(FileSystem::getPath("resources/ignore/qw/Quarantine722Wraith.stl"));
    // Model ourModel(FileSystem::getPath("resources/ignore/sd/SheHulk_Decimated.obj"));
    // Model ourModel(FileSystem::getPath("resources/objects/sz/sz.fbx"));
    // Model ourModel(FileSystem::getPath("resources/objects/dd/gedou.fbx"));
    // Model ourModel(FileSystem::getPath("resources/ignore/Nv_JianKe/NnJianKe_Delete Light UV_Mod.fbx"));
    // Model ourModel(FileSystem::getPath("resources/ignore/r/robot.FBX"));
    Model ourModel(FileSystem::getPath("resources/objects/gd/sazabi_1.obj"));
    
    ShaderLoader ourShader("animation.vs", "model.fs");
    ourShader.use();
    ourShader.setBool("hasAnimation", ourModel.hasAnimation());

    glm::vec3 light = glm::vec3(1.0f, 1.0f, 1.0f);

    ourShader.use();
    ourShader.setVec3("light.ambient", glm::vec3(0.1f, 0.1f, 0.1f));
    ourShader.setVec3("light.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
    ourShader.setVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));
    ourShader.setVec3("light.position", glm::vec3(100.0f, 100.0f, 100.0f));

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glClearColor(0.7, 0.7f, 0.7f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ourShader.use();
        ourShader.setVec3("viewPos", camera.Position);

        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -1.5f, -0.0f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(0.005f, 0.005f, 0.005f));	// it's a bit too big for our scene, so scale it down
        ourShader.setMat4("model", model);
        ourShader.setMat3("nor_model", glm::mat3(glm::transpose(glm::inverse(model))));

        ourModel.runAnimator(ourShader);
        ourModel.Draw(ourShader);


        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);

}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

bool hold_press = false;

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (!hold_press)
        return;

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT)
    {
        if(action == GLFW_PRESS)
        {
            hold_press = true;
        }
        else 
        {
            hold_press = false;
            firstMouse = true;
        }
    }
}