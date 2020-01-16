#include <tools/controller.h>
#include <GLFW/glfw3.h>
#include "imgui.h"

Controller::Controller(SurroundCamera* camera):Camera(camera)
{

}

Controller::~Controller()
{

}

void Controller::loop()
{
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
}

void Controller::control()
{
    keyboard();
}

void Controller::keyboard()
{
    Camera_Movement movement;
    for (int i = 0; i < IM_ARRAYSIZE(ImGui::GetIO().KeysDown); i++)
    {
        if (ImGui::IsKeyPressed(i))
        { 
            switch (i)
            {
            case 87:
                movement = FORWARD;
                break;
            case 83:
                movement = BACKWARD;
                break;
            case 65:
                movement = RIGHT;
                break;
            case 68:
                movement = LEFT;
                break;
            default:
                continue;
            }
            Camera->ProcessKeyboard(movement, deltaTime);
        }
    }
}