#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 ourTexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float mixValue = 0.2;

void main()
{
    vec2 vertFlippedTexCoord = vec2(ourTexCoord.x*-1, ourTexCoord.y);
    FragColor = mix(texture(texture1, ourTexCoord),
                    texture(texture2, vertFlippedTexCoord), mixValue);
}
