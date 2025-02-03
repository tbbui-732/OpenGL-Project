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
unsigned int loadTexture(std::string texPath);

// settings
const unsigned int SCR_WIDTH    = 1200;
const unsigned int SCR_HEIGHT   = 800;
const std::string shaderPath    = std::filesystem::current_path().string() + "/../src/2.lighting/16.light_casters/"; // NOTE: make sure to update this correctly!
const std::string texturePath   = std::filesystem::current_path().string() + "/../resources/textures/"; // NOTE: make sure to update this correctly!

// frames
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// camera
Camera* camera = new Camera(SCR_WIDTH, SCR_HEIGHT);

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
glfwInit();
glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

// window creation
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
if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    logError("Failed to initialize GLAD");
    return -1;
}


//////////////////
///// OPENGL /////
//////////////////
glEnable(GL_DEPTH_TEST);


//////////////////
///// SHADER /////
//////////////////
// build and compile our shader program
Shader objectShader( (shaderPath + "object.vert").c_str(), (shaderPath + "object.frag").c_str() );
Shader lampShader( (shaderPath + "lamp.vert").c_str(), (shaderPath + "lamp.frag").c_str() );

////////////////////
///// VERTICES /////
////////////////////
// set up vertex data
float vertices[] = {
    // positions          // normals           // texture coords
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
};

// generate a bunch of random cube positions
int nCubes = 15;
std::vector<glm::vec3> cubePos;
cubePos.reserve(nCubes);

for (int i = 0; i < nCubes; i++) {
    int x, y, z;
    float minV = -5.0f; 
    float maxV =  5.0f;

    x = genRandFloat(minV, maxV);
    y = genRandFloat(minV, maxV);
    z = genRandFloat(minV, maxV);

    glm::vec3 pos(x, y, z);
    cubePos.push_back(pos);
}

// generate point light positions
const int NR_POINT_LIGHTS = 4;
std::vector<glm::vec3> pointLightPos;
pointLightPos.reserve(NR_POINT_LIGHTS);

for (int pointLightIdx = 0; pointLightIdx < NR_POINT_LIGHTS; ++pointLightIdx) {
    glm::vec3 newPos(genRandFloat(-3, 3), genRandFloat(-3, 3), genRandFloat(-3, 3));
    pointLightPos.push_back(newPos);
}


////////////////////
///// TEXTURES /////
////////////////////
// container texture
unsigned int texture = loadTexture((texturePath + "container2.png"));

// container specular texture
unsigned int specularTexture = loadTexture((texturePath + "container2_specular.png"));

///////////////
///// VBO /////
///////////////
unsigned int VBO;
glGenBuffers(1, &VBO);
glBindBuffer(GL_ARRAY_BUFFER, VBO);
glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

///////////////
///// VAO /////
///////////////
// generate VAOs
unsigned int objectVAO, lampVAO;
glGenVertexArrays(1, &objectVAO);
glGenVertexArrays(1, &lampVAO);

// set object VAO
glBindVertexArray(objectVAO);

glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)0);
glEnableVertexAttribArray(0);
glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(3*sizeof(float)));
glEnableVertexAttribArray(1);
glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(6*sizeof(float)));
glEnableVertexAttribArray(2);

// set lamp VAO
glBindVertexArray(lampVAO);

glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)0);
glEnableVertexAttribArray(0);

// unbind to prevent accidental state changes
glBindVertexArray(0);

