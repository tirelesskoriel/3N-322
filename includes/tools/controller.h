#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <model/surround_camera.h>
class Controller
{
private:
    float lastXA = 0.0f;
    float lastYA = 0.0f;
    bool firstMouse = true;
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    SurroundCamera* Camera;
    
public:
    Controller(SurroundCamera* camera);
    ~Controller();

    void loop();
    void control();
    void keyboard();
};

#endif