#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 ourTexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;
vec2 vertFlippedTexCoord = vec2(ourTexCoord.x*-1, ourTexCoord.y);

void main()
{
    FragColor = mix(texture(texture1, ourTexCoord),
                    texture(texture2, vertFlippedTexCoord), 0.2f);
}
