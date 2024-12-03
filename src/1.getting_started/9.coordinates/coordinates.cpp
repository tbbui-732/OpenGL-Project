#include "../../../include/glad/glad.h"
#include "../../../include/glfw/glfw3.h"
#include "../../../include/learnopengl/shader.h"
#include "../../../include/stb_image/stb_image.h"
#include "../../../include/glm/gtc/type_ptr.hpp"

#include <iostream>
#include <filesystem>
#include <string>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void logError(std::string comment);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
const std::string shaderPath = std::filesystem::current_path().string() + "/src/1.getting_started/9.coordinates/shaders/";
const std::string resourcesPath = std::filesystem::current_path().string() + "/resources/textures/";

int main() {
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
    std::string vertexPath = shaderPath + "vertex.glsl";
    std::string fragmentPath = shaderPath + "fragment.glsl";
    Shader ourShader(vertexPath.c_str(), fragmentPath.c_str());

    ////////////////////
    ///// TEXTURES /////
    ////////////////////
    unsigned int textures[2];
    glGenTextures(2, textures);
   
    // repeat on wrap
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // linear filter when magnifying/minifying
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // load images
    int width, height, nrChannels;
    unsigned char *data;
    
    // load container texture
    glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, textures[0]); // activate texture unit 1
    data = stbi_load((resourcesPath + "container.jpg").c_str(), &width, &height, &nrChannels, 0);
    if (!data) logError("Unable to load container.jpg");
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    // load awesome face texture
    glActiveTexture(GL_TEXTURE1); glBindTexture(GL_TEXTURE_2D, textures[1]); // activate texture unit 2
    stbi_set_flip_vertically_on_load(true);
    data = stbi_load((resourcesPath + "awesomeface.png").c_str(), &width, &height, &nrChannels, 0);
    if (!data) logError("Unable to load awesomeface.jpg");
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    // unload image
    stbi_image_free(data);

    ////////////////////
    ///// VERTICES /////
    ////////////////////
    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
		// positions		  // colors		      // texture coords
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,           // top right
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,           // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,           // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f	        // top left
	};

	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};

    //////////////////////
    ///// INITIALIZE /////
    //////////////////////
    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO); 
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    //////////////////
    ///// OBJECT /////
    //////////////////
    // create and bind vao, vbo, and ebo
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // set attribute pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*) 0); // position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*) (3*sizeof(float))); // color
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*) (6*sizeof(float))); // texture coordinate
    glEnableVertexAttribArray(2);

    // unbind to prevent accidental state changes
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    ///////////////////////
    ///// BIND SHADER /////
    ///////////////////////
    // -----------
    ourShader.use();
    ourShader.setInt("texture1", 0); // update fragment shader's uniform value
    ourShader.setInt("texture2", 1);

    ///////////////////////////////////
    ///// TRANSFORMATION MATRICES /////
    ///////////////////////////////////
    // model matrix
    glm::mat4 model(1.0f);
    model = glm::rotate(model, glm::degrees(-35.0f), glm::vec3(1.0f, 0.0f, 0.0f)); // tilt 45 degrees around x-axis

    // view matrix
    glm::mat4 view(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f)); // move camera back a little bit

    // project matrix
    glm::mat4 projection(1.0f);
    projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f); // 800x600, 0.1 to 100.0

    // send matrices to shader
    glUniformMatrix4fv(glGetUniformLocation(ourShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(ourShader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(ourShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    ///////////////////////
    ///// RENDER LOOP /////
    ///////////////////////
    while (!glfwWindowShouldClose(window)) {
        // input
        // -----
        processInput(window);

        // set background
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // bind textures to corresponding texture units
        // -------------
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textures[0]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, textures[1]);

        // draw element
        // ------------
        ourShader.use();
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // rectangle

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

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
