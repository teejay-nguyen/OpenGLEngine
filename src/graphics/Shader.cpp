#include "Shader.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <glad/glad.h>

Shader::Shader()
{

}

Shader::~Shader()
{
    if (m_ID != 0)
        glDeleteProgram(m_ID);
}

bool Shader::Create(const std::string& vertexPath, const std::string& fragmentPath)
{
    std::string vertexShaderSource = ReadFile(vertexPath);
    std::string fragmentShaderSource = ReadFile(fragmentPath);

    // Check if shader sources are empty
    if (vertexShaderSource.empty() || fragmentShaderSource.empty())
    {
        return false;
    }

    // Create shader program
    // Create vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

    if (!CompileShader(vertexShader, vertexShaderSource.c_str(), "Vertex"))
    {
        glDeleteShader(vertexShader);
        return false;
    }

    // Create fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    if (!CompileShader(fragmentShader, fragmentShaderSource.c_str(), "Fragment"))
    {
        glDeleteShader(fragmentShader);
        glDeleteShader(vertexShader);
        return false;
    }

    // Create shader program
    m_ID = glCreateProgram();

    glAttachShader(m_ID, vertexShader);
    glAttachShader(m_ID, fragmentShader);

    glLinkProgram(m_ID);

    GLint success;
    glGetProgramiv(
        m_ID,
        GL_LINK_STATUS,
        &success
    );
    
    if (success == GL_FALSE)
    {
        char infoLog[512];
        glGetProgramInfoLog(
            m_ID,
            512,
            nullptr,
            infoLog
        );

        std::cerr << "Shader program linking failed:\n"
                  << infoLog << std::endl;

        glDeleteProgram(m_ID);
        m_ID = 0;

        glDeleteShader(fragmentShader);
        glDeleteShader(vertexShader);

        return false;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return true;
}

void Shader::Bind() const
{
    glUseProgram(m_ID);
}

bool Shader::CompileShader(unsigned int shader, 
                           const char* source, 
                           const char* shaderName)
{
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (success == GL_FALSE)
    {
        char infoLog[512];
        glGetShaderInfoLog(
            shader, 
            512, 
            nullptr, 
            infoLog);

        std::cerr << shaderName << " shader compilation failed:\n"
                  << infoLog << std::endl;

        return false;
    }

    return true;
}

std::string Shader::ReadFile(const std::string& path)
{
    std::ifstream file(path);

    if (!file.is_open())
    {
        std::cerr << "Failed to open " << path << std::endl;
        return "";
    }

    std::stringstream stream;
    stream << file.rdbuf();
    return stream.str();
}