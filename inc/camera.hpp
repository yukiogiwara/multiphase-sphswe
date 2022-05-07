/**
 * @file camera.hpp
 * @brief Definition of camera
 * @author Yuki Ogiwara
 * @date 2022-05-07
 */

#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

/**
 * @brief camera
 */
class Camera {
public:
    Camera(const glm::vec3 &position, const glm::vec3 &target, const glm::vec3 &world_up, float fov, float aspect_ratio, float clliping_near, float clliping_far);
    ~Camera();

    glm::mat4 GenViewMatrix();
    glm::mat4 GenProjectionMatrix();

    void ImGui(GLFWwindow* window);

    void SetPosition(glm::vec3 position);
    void SetAspectRatio(float aspect_ratio);

public:

private:
    void Update();

private:
    glm::vec3 position_;
    glm::vec3 target_;
    glm::vec3 world_up_;
    glm::vec3 front_;
    glm::vec3 right_;
    glm::vec3 up_;
    float fov_;
    float aspect_ratio_;
    float clliping_near_;
    float clliping_far_;
};