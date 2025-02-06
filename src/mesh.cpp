// TODO create a header file for this cpp file

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoord;
};

struct Texture {
    unsigned int id;
    std::string type;
};

class Mesh {
    public:
        // mesh data
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;

        // ctor
        Mesh(
            std::vector<Vertex> vertices,
            std::vector<unsigned int> indices,
            std::vector<Texture> textures) {

            this->vertices = vertices;
            this->indices = indices;
            this->textures = textures;

        }

        void Draw(Shader &shader) {
            // TODO ...
        }

    private:
        // render data
        unsigned int VAO, VBO, EBO;

        void setupMesh() {
            // TODO ...
        }
}
