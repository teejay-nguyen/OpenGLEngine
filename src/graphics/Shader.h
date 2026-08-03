#pragma once

#include <string>

class Shader
{
public:
    Shader();
    ~Shader();

    bool Create(
        const std::string& vertexPath,
        const std::string& fragmentPath
    );

    void Bind() const;

private:
    bool CompileShader(
        unsigned int shader,
        const char* source,
        const char* shaderName
    );

    std::string ReadFile(const std::string& path);

private:
    unsigned int m_ID = 0;
};