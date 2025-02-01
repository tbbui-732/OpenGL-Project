#version 330 core
out vec4 FragColor;

// Input from vertex shader
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

// Declarations
vec3 CalcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir);
 
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
}

// View position
uniform vec3 viewPos;

// Material
uniform Material material;

// Lighting
uniform DirectionalLight dirLight;

// ----- FRAGMENT SHADER MAIN ----- 
void main() {

} 

// Function Definitions
vec3 CalcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir) {
    // ambient 
    vec3 ambient = light.ambient * vec3(texture(material.ambient, TexCoord));

    // diffuse 
    vec3 lightDir = normalize(-light.direction);
    vec3 diffuse = light.diffuse * max(dot(normal, lightDir), 0.0) * 
                        vec3(texture(material.diffuse, TexCoord));

    // specular 
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 spec = pow(max(dot(reflectDir, viewDir),0.0), material.shininess);
    vec3 specular = light.specular * spec * 
                        vec3(texture(material.specular, TexCoord));

    return (ambient + diffuse + specular);
}
