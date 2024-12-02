//#version 330 core
//out vec4 FragColor;
//
//in vec3 ourColor;
//in vec2 ourTexCoord;
//
//uniform sampler2D texture1;
//uniform sampler2D texture2;
//float mixValue = 0.2;
//
//void main()
//{
//    vec2 vertFlippedTexCoord = vec2(ourTexCoord.x*-1, ourTexCoord.y);
//    FragColor = mix(texture(texture1, ourTexCoord),
//                    texture(texture2, vertFlippedTexCoord), mixValue);
//}

#version 330 core
out vec4 FragColor;

void main()
{
    FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);
}
