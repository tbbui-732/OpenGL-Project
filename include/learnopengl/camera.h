#ifndef CAMERA_H
#define CAMERA_H

#include "shader.h"
#include "../glad/glad.h"
#include "../glfw/glfw3.h"

enum CameraDirection {
    FORWARD,
    BACKWARD, 
    LEFT,
    RIGHT,
    UP,
    DOWN
};

class Camera {
private:
    void updateCameraAxes() {
        // Front
        glm::vec3 newCameraFront;
        newCameraFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        newCameraFront.y = sin(glm::radians(pitch));
        newCameraFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        cameraFront = glm::normalize(newCameraFront);

        // Right
        cameraRight = glm::normalize(glm::cross(cameraFront, worldUp));

        // Up
        cameraUp = glm::normalize(glm::cross(cameraFront, cameraRight));
    }

public:
    /////////////////////
    ///// VARIABLES /////
    /////////////////////
    // screen dimensions
    float scrWidth;
    float scrHeight;

    // camera axes
    glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 cameraRight = glm::normalize(glm::cross(cameraFront, worldUp));

    // camera settings
    float fov = 45.0f;

    // camera direction
    bool firstMouse = true;
    float yaw = 0.0f;
    float pitch = 0.0f;

    // frames
    float lastX;
    float lastY;
    Camera(float width, float height) {
        scrWidth = width;
        scrHeight = height;
        lastX = scrWidth / 2.0f;
        lastY = scrHeight / 2.0f;
    }

    /////////////////////
    ///// FUNCTIONS /////
    /////////////////////
    void processKeyboard(CameraDirection direction, float deltaTime) {
        const float cameraSpeed = 2.5f * deltaTime;

        switch (direction) {
            case FORWARD:
                cameraPos += cameraSpeed * cameraFront;
                break;
            case BACKWARD:
                cameraPos -= cameraSpeed * cameraFront;
                break;
            case LEFT: {
                glm::vec3 cameraRight = glm::normalize(glm::cross(cameraFront, cameraUp));
                cameraPos -= cameraSpeed * cameraRight;
                break;
            }
            case RIGHT: {
                glm::vec3 cameraRight = glm::normalize(glm::cross(cameraFront, cameraUp));
                cameraPos += cameraSpeed * cameraRight;
                break;
            }
            case UP:
                cameraPos += cameraSpeed * cameraUp;
                break;
            case DOWN:
                cameraPos -= cameraSpeed * cameraUp;
                break;
        }
    }
};

#endif
