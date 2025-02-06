// TODO create a header file for this cpp file

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec3 TexCoord;
};

struct Texture {
    unsigned int id;
    std::string type;
};
