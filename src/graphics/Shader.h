#pragma once

class Shader
{
public:
    Shader();
    ~Shader();

    bool Create(
        const char* vertexShaderSource,
        const char* fragmentShaderSource
    );

    void Bind() const;

private:
    unsigned int m_ID = 0;

    bool CompileShader(
        unsigned int shader,
        const char* source,
        const char* shaderName
    );
};