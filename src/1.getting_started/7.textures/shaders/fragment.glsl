//#version 330 core
//out vec4 FragColor;
//
//in vec3 ourColor;
//in vec2 ourTexCoord;
//
//uniform sampler2D texture1;
//
//void main() {
//    FragColor = vec4(ourColor, 1.0);
//}

#version 330 core
out vec4 FragColor;
void main()
{
   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
}
