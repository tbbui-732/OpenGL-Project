#ifndef MESH_H
#define MESH_H

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoord;
};

struct Texture {
    unsigned int id;
    std::string type; // diffuse, specular
    std::string path;
};

class Mesh {
private:
    // render data
    unsigned int VAO, VBO, EBO;

    void setupMesh() {
        // set vertex array
        glGenVertexArrays(1, &VAO);

        // set buffers
        glGenBuffers(1, &EBO);
        glGenBuffers(1, &VBO);

        // bind vao
        glBindVertexArray(VAO);

        // send data to vbo (?)
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), 
                        &vertices[0], GL_STATIC_DRAW);

        // send data to ebo (?)
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), 
                        &indices[0], GL_STATIC_DRAW);

        // position
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        glEnableVertexAttribArray(0);

        // normal
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
        glEnableVertexAttribArray(1);

        // texture
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoord));
        glEnableVertexAttribArray(2);

        // unbind vao to prevent accidental state changes
        glBindVertexArray(0);
    }

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

        setupMesh();
    }

    void Draw(Shader &shader) {
        unsigned int diffuseNr = 1;
        unsigned int specularNr = 1;

        // bind the textures first
        for (int texUnit = 0; texUnit < textures.size(); ++texUnit) {
            glActiveTexture(GL_TEXTURE0 + texUnit);

            // receive texture number (the "N" in diffuse_textureN)
            std::string name, number;
            name = textures[texUnit].type;

            if (name == "texture_diffuse") {
                number = std::to_string(diffuseNr++);
            }
            else if (name == "texture_specular") {
                number = std::to_string(specularNr++);
            }
            else {
                std::cout << "MESH::DRAW::ERROR - textures[tex].type is invalid! Must be 'texture_diffuse' or 'texture_specular'" << std::endl;
                exit(1);
            }

            shader.setFloat(("material." + name + number).c_str(), texUnit);
            glBindTexture(GL_TEXTURE_2D, textures[texUnit].id);
        }
        glActiveTexture(GL_TEXTURE0);

        // draw mesh
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

        // unbind to prevent accidental state changes
        glBindVertexArray(0);
    }
};

#endif
