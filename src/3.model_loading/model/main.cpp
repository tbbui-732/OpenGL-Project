#include "glad/glad.h"
#include "glfw/glfw3.h"
#include "learnopengl/shader.h"
#include "learnopengl/camera.h"
#include "stb_image/stb_image.h"

// global includes
#include <cstdio>
#include <iostream>
#include <filesystem>
#include <ostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <vector>

// structs
typedef struct Attenuation {
    float constant;
    float linear;
    float quadratic;
} Attenuation;

typedef struct Phong {
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
} Phong;

typedef struct DirectionalLight {
    glm::vec3   direction;
    Phong       phong; 
} DirectionalLight;

typedef struct PointLight {
    glm::vec3   position;
    glm::vec3   baseColor;
    Phong       phong;
    Attenuation attenuation;
} PointLight;

#include "include/learnopengl/theme.h"

// declarations
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void logError(std::string comment);
void mouse_callback(GLFWwindow* window, double xpos, double ypos); // xpos and ypos are the mouse's current position
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
float genRandFloat(float min, float max);
unsigned int loadTexture(std::string texPath);
void setPointLights(const std::vector<PointLight>& PointLights, const Shader& shaderProgram, const int MAX_POINT_LIGHTS);

// settings
const unsigned int SCR_WIDTH    = 1200;
const unsigned int SCR_HEIGHT   = 800;
const std::string shaderPath    = std::filesystem::current_path().string() + "/../src/3.model_loading/model/"; // NOTE: make sure to update this correctly!
const std::string texturePath   = std::filesystem::current_path().string() + "/../resources/textures/"; // NOTE: make sure to update this correctly!
const Theme::PhongTheme THEME   = Theme::NORMAL;
const Attenuation att = { 1.0f, 0.09f, 0.032f };

// frames
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// camera
Camera* camera = new Camera(SCR_WIDTH, SCR_HEIGHT);

// toggles flashlight state in object fragment
bool flashLightOn = false;


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
glfwSetKeyCallback(window, key_callback);


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

//////////////////////////
///// LIGHTING/THEME /////
//////////////////////////
// set properties for background
glm::vec3 background;
Theme::setBackgroundColor(background, THEME);

// set properties for directional light
DirectionalLight dirLight;
dirLight.direction = glm::vec3(0.0, 1.0, 0.0);
Theme::setDirectionLightPhong(dirLight, THEME);

// generate point light settings
const int NR_POINT_LIGHTS = 4;
std::vector<PointLight> PointLights;
PointLights.reserve(NR_POINT_LIGHTS);

for (int idx = 0; idx < NR_POINT_LIGHTS; ++idx) {
    int x, y, z;
    x = genRandFloat(-3, 3);
    y = genRandFloat(-3, 3);
    z = genRandFloat(-3, 3);
    glm::vec3 pos(x, y, z);

    PointLight pl;
    pl.position = pos;
    pl.attenuation = att;
    Theme::setPointLightPhong(pl, THEME);

    PointLights.push_back(pl);
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
    glClearColor(background.x, background.y, background.z, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // matrices
    glm::mat4 model         = glm::mat4(1.0f);
    glm::mat4 view          = camera->getViewMatrix();
    glm::mat4 projection    = camera->getProjectionMatrix();

    // use object shader
    objectShader.use();

    // set directional light properties 
    objectShader.setVec3("dirLight.direction"   , dirLight.direction);
    objectShader.setVec3("dirLight.ambient"     , dirLight.phong.ambient);
    objectShader.setVec3("dirLight.diffuse"     , dirLight.phong.diffuse);
    objectShader.setVec3("dirLight.specular"    , dirLight.phong.specular);

    // set point light(s) properties
    setPointLights(PointLights, objectShader, NR_POINT_LIGHTS);

    // set spot light property (flashlight)
    objectShader.setBool("flashLightOn", flashLightOn);

    if (flashLightOn) {
        objectShader.setVec3("flashLight.position"    , camera->cameraPos);
        objectShader.setVec3("flashLight.direction"   , camera->cameraFront);
        objectShader.setVec3("flashLight.diffuse"     , glm::vec3(1.0));
        objectShader.setVec3("flashLight.specular"    , glm::vec3(1.0));
        objectShader.setFloat("flashLight.constant"   , 1.0f);
        objectShader.setFloat("flashLight.linear"     , 0.09f);
        objectShader.setFloat("flashLight.quadratic"  , 0.032f);
        objectShader.setFloat("flashLight.innerAngle" , glm::cos(glm::radians(12.0f)));
        objectShader.setFloat("flashLight.outerAngle" , glm::cos(glm::radians(17.0f)));
    }

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

    // update lamp shader
    lampShader.use();

    for (int lampIdx = 0; lampIdx < NR_POINT_LIGHTS; ++lampIdx) {
        model = glm::mat4(1.0f);
        model = glm::translate(model, PointLights[lampIdx].position);
        model = glm::scale(model, glm::vec3(0.2f));

        lampShader.setMat4("model", model);
        lampShader.setMat4("view", view);
        lampShader.setMat4("projection", projection);
        lampShader.setVec3("lightColor", PointLights[lampIdx].baseColor);

        glBindVertexArray(lampVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

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

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_RELEASE) {
        return;
    }

    if (key == GLFW_KEY_F) {
        flashLightOn = !flashLightOn;
    }
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

void setPointLights(const std::vector<PointLight>& PointLights, const Shader& shaderProgram, const int MAX_POINT_LIGHTS) {
    // NOTE: this is super restrictive, but i can't imagine myself adding more than 10 point lights for the
    //  scope of this project anyways
    if (MAX_POINT_LIGHTS > 9) {
        logError("SETPOINTLIGHTS::ERROR::MAX_POINT_LIGHTS_EXCEEDED");
        exit(1);
    }

    const int sz = (std::string("pointLights[n]")).size() + 1; // golly i love strings
    char plChArr[sz];
    int idx = 0;

    for (const PointLight& pl : PointLights) {
        snprintf(plChArr, sz, "pointLights[%d]", idx++);
        const std::string plStr(plChArr);

        shaderProgram.setVec3 (plStr + ".position" , pl.position);
        shaderProgram.setVec3 (plStr + ".ambient"  , pl.phong.ambient);
        shaderProgram.setVec3 (plStr + ".diffuse"  , pl.phong.diffuse);
        shaderProgram.setVec3 (plStr + ".specular" , pl.phong.specular);
        shaderProgram.setFloat(plStr + ".constant" , pl.attenuation.constant);
        shaderProgram.setFloat(plStr + ".linear"   , pl.attenuation.linear);
        shaderProgram.setFloat(plStr + ".quadratic", pl.attenuation.quadratic);
    }
}
