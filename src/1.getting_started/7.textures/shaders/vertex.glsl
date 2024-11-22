#version 330 core
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 aTexCoord;

out vec3 ourPosition;
out vec2 ourTexCoord;

void main() {
    ourPosition = aPosition; 
    ourTexCoord = aTexCoord;
}
