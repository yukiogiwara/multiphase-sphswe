/**
 * @file light.hpp 
 * @brief Definition of light
 * @author Yuki Ogiwara
 * @date 2022-05-07
 */

#pragma once

#include <glm/glm.hpp>

/**
 * @brief directional light
 */
class DirectionalLight {
public:
    DirectionalLight(const glm::vec3 &dir, const glm::vec3 &amb, const glm::vec3 &dif, const glm::vec3 &spe);
    ~DirectionalLight();

public:
    glm::vec3 GetDirection() const;
    glm::vec3 GetAmbient() const;
    glm::vec3 GetDiffuse() const;
    glm::vec3 GetSpecular() const;

private:

private:
    glm::vec3 direction_;
    glm::vec3 ambient_;
    glm::vec3 diffuse_;
    glm::vec3 specular_;

};