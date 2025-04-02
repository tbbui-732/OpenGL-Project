#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

const float offset = 1.0 / 300.0;

void main() {
    // basic
    //FragColor = vec4(vec3(texture(screenTexture, TexCoords)), 1.0);

    // inversed colors
    //FragColor = vec4(vec3(1.0 - texture(screenTexture, TexCoords)), 1.0);

    // kernel matrix effects
    // identities sourced here: [https://en.wikipedia.org/wiki/Kernel_(image_processing)]
    float identityKernel[9] = float[](
         0,  0,  0,
         0,  1,  0,
         0,  0,  0
    );
    float ridgeKernel[9] = float[](
         0, -1,  0,
        -1,  4, -1,
         0, -1,  0
    );
    float sharpenKernel[9] = float[](
         0, -1,  0,
        -1,  5, -1,
         0, -1,  0
    );
    float boxBlur[9] = float[](
         1.0/9,  1.0/9,  1.0/9,
         1.0/9,  1.0/9,  1.0/9,
         1.0/9,  1.0/9,  1.0/9
    );
    float gaussianBlur[9] = float[](
         1.0/16,  2.0/16,  1.0/16,
         2.0/16,  4.0/16,  2.0/16,
         1.0/16,  2.0/16,  1.0/16
    );
    vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // top-left
        vec2( 0.0f,    offset), // top-center
        vec2( offset,  offset), // top-right
        vec2(-offset,  0.0f),   // center-left
        vec2( 0.0f,    0.0f),   // center-center
        vec2( offset,  0.0f),   // center-right
        vec2(-offset, -offset), // bottom-left
        vec2( 0.0f,   -offset), // bottom-center
        vec2( offset, -offset)  // bottom-right
    );
    vec3 sampleTexture[9];
    for (int i = 0; i < 9; ++i) {
        sampleTexture[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
    }
    vec3 col = vec3(0.0);
    for (int i = 0; i < 9; ++i) {
        col += sampleTexture[i] * gaussianBlur[i]; // NOTE(bao): CHANGE KERNEL MATRIX HERE~!
    }
    FragColor = vec4(col, 1.0);
}
