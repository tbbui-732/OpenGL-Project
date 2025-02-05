#ifndef THEME_H
#define THEME_H

// enums
enum PhongTheme { NORMAL, HORROR, FACTORY, BIOCHEM };

// declarations
void setDirLight(); // TODO
void setPointLight(); // TODO
void setSpotLight(); // TODO
void setBackground(); // TODO

// set up values for "colors"
const std::vector<std::vector<float>> rgbVals = {
    {210.0, 180.0, 140.0}, // Light Tan 
    {64.0, 64.0, 64.0},    // Dark Gray 
    {211.0, 211.0, 211.0}, // Light Gray 
    {0.0, 0.0, 0.0},       // Black 
    {255.0, 255.0, 255.0}, // White 
    {255.0, 0.0, 0.0},     // Red 
    {255.0, 102.0, 102.0}, // Light Red 
    {255.0, 255.0, 153.0}, // Light Yellow 
    {173.0, 216.0, 230.0}, // Light Blue 
    {144.0, 238.0, 144.0}  // Light Green 
};

class Theme {
private:
    std::vector<glm::vec3> colors;
    Shader shaderProgram;
    std::string dirLight; 
    std::string pointLight;
    std::string spotLight;

    void normalizeColor(glm::vec3& rgb) {
        rgb.x /= 255.0f; 
        rgb.y /= 255.0f; 
        rgb.z /= 255.0f; 
    }

public:
    Theme(  
        const Shader& _shaderProgram, 
        const std::string& _dirLight, 
        const std::string& _pointLight, 
        const std::string& _spotLight ) 
    {
        // set private variables
        this->shaderProgram = _shaderProgram;
        this->dirLight      = _dirLight;
        this->pointLight    = _pointLight;
        this->spotLight     = _spotLight;
        
        // update colors
        colors.reserve(rgbVals.size());
        for (std::vector<int> rgbVal : rgbVals) {
            glm::vec3 color(rgbVal[0], rgbVal[1], rgbVal[2]);
            normalizeColor(color);
            colors.push_back(color);
        }
    }
};


#endif
