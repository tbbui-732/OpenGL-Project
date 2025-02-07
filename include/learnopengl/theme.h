#ifndef THEME_H
#define THEME_H

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

        background = getColor(color);
    }  

    // TODO: Needs testing
    inline void setDirectionLightPhong(DirectionalLight& dirLight, const PhongTheme theme) {
        glm::vec3 ambient, diffuse, specular;

        switch (theme) {
            case NORMAL: {
                ambient  = glm::vec3(0.2f);
                diffuse  = glm::vec3(0.5f);
                specular = glm::vec3(1.0f);
                break;
            }
            case HORROR: {
                ambient  = glm::vec3(0.0f);
                diffuse  = glm::vec3(0.0f);
                specular = glm::vec3(0.0f);
                break;
            }
            case FACTORY: {
                glm::vec3 blue = getColor(LIGHT_BLUE);
                ambient  =  blue * 0.2f;
                diffuse  =  blue * 0.5f;
                specular =  blue * 1.0f;
                break;
            }
            case BIOCHEM: {
                glm::vec3 green = getColor(LIGHT_GREEN);
                ambient  =  green * 0.2f;
                diffuse  =  green * 0.5f;
                specular =  green * 1.0f;
                break;
            }
        }

        dirLight.phong = {ambient, diffuse, specular};
    }  

    inline void setPointLightPhong(PointLight& pointLight, const PhongTheme theme) {
        glm::vec3 ambient, diffuse, specular, color;

        switch (theme) {
            case NORMAL:
                color = glm::vec3(1.0f);
                break;
            case HORROR:
                color = getColor(RED);
                break;
            case FACTORY:
                color = getColor(LIGHT_BLUE);
                break;
            case BIOCHEM:
                color = getColor(LIGHT_GREEN);
                break;
        }

        ambient = color * 0.2f;
        diffuse = color * 0.5f;
        specular = color * 1.0f;

        pointLight.phong = {ambient, diffuse, specular};
        pointLight.baseColor = color;
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
