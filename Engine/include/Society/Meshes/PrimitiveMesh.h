//
// Created by Peter Farber on 5/23/24.
//

#ifndef PRIMITIVEMESH_H
#define PRIMITIVEMESH_H
#include <vector>
#include <glad/glad.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace sc {
    // struct Vertex;
    struct MeshVertex {
        // position
        glm::vec3 Position{};
        // normal
        glm::vec3 Normal{};
        // texCoords
        glm::vec2 TexCoords{};
    };


    class PrimitiveMesh {

        protected:
            std::vector<MeshVertex> m_vertices;
            std::vector<unsigned int> m_indices;
            unsigned int m_VAO;
            unsigned int m_VBO;
            unsigned int m_EBO;

            template<typename T, size_t N>
            static void ConvertToMeshVerticies(
                T (&in)[N],
                std::vector<MeshVertex> &out) {
                constexpr unsigned int size = sizeof(in) / sizeof(float);
                constexpr unsigned int len = sizeof(MeshVertex) / sizeof(float);

                for (unsigned int i = len - 1; i < size; i += len) {
                    const auto position = glm::vec3(
                        in[i - (len - 1)],
                        in[i - (len - 2)],
                        in[i - (len - 3)]);
                    const auto normal = glm::vec3(
                        in[i - (len - 4)],
                        in[i - (len - 5)],
                        in[i - (len - 6)]);
                    const auto texCoords = glm::vec2(
                        in[i - (len - 7)],
                        in[i - (len - 8)]);
                    out.push_back({position, normal, texCoords});
                }
            }

        public:
            PrimitiveMesh() {
                m_VAO = 0;
                m_VBO = 0;
                m_EBO = 0;
            }

            virtual ~PrimitiveMesh() {
                glDeleteVertexArrays(1, &m_VAO);
                glDeleteBuffers(1, &m_VBO);
                glDeleteBuffers(1, &m_EBO);
            }


            virtual void Draw() = 0;

    };
}


#endif //PRIMITIVEMESH_H
