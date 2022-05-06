/**
 * @file shader.hpp
 * @brief Definition of shader
 * @author Yuki Ogiwara
 * @date 2022-05-07
 */

#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

/**
 * @brief shader
 */
class Shader {
public:
    unsigned int id_;

public:
    Shader(const char* vertex_path, const char* fragment_path);
    ~Shader();

    void Use();
    void SetBool(const std::string &name, bool value) const;
    void SetInt(const std::string &name, int value) const;
    void SetFloat(const std::string &name, float value) const;
    void SetVec3(const std::string &name, const glm::vec3 &value) const;
    void SetMat4(const std::string &name, const glm::mat4 &value) const;

private:

private:

};