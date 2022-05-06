/**
 * @file shader.hpp
 * @brief Implementation of shader
 * @author Yuki Ogiwara
 * @date 2022-05-07
 */

#include "shader.hpp"

/**
 * @brief constructor
 * @param[in] vertex_path vertex shader
 * @param[in] fragment_path fragment shader
 */
Shader::Shader(const char* vertex_path, const char* fragment_path) {
    std::string vertex_code, fragment_code;
    std::ifstream vertex_shader_file, fragment_shader_file;

    vertex_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fragment_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    // Read shader files
    try {
        vertex_shader_file.open(vertex_path);
        fragment_shader_file.open(fragment_path);

        std::stringstream vertex_shader_stream, fragment_shader_stream;
        vertex_shader_stream << vertex_shader_file.rdbuf();
        fragment_shader_stream << fragment_shader_file.rdbuf();

        vertex_shader_file.close();
        fragment_shader_file.close();

        vertex_code = vertex_shader_stream.str();
        fragment_code = fragment_shader_stream.str();
    } catch(std::ifstream::failure e) {
        std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }

    const char* vertex_shader_code = vertex_code.c_str();
    const char* fragment_shader_code = fragment_code.c_str();

    // Compile shaders
    unsigned int vertex, fragment;
    int success;
    char info_log[512];

    // Vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertex_shader_code, NULL);
    glCompileShader(vertex);
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(vertex, 512, NULL, info_log);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << info_log << std::endl;
    }

    // Fragment shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragment_shader_code, NULL);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(fragment, 512, NULL, info_log);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << info_log << std::endl;
    }

    // Link shaders
    id_ = glCreateProgram();
    glAttachShader(id_, vertex);
    glAttachShader(id_, fragment);
    glLinkProgram(id_);
    glGetProgramiv(id_, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(id_, 512, NULL, info_log);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << info_log << std::endl;
    }

    // Delete shaders
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

/**
 * @brief destructor
 */
Shader::~Shader() {
    glDeleteProgram(id_);
}

/**
 * @brief use shader program
 */
void Shader::Use() {
    glUseProgram(id_);
}

/**
 * @brief set bool to uniform variable
 * @param[in] name name of uniform
 * @param[in] value
 */
void Shader::SetBool(const std::string &name, bool value) const {
    glUniform1i(glGetUniformLocation(id_, name.c_str()), (int)value);
}

/**
 * @brief set int to uniform variable
 * @param[in] name name of uniform
 * @param[in] value
 */
void Shader::SetInt(const std::string &name, int value) const {
    glUniform1i(glGetUniformLocation(id_, name.c_str()), (int)value);
}

/**
 * @brief set float to uniform variable
 * @param[in] name name of uniform
 * @param[in] value
 */
void Shader::SetFloat(const std::string &name, float value) const {
    glUniform1f(glGetUniformLocation(id_, name.c_str()), (int)value);
}

/**
 * @brief set vec3 to uniform variable
 * @param[in] name name of uniform
 * @param[in] value
 */
void Shader::SetVec3(const std::string &name, const glm::vec3 &value) const {
    glUniform3fv(glGetUniformLocation(id_, name.c_str()), 1, glm::value_ptr(value));
}

/**
 * @brief set mat4 to uniform variable
 * @param[in] name name of uniform
 * @param[in] value
 */
void Shader::SetMat4(const std::string &name, const glm::mat4 &value) const {
    glUniformMatrix4fv(glGetUniformLocation(id_, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}
