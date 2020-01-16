// dear imgui: standalone example application for GLFW + OpenGL 3, using programmable pipeline
// If you are new to dear imgui, see examples/README.txt and documentation at the top of imgui.cpp.
// (GLFW is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan graphics context creation, etc.)

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <context.h>
#include <render/opengl_render.h>
#include <tools/controller.h>

const size_t SCR_WIDTH = 1280;
const size_t SCR_HEIGHT = 720;

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

int main(int, char**)
{
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    const char* glsl_version = "#version 330";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);  
    

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "3N-322", NULL, NULL);
    if (window == NULL)
        return 1;

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    if (gladLoadGL() == 0)
    {
        fprintf(stderr, "Failed to initialize OpenGL loader!\n");
        return 1;
    }

    // glEnable(GL_DEPTH_TEST);
	// glEnable(GL_MULTISAMPLE);
    // glfwWindowHint(GLFW_SAMPLES, 4);
    // glEnable(GL_PROGRAM_POINT_SIZE);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
        
    SurroundCamera* camera = new SurroundCamera(3.0f, 0.0f, 0.0f, glm::vec3(0.0f, 0.0f, 0.0f));
    Context* context = new Context(camera, SCR_WIDTH, SCR_HEIGHT);
    OpenglRender* render = new OpenglRender(context);
    Controller* controller = new Controller(camera);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    ImVec4 clear_color = ImVec4(0.7f, 0.7f, 0.7f, 1.00f);

    bool show_demo_window = true;
    static ImGuiInputTextFlags flags = ImGuiInputTextFlags_AllowTabInput;

    bool sss = false;
    int view_type_radio = 0;
    int pixel_level = 1;
    int default_pixel_level = 1;

    int window_flags = ImGuiWindowFlags_NoTitleBar;
    window_flags |= ImGuiWindowFlags_NoScrollbar;
    window_flags |= ImGuiWindowFlags_NoResize;
    window_flags |= ImGuiWindowFlags_NoCollapse;
    window_flags |= ImGuiWindowFlags_NoBackground;

    while (!glfwWindowShouldClose(window))
    {

        
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // if (show_demo_window)
        // ImGui::ShowDemoWindow(&show_demo_window);

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
        ImGui::Begin("3N-322 CONSOLE", nullptr, window_flags);                          // Create a window called "Hello, world!" and append into it.
        // ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

        ImGui::RadioButton("3D Viewer", &view_type_radio, 0);
        ImGui::NewLine();
        ImGui::RadioButton("3N-322", &view_type_radio, 1);
        if(view_type_radio == 1)
        {
            ImGui::SliderInt("PL", &pixel_level, 1, FRAME_BUFFER_COUNT);
            render->is_3n = true;
            render->PL = pixel_level;
        }
        else
        {
            ImGui::SliderInt("PL", &default_pixel_level, 1, 1);
            render->is_3n = false;
            render->PL = default_pixel_level;
        }

        ImGui::NewLine();
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();

        render->render();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        controller->loop();
        controller->control();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    
    delete camera;
    delete context;
    delete render;

    return 0;
}
