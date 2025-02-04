#version 330 core
#define NR_POINT_LIGHTS 4

out vec4 FragColor;

// Input from vertex shader
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

// Structs
struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct DirectionalLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float constant;
    float linear;
    float quadratic;
};

struct SpotLight {
    vec3 position;
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;

    float innerAngle;
    float outerAngle;
};

// Declarations
vec3 CalcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 viewDir);

// View position
uniform vec3 viewPos;

// Material
uniform Material material;

// Lighting
uniform DirectionalLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];

// TODO: Write up a function to calculate the SpotLight.
// In this case, I want to create a flashlight, so it'll be attached to 
//  the player... GOODLUCK!
uniform SpotLight flashLight; // attached to the player

// ----- FRAGMENT SHADER MAIN ----- 
void main() {
    // properties
    vec3 normal = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    //// phase 1: direction light 
    //vec3 result = CalcDirLight(dirLight, normal, viewDir);

    //// phase 2: point lights
    //for (int plIdx = 0; plIdx < NR_POINT_LIGHTS; ++plIdx) {
    //    result += CalcPointLight(pointLights[plIdx], normal, viewDir);
    //}

    // TODO: phase 3: spot lights
    //result += CalcSpotLight(flashLight, normal, viewDir);
    vec3 result = CalcSpotLight(flashLight, normal, viewDir);

    FragColor = vec4(result, 1.0);
} 

// Function Definitions
vec3 CalcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir) {
    // ambient 
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));

    // diffuse 
    vec3 lightDir = normalize(-light.direction);
    vec3 diffuse = light.diffuse * max(dot(normal, lightDir), 0.0) * 
                        vec3(texture(material.diffuse, TexCoord));

    // specular 
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(reflectDir, viewDir),0.0), material.shininess);
    vec3 specular = light.specular * spec * 
                        vec3(texture(material.specular, TexCoord));

    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 viewDir) {
    // ambient 
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));

    // diffuse 
    vec3 lightDir = normalize(light.position - FragPos);
    vec3 diffuse = light.diffuse * max(dot(normal, lightDir), 0.0) * 
                        vec3(texture(material.diffuse, TexCoord));

    // specular 
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(reflectDir, viewDir),0.0), material.shininess);
    vec3 specular = light.specular * spec * 
                        vec3(texture(material.specular, TexCoord));

    // attenuation (light intensity based off distance)
    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / ((light.constant) +
                               (light.linear * distance) +
                               (light.quadratic * distance * distance));

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 viewDir) {
    // ambient
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));

    // diffuse
    vec3 lightDir = normalize(light.position - FragPos);
    vec3 diffuse = light.diffuse * max(dot(normal, lightDir), 0.0) * vec3(texture(material.diffuse, TexCoord));

    // specular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoord));

    // spotlight (with soft edges)
    float theta = dot(lightDir, normalize(-light.direction)); // NOTE: why do we negate viewDir? Is that even necessary?
    float epsilon = (light.innerAngle - light.outerAngle);
    float intensity = clamp((theta - light.outerAngle)/epsilon, 0.0, 1.0);
    diffuse *= intensity;
    specular *= intensity;

    // attenuation
    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / ((light.constant) +
                               (light.linear * distance) + 
                               (light.quadratic * distance * distance));

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}
