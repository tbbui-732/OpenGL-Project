#ifndef CAMERA_H
#define CAMERA_H

#include "shader.h"
#include "../glad/glad.h"
#include "../glfw/glfw3.h"

class Camera {
private:
    // camera axes
    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

    // frames
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    float lastX;
    float lastY;

    // camera settings
    float fov = 45.0f;

    // camera direction
    bool firstMouse = true;
    float yaw = 0.0f;
    float pitch = 0.0f;

public:
    Camera(float width, float height) {
        lastX = width / 2.0f;
        lastY = height / 2.0f;
    }
};

#endif
