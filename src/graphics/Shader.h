#pragma once

#include <string>

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

private:
    void Release();

    bool CompileShader(
        unsigned int shader,
        const char* source,
        const char* shaderName
    );

    std::string ReadFile(const std::string& path);

private:
    unsigned int m_ID = 0;
};