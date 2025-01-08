#include "../../../include/glad/glad.h"
#include "../../../include/glfw/glfw3.h"
#include "../../../include/learnopengl/shader.h"
#include "../../../include/learnopengl/camera.h"
#include "../../../include/stb_image/stb_image.h"

#include <iostream>
#include <filesystem>
#include <ostream>
#include <string>
#include <cstdlib>
#include <ctime>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void logError(std::string comment);
void mouse_callback(GLFWwindow* window, double xpos, double ypos); // xpos and ypos are the mouse's current position
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
float genRandFloat(float min, float max);

// settings
const unsigned int SCR_WIDTH    = 1600;
const unsigned int SCR_HEIGHT   = 1200;
const std::string shaderPath    = std::filesystem::current_path().string() + "/../src/2.lighting/13.basic_lighting/"; // NOTE: make sure to update this correctly!

// frames
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// camera
Camera* pCamera = new Camera(SCR_WIDTH, SCR_HEIGHT);

int main() {
    //////////////////////////////
    ///// PRE-INITIALIZATION /////
    //////////////////////////////
    // set random seed
    srand(static_cast<unsigned int>(time(0)));

    ////////////////
    ///// GLFW /////
    ////////////////
    // initialize and configure
    // --------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        logError("Failed to create GLFW window");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // hide mouse and capture its movement
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    ////////////////
    ///// GLAD /////
    ////////////////
    // load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        logError("Failed to initialize GLAD");
        return -1;
    }

    //////////////////
    ///// SHADER /////
    //////////////////
    // build and compile our shader program
    // ------------------------------------
    Shader lightingShader((shaderPath + "vertex.glsl").c_str(), (shaderPath + "fragment.glsl").c_str());
    Shader lightSourceShader((shaderPath + "light_vertex.glsl").c_str(), (shaderPath + "light_frag.glsl").c_str());

    ////////////////////
    ///// VERTICES /////
    ////////////////////
    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
    //  position              normals
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
    };

    // regular cube
    glm::vec3 regularCubePos(-1.0f, -1.0f, 5.0f);

    // light source
    glm::vec3 lightCubePos(2.0f, 1.0f, 10.0f);

    ///////////////////////
    ///// VBO AND VAO /////
    ///////////////////////
    // FIRST - initialize cube's VAO and VBO
    unsigned int VBO, cubeVAO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(cubeVAO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0); // position attribute
    glEnableVertexAttribArray(0);

    // SECOND - initialize light source cube's VAO and VBO
    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);

    // need to bind VBO in order to link with vertex attrib pointer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0); // position attribute
    glEnableVertexAttribArray(0);

    // unbind to prevent accidental state changes
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    /////////////////////////
    ///// DEPTH TESTING /////
    /////////////////////////
    glEnable(GL_DEPTH_TEST);

    ///////////////////////
    ///// RENDER LOOP /////
    ///////////////////////
    while (!glfwWindowShouldClose(window)) {
        // update delta time
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // set background
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // TODO: What does this do?

        // draw element
        // ------------
        lightingShader.use();
        lightingShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
        lightingShader.setVec3("objectColor", 1.0f, 0.5f, 0.3f);

        // RENDER CUBE 
        glm::mat4 projection = pCamera->getProjectionMatrix();
        lightingShader.setMat4("projection", projection);

        glm::mat4 view = pCamera->getViewMatrix();
        lightingShader.setMat4("view", view);

        glm::mat4 model(1.0f);
        model = glm::translate(model, regularCubePos);
        lightingShader.setMat4("model", model);

        glBindVertexArray(cubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // RENDER LIGHT SOURCE
        lightSourceShader.use();
        lightSourceShader.setMat4("projection", projection);
        lightSourceShader.setMat4("view", view);

        model = glm::mat4(1.0f);
        model = glm::translate(model, lightCubePos);
        lightSourceShader.setMat4("model", model);

        glBindVertexArray(lightVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // unbind vaos to prevent accidental state changes
        glBindVertexArray(0);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightVAO);
    glDeleteBuffers(1, &VBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        pCamera->processKeyboard(FORWARD, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        pCamera->processKeyboard(BACKWARD, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        pCamera->processKeyboard(LEFT, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        pCamera->processKeyboard(RIGHT, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        pCamera->processKeyboard(UP, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        pCamera->processKeyboard(DOWN, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

// log out red text to standard output when there's an error
void logError(std::string comment) {
    std::cout << "\033[1;31m" << comment << "\033[0m" << std::endl;
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) { // xpos and ypos are the mouse's current position
    pCamera->processMouseMovement(static_cast<float>(xposIn), static_cast<float>(yposIn), true);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    pCamera->processScrollMovement(yoffset, 0.25f);
}

float genRandFloat(float min, float max) {
    // Source: user "lastchance" - https://cplusplus.com/forum/general/242186/
    return min + (max - min) * rand() / RAND_MAX;
}
