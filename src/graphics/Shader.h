#pragma once

#include <glm/glm.hpp>

#include <string>
#include <unordered_map>

class Shader
{
public:
    Shader();
    ~Shader();

    // A Shader owns a GPU program handle: copying would double-delete it, so it is move-only.
    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

    Shader(Shader&& other) noexcept;
    Shader& operator=(Shader&& other) noexcept;

    bool Create(
        const std::string& vertexPath,
        const std::string& fragmentPath
    );

    void Bind() const;

    // Uniform setters. These use glProgramUniform* (OpenGL 4.1+), so they
    // write straight to this program and do not require it to be bound.
    void SetInt(const std::string& name, int value) const;
    void SetFloat(const std::string& name, float value) const;
    void SetVec3(const std::string& name, const glm::vec3& value) const;
    void SetVec4(const std::string& name, const glm::vec4& value) const;
    void SetMat4(const std::string& name, const glm::mat4& value) const;

private:
    void Release();

    bool CompileShader(
        unsigned int shader,
        const char* source,
        const char* shaderName
    );

    std::string ReadFile(const std::string& path);

    // Looks up (and caches) a uniform location; returns -1 if the uniform
    // does not exist or was optimized out by the GLSL compiler.
    int GetUniformLocation(const std::string& name) const;

private:
    unsigned int m_ID = 0;

    // glGetUniformLocation is a string lookup inside the driver; cache the results.
    mutable std::unordered_map<std::string, int> m_UniformLocations;
};
