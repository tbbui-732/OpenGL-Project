#ifndef THEME_H
#define THEME_H

// NOTE: DELETE ONCE DONE USING! This is for LSP purposes only
#include <vector>
#include "../glm/glm.hpp"
#include "../learnopengl/shader.h"

namespace Theme {
    // enums
    enum PhongTheme { 
        NORMAL,
        HORROR,
        FACTORY,
        BIOCHEM
    };

    enum Color {
        LIGHT_TAN,
        DARK_GRAY,
        LIGHT_GRAY,
        BLACK,
        WHITE,
        RED,
        LIGHT_RED,
        LIGHT_YELLOW,
        LIGHT_BLUE,
        LIGHT_GREEN
    };

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

    inline void setBackgroundColor(glm::vec3& background, const PhongTheme theme);
    inline void setDirectionLightPhong();
    inline void setPointLightPhong();
    inline void setSpotLightPhong();
    inline glm::vec3 getColor(Color color);
    inline glm::vec3 arrToVec(std::vector<float> arr);
    inline void rgbToUnit(glm::vec3& vec);

    inline void setBackgroundColor(glm::vec3& background, const PhongTheme theme) {
        Color color;
        switch (theme) {
            case NORMAL:
                color = DARK_GRAY;
                break;
            case HORROR:
                color = BLACK;
                break;
            case FACTORY:
                color = LIGHT_GRAY;
                break;
            case BIOCHEM:
                color = WHITE;
                break;
        }

        std::vector<float> colorArr = rgbVals[color];
        glm::vec3 colorVec = arrToVec(colorArr);
        background = colorVec;
    }  

    inline void setDirectionLightPhong(Shader& shaderProgram) {
        
    }  

    inline void setPointLightPhong() {

    }  

    inline void setSpotLightPhong() {

    }  

    inline glm::vec3 getColor(Color color) {
        std::vector<float> colorArr = rgbVals[color];
        glm::vec3 colorVec = arrToVec(colorArr);
        return colorVec;
    }

    inline glm::vec3 arrToVec(std::vector<float> arr) {
        glm::vec3 vec(arr[0], arr[1], arr[2]);
        rgbToUnit(vec);
        return vec;
    }

    inline void rgbToUnit(glm::vec3& vec) {
        float scale = 255.0f;
        vec.x /= scale;
        vec.y /= scale;
        vec.z /= scale;
    }
}

#endif
