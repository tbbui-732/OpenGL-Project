#ifndef CAMERA_H
#define CAMERA_H

#include "shader.h"
#include "../glad/glad.h"
#include "../glfw/glfw3.h"

class Camera {
private:
    /////////////////////
    ///// VARIABLES /////
    /////////////////////
    // screen dimensions
    float scrWidth;
    float scrHeight;

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

    // scene objects
    std::vector<glm::vec3> cubePositions; 

    /////////////////////
    ///// FUNCTIONS /////
    /////////////////////
    float randFloat(float min, float max) {
        // Source: user "lastchance" - https://cplusplus.com/forum/general/242186/
        return min + (max - min) * rand() / RAND_MAX;
    }

    void generateCubes(int nCubePositions) {
        std::pair<float, float> xRange = { -7.0f, 7.0f };
        std::pair<float, float> yRange = { -7.0f, 7.0f };
        std::pair<float, float> zRange = { -7.0f, 7.0f };
        cubePositions.reserve(nCubePositions);

        for (int i = 0; i < nCubePositions; i++) {
            glm::vec3 newPos(
                    randFloat(xRange.first, xRange.second), 
                    randFloat(yRange.first, yRange.second), 
                    randFloat(zRange.first, zRange.second)
                    );
            cubePositions.push_back(newPos);
        }
    }

public:
    Camera(float width, float height) {
        scrWidth = width;
        scrHeight = height;
        lastX = scrWidth / 2.0f;
        lastY = scrHeight / 2.0f;
    }
};

#endif
