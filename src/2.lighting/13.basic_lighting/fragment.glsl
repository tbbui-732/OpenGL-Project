#version 330 core
out vec4 FragColor;

uniform vec3 lightColor;
uniform vec3 objectColor;

float ambientStrength = 0.1f;

void main()
{
    vec3 ambientObject = objectColor * ambientStrength;
    FragColor = vec4(ambientObject * lightColor, 1.0f);
}