///////////////////////
///// RENDER LOOP /////
///////////////////////
while (!glfwWindowShouldClose(window)) {
    // update delta time
    float currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    // input
    processInput(window);

    // set background
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // matrices
    glm::mat4 model         = glm::mat4(1.0f);
    glm::mat4 view          = camera->getViewMatrix();
    glm::mat4 projection    = camera->getProjectionMatrix();

    // use object shader
    objectShader.use();

    // set directional light properties 
    objectShader.setVec3("dirLight.direction"   , glm::vec3(1.0));
    objectShader.setVec3("dirLight.ambient"     , glm::vec3(0.2));
    objectShader.setVec3("dirLight.diffuse"     , glm::vec3(0.5));
    objectShader.setVec3("dirLight.specular"    , glm::vec3(1.0));

    // set point light(s) properties
    //for (int plIdx = 0; plIdx < NR_POINT_LIGHTS; ++plIdx) {
    //    std::string st = std::format("pointLights{}", plIdx);
    //    objectShader.setVec3 (std::format("{}.position  = {}", st, glm::vec3(plIdx)));
    //    objectShader.setVec3 (std::format("{}.ambient   = {}", st, glm::vec3(0.2)));
    //    objectShader.setVec3 (std::format("{}.diffuse   = {}", st, glm::vec3(0.5)));
    //    objectShader.setVec3 (std::format("{}.specular  = {}", st, glm::vec3(1.0)));
    //    objectShader.setFloat(std::format("{}.constant  = {}", st, 1.0));
    //    objectShader.setFloat(std::format("{}.linear    = {}", st, 0.09));
    //    objectShader.setFloat(std::format("{}.quadratic = {}", st, 0.032));
    //}

    // TODO: Condense this monstrosity please for the love of god
    objectShader.setVec3  ("pointLights[0].position" ,     glm::vec3(1.0) );
    objectShader.setVec3  ("pointLights[0].ambient"  ,     glm::vec3(0.2) );
    objectShader.setVec3  ("pointLights[0].diffuse"  ,     glm::vec3(0.5) );
    objectShader.setVec3  ("pointLights[0].specular" ,     glm::vec3(1.0) );
    objectShader.setFloat ("pointLights[0].constant" ,     1.0      );
    objectShader.setFloat ("pointLights[0].linear"   ,     0.09     );
    objectShader.setFloat ("pointLights[0].quadratic",     0.032    );

    objectShader.setVec3  ("pointLights[1].position" ,     glm::vec3(2.0) );
    objectShader.setVec3  ("pointLights[1].ambient"  ,     glm::vec3(0.2) );
    objectShader.setVec3  ("pointLights[1].diffuse"  ,     glm::vec3(0.5) );
    objectShader.setVec3  ("pointLights[1].specular" ,     glm::vec3(1.0) );
    objectShader.setFloat ("pointLights[1].constant" ,     1.0      );
    objectShader.setFloat ("pointLights[1].linear"   ,     0.09     );
    objectShader.setFloat ("pointLights[1].quadratic",     0.032    );

    objectShader.setVec3  ("pointLights[2].position" ,     glm::vec3(3.0) );
    objectShader.setVec3  ("pointLights[2].ambient"  ,     glm::vec3(0.2) );
    objectShader.setVec3  ("pointLights[2].diffuse"  ,     glm::vec3(0.5) );
    objectShader.setVec3  ("pointLights[2].specular" ,     glm::vec3(1.0) );
    objectShader.setFloat ("pointLights[2].constant" ,     1.0      );
    objectShader.setFloat ("pointLights[2].linear"   ,     0.09     );
    objectShader.setFloat ("pointLights[2].quadratic",     0.032    );

    objectShader.setVec3  ("pointLights[3].position" ,     glm::vec3(4.0) );
    objectShader.setVec3  ("pointLights[3].ambient"  ,     glm::vec3(0.2) );
    objectShader.setVec3  ("pointLights[3].diffuse"  ,     glm::vec3(0.5) );
    objectShader.setVec3  ("pointLights[3].specular" ,     glm::vec3(1.0) );
    objectShader.setFloat ("pointLights[3].constant" ,     1.0      );
    objectShader.setFloat ("pointLights[3].linear"   ,     0.09     );
    objectShader.setFloat ("pointLights[3].quadratic",     0.032    );

    // set object material properties
    objectShader.setInt("material.diffuse", 0);
    objectShader.setInt("material.specular", 1);
    objectShader.setFloat("material.shininess", 64.0f);

    // set object position
    objectShader.setVec3("viewPos", camera->cameraPos);
    objectShader.setMat4("view", view);
    objectShader.setMat4("projection", projection);

    // bind texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, specularTexture);

    // draw object
    glBindVertexArray(objectVAO);

    for (int i = 0; i < nCubes; i++) {
        model = glm::mat4(1.0f);    
        model = glm::translate(model, cubePos[i]);
        
        float angle = 20.0f * i;
        model = glm::rotate(model, angle, glm::vec3(1.0f, 0.5f, 0.3f));
        objectShader.setMat4("model", model);

        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    // TODO: need to render 4 lamps 
    // update lamp shader
    lampShader.use();

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(1.2f, 1.0f, 2.0f));
    model = glm::scale(model, glm::vec3(0.2f));

    lampShader.setMat4("model", model);
    lampShader.setMat4("view", view);
    lampShader.setMat4("projection", projection);

    lampShader.setVec3("lightColor", glm::vec3(1.0f));

    // draw lamp
    glBindVertexArray(lampVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // unbind vaos to prevent accidental state changes
    glBindVertexArray(0);

    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
    // -------------------------------------------------------------------------------
    glfwSwapBuffers(window);
    glfwPollEvents();
}

// de-allocate all resources once they've outlived their purpose:
// ------------------------------------------------------------------------
glDeleteBuffers(1, &VBO);
glDeleteVertexArrays(1, &objectVAO);
glDeleteVertexArrays(1, &lampVAO);


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
        camera->processKeyboard(FORWARD, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera->processKeyboard(BACKWARD, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera->processKeyboard(LEFT, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera->processKeyboard(RIGHT, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera->processKeyboard(UP, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera->processKeyboard(DOWN, deltaTime);
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
    camera->processMouseMovement(static_cast<float>(xposIn), static_cast<float>(yposIn), true);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    camera->processScrollMovement(yoffset, 0.25f);
}

unsigned int loadTexture(std::string texPath) {
    // read in texture image
    int texWidth, texHeight, nChannels;
    unsigned char* data = stbi_load(texPath.c_str(), &texWidth, &texHeight, &nChannels, 0);

    if (!data) {
        std::cout << "STBI_LOAD::ERROR: Unable to load texture" << std::endl;
        exit(1);
    }

    int format;
    switch (nChannels) {
        case 3: 
            format = GL_RGB;
        case 4: 
            format = GL_RGBA;
    }

    // create and bind texture
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    // wrapping behavior
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

    // filtering behavior
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // load image
    glTexImage2D(GL_TEXTURE_2D, 0, format, texWidth, texHeight, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    // clean-up
    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);

    return texture;
}

float genRandFloat(float min, float max) {
    // Source: user "lastchance" - https://cplusplus.com/forum/general/242186/
    return min + (max - min) * rand() / RAND_MAX;
}
