//
// Created by Peter Farber on 5/23/24.
//

#include <Society/Meshes/Sphere.h>
#include <glad/glad.h>
#include <vector>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace sc {
    Sphere::Sphere() {

        if (m_VAO == 0) {

            glGenVertexArrays(1, &m_VAO);

            glGenBuffers(1, &m_VBO);
            glGenBuffers(1, &m_EBO);

            for (unsigned int x = 0; x <= X_SEGMENTS; ++x) {
                for (unsigned int y = 0; y <= Y_SEGMENTS; ++y) {
                    const float xSegment =
                        static_cast<float>(x) / static_cast<float>(X_SEGMENTS);
                    const float ySegment =
                        static_cast<float>(y) / static_cast<float>(Y_SEGMENTS);
                    const float xPos =
                        std::cos(xSegment * 2.0f * PI) * std::sin(
                            ySegment * PI);
                    const float yPos = std::cos(ySegment * PI);
                    const float zPos =
                        std::sin(xSegment * 2.0f * PI) * std::sin(
                            ySegment * PI);
                    MeshVertex vertex;
                    vertex.Position = glm::vec3(xPos, yPos, zPos);
                    vertex.Normal = glm::vec3(xPos, yPos, zPos);
                    vertex.TexCoords = glm::vec2(xSegment, ySegment);
                    m_vertices.push_back(vertex);
                }
            }

            bool oddRow = false;
            for (unsigned int y = 0; y < Y_SEGMENTS; ++y) {
                if (!oddRow) // even rows: y == 0, y == 2; and so on
                {
                    for (unsigned int x = 0; x <= X_SEGMENTS; ++x) {
                        m_indices.push_back(y * (X_SEGMENTS + 1) + x);
                        m_indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
                    }
                } else {
                    for (int x = X_SEGMENTS; x >= 0; --x) {
                        m_indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
                        m_indices.push_back(y * (X_SEGMENTS + 1) + x);
                    }
                }
                oddRow = !oddRow;
            }
            m_indexCount = static_cast<unsigned int>(m_indices.size());

            glBindVertexArray(m_VAO);
            glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
            glBufferData(
                GL_ARRAY_BUFFER,
                static_cast<GLint>(m_vertices.size() * sizeof(MeshVertex)),
                &m_vertices[0],
                GL_STATIC_DRAW);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
            glBufferData(
                GL_ELEMENT_ARRAY_BUFFER,
                static_cast<GLint>(m_indices.size() * sizeof(unsigned int)),
                &m_indices[0],
                GL_STATIC_DRAW);

            // set the vertex attribute pointers
            // vertex Positions
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(
                0,
                3,
                GL_FLOAT,
                GL_FALSE,
                sizeof(MeshVertex),
                static_cast<void *>(nullptr)
                );
            // vertex normals
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(
                1,
                3,
                GL_FLOAT,
                GL_FALSE,
                sizeof(MeshVertex),
                reinterpret_cast<void *>(offsetof(MeshVertex, Normal))
                );
            // vertex texture coords
            glEnableVertexAttribArray(2);
            glVertexAttribPointer(
                2,
                2,
                GL_FLOAT,
                GL_FALSE,
                sizeof(MeshVertex),
                reinterpret_cast<void *>(offsetof(MeshVertex, TexCoords))
                );
            glBindVertexArray(0);

        }

    }

    void Sphere::Draw() {
        glBindVertexArray(m_VAO);
        glDrawElements(
            GL_TRIANGLE_STRIP,
            static_cast<GLint>(m_indexCount),
            GL_UNSIGNED_INT,
            nullptr);
        glBindVertexArray(0);

    }
}
