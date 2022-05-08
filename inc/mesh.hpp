/**
 * @file mesh.hpp 
 * @brief Definition of mesh
 * @author Yuki Ogiwara
 * @date 2022-05-08
 */

#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>

/**
 * @brief mesh 
 */
class Mesh {
public:
    Mesh();
    ~Mesh();

    void SendDataToBuffer();
    void Draw();

public:
    std::vector<glm::vec3> vertices_;
    std::vector<unsigned int> indices_;

private:

private:
    GLuint vao_;
    GLuint vbo_;
    GLuint ebo_;
};