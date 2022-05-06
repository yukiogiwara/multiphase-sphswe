/**
 * @file camera.hpp
 * @brief Definition of camera
 * @author Yuki Ogiwara
 * @date 2022-05-07
 */

#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

/**
 * @brief camera
 */
class Camera {
public:
    Camera(const glm::vec3 &position, const glm::vec3 &target, const glm::vec3 &world_up, float fov, float aspect_ratio, float clliping_near, float clliping_far);

    glm::mat4 GenViewMatrix();
    glm::mat4 GenProjectionMatrix();

    void SetPosition(glm::vec3 position);
    void SetFov(float fov);
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