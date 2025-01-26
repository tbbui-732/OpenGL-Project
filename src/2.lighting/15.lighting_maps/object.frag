#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;
  
uniform vec3 lightPos; 
uniform vec3 lightColor; // why aren't these being used?
uniform vec3 objectColor; // why aren't these being used?
uniform vec3 viewPos;

uniform sampler2D ourTexture;

struct Material {
    sampler2D diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material material;
uniform Light light;

void main()
{
    //// --- ambient --- 
    //vec3 ambient = light.ambient * material.ambient;
  	//
    //// --- diffuse ---
    //vec3 norm = normalize(Normal);
    //vec3 lightDirection = normalize(lightPos - FragPos);
    //float diff = max(dot(norm, lightDirection), 0.0);
    //vec3 diffuse = light.diffuse * (diff * material.diffuse);

    //// --- specular ---
    //vec3 viewDirection = normalize(viewPos - FragPos);
    //vec3 reflectDirection = reflect(-lightDirection, norm);
    //float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), 
    //                    material.shininess);
    //vec3 specular = light.specular * (spec * material.specular); 

    // TODO: Uncomment this once done applying textures
    //FragColor = vec4((ambient + diffuse + specular), 1.0);

    FragColor = texture(ourTexture, TexCoord);
} 
