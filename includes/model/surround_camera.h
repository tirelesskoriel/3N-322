#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <iostream>

#define PI 3.1415826
#define AP(A) (A / 360.0f * 3.1415826)

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

const float SPEED       =  1.0f;
const float ZOOM        =  45.0f;

class SurroundCamera
{
public:
    glm::vec3 Eye;
    glm::vec3 Center;
    glm::vec3 Up;

    float Zoom;

    float OffsetPosition;
    float Radius = 0.0f;
    float Pitch = 0.0f;
    float Yaw = 0.0f;

    SurroundCamera(float radius, float pitch_angle, float yaw_angle, glm::vec3 target) 
        : Up(glm::vec3(0.0f, 1.0f, 0.0f))
        , Zoom(ZOOM), Radius(radius), Center(target),Pitch(AP(pitch_angle)), Yaw(AP(yaw_angle))
    {
        RefreshEye(false);
    }

    glm::mat4 GetViewMatrix()
    {
        return glm::lookAt(Eye, Center, Up);
    }

    void ProcessKeyboard(Camera_Movement direction, float deltaTime)
    {
        
        float d = SPEED * deltaTime * PI;

        if (direction == FORWARD)
            Pitch += d;
        if (direction == BACKWARD)
            Pitch -= d;
        if (direction == RIGHT)
            Yaw += d;
        if (direction == LEFT)
            Yaw -= d;

        RefreshEye(direction == LEFT || direction == RIGHT);
    }

    void RefreshEye(bool l_or_right)
    {
        float temp = cos(Pitch) * Radius;
        Eye.x = temp * sin(Yaw);

        if (!l_or_right)
            Eye.y = sin(Pitch) * Radius;

        Eye.z = temp * cos(Yaw);
    }
};
#endif