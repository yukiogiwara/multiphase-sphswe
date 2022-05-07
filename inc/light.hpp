/**
 * @file light.hpp 
 * @brief Definition of light
 * @author Yuki Ogiwara
 * @date 2022-05-07
 */

#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

/**
 * @brief directional light
 */
class DirectionalLight {
public:
    DirectionalLight(const glm::vec3 &dir, const glm::vec3 &amb, const glm::vec3 &dif, const glm::vec3 &spe);
    ~DirectionalLight();

    void ImGui(GLFWwindow* window);

    glm::vec3 GetDirection() const;
    glm::vec3 GetAmbient() const;
    glm::vec3 GetDiffuse() const;
    glm::vec3 GetSpecular() const;

public:

private:

private:
    glm::vec3 direction_;
    glm::vec3 ambient_;
    glm::vec3 diffuse_;
    glm::vec3 specular_;
};