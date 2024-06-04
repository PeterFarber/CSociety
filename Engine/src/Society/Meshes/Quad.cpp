//
// Created by Peter Farber on 5/23/24.
//
#include <Society/Meshes/Quad.h>

#include <glad/glad.h>
#include <vector>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace sc {

    Quad::Quad() {

        constexpr float vertices[] = {
            // positions        // texture Coords
            -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
            1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
            1.0f, -1.0f, 0.0, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
        };

        ConvertToMeshVerticies(vertices, m_vertices);

        // setup plane VAO
        glGenVertexArrays(1, &m_VAO);
        glGenBuffers(1, &m_VBO);
        glBindVertexArray(m_VAO);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(
            GL_ARRAY_BUFFER,
            static_cast<GLint>(m_vertices.size() * sizeof(MeshVertex)),
            &m_vertices[0],
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

    void Quad::Draw() {
        glBindVertexArray(m_VAO);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glBindVertexArray(0);
    }


}
