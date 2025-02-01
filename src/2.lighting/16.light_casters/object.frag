#version 330 core
out vec4 FragColor;

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

struct FlashLight {
    vec3 position; 
    vec3 direction; 
    float cutOff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;
  
uniform vec3 viewPos;

uniform Material material;
uniform DirectionalLight sun;
uniform PointLight lamp;
uniform FlashLight torch;

void main()
{
    vec3 torchDir = normalize(torch.position - FragPos);
    float theta = dot(torchDir, normalize(-torch.direction));

    if (theta > torch.cutOff)
    {    
        // ambient
        vec3 ambient = torch.ambient * vec3(texture(material.diffuse, TexCoord));
        
        // diffuse 
        vec3 norm = normalize(Normal);
        float diff = max(dot(norm, torchDir), 0.0);
        vec3 diffuse = torch.diffuse * diff * vec3(texture(material.diffuse, TexCoord));  
        
        // specular
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-torchDir, norm);  
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        vec3 specular = torch.specular * spec * vec3(texture(material.specular, TexCoord));  
        
        // attenuation
        float distance    = length(torch.position - FragPos);
        float attenuation = 1.0 / (lamp.constant + lamp.linear * distance + lamp.quadratic * (distance * distance));    

        diffuse *= attenuation;
        specular *= attenuation;   
            
        vec3 result = ambient + diffuse + specular;
        FragColor = vec4(result, 1.0);
    }
    else 
    {
        FragColor = vec4(torch.ambient * vec3(texture(material.diffuse, TexCoord)), 1.0);
    }



    //// --- ambient --- 
    //vec3 ambient          = sun.ambient * vec3(texture(material.diffuse, TexCoord));
  	//
    //// --- diffuse ---
    //vec3 norm             = normalize(Normal);
    //vec3 lightDir         = normalize(-sun.direction); // NOTE: Negative b/c it's easier to imagine the light coming FROM the light source
    //float diff            = max(dot(norm, lightDir), 0.0);
    //vec3 diffuse          = sun.diffuse * diff * vec3(texture(material.diffuse, TexCoord));

    //// --- specular ---
    //vec3 viewDir          = normalize(viewPos - FragPos);
    //vec3 reflectDir       = reflect(-lightDir, norm);
    //float spec            = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    //vec3 specular         = sun.specular * spec * vec3(texture(material.specular, TexCoord));





    //// --- attenuation ---
    //float distance        = length(lamp.position - FragPos);
    //float attenuation     = 1.0 / (lamp.constant + lamp.linear * distance + lamp.quadratic * (distance * distance));

    //// --- ambient --- 
    //vec3 ambient          = lamp.ambient * vec3(texture(material.diffuse, TexCoord));
  	//
    //// --- diffuse ---
    //vec3 norm             = normalize(Normal);
    //vec3 lightDir         = normalize(lamp.position - FragPos);
    //float diff            = max(dot(norm, lightDir), 0.0);
    //vec3 diffuse          = lamp.diffuse * diff * vec3(texture(material.diffuse, TexCoord));

    //// --- specular ---
    //vec3 viewDir          = normalize(viewPos - FragPos);
    //vec3 reflectDir       = reflect(-lightDir, norm);
    //float spec            = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    //vec3 specular         = lamp.specular * spec * vec3(texture(material.specular, TexCoord));

    //// --- apply ---
    //ambient     *= attenuation;
    //diffuse     *= attenuation;
    //specular    *= attenuation;
    //FragColor = vec4((ambient + diffuse + specular), 1.0);
} 
