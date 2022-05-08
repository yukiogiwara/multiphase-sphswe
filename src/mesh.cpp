/**
 * @file mesh.cpp
 * @brief Implementation of mesh
 * @author Yuki Ogiwara
 * @date 2022-05-08
 */

#include "mesh.hpp"

/**
 * @brief constructor
 */
Mesh::Mesh() {
    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo_);
    glGenBuffers(1, &ebo_);
}

/**
 * @brief destructor
 */
Mesh::~Mesh() {
    glDeleteBuffers(1, &ebo_);
    glDeleteBuffers(1, &vbo_);
    glDeleteVertexArrays(1, &vao_);
}

/**
 * @brief send vertex data to buffer
 */
void Mesh::SendDataToBuffer() {
    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*vertices_.size(), vertices_.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*indices_.size(), indices_.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(0);
}

/**
 * @brief draw object
 */
void Mesh::Draw() {
    glBindVertexArray(vao_);
    glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}