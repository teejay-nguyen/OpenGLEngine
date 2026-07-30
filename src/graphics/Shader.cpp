#include "Shader.h"

#include <iostream>
#include <glad/glad.h>

Shader::Shader()
{

}

Shader::~Shader()
{
    if (m_ID != 0)
        glDeleteProgram(m_ID);
}

bool Shader::Create(const char* vertexShaderSource, const char* fragmentShaderSource)
{
    // Create shader program
    // Create vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(
        vertexShader,
        1,
        &vertexShaderSource,
        nullptr
    );
    
    glCompileShader(vertexShader);

    GLint success;
    glGetShaderiv(
        vertexShader,
        GL_COMPILE_STATUS,
        &success
    );
    
    if (success == GL_FALSE)
    {
        char infoLog[512];
        glGetShaderInfoLog(
            vertexShader,
            512,
            nullptr,
            infoLog
        );

        std::cerr << "Vertex shader compilation failed:\n"
                  << infoLog << std::endl;

        glDeleteShader(vertexShader);

        return false;
    }

    // Create fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(
        fragmentShader,
        1,
        &fragmentShaderSource,
        nullptr
    );
    
    glCompileShader(fragmentShader);

    glGetShaderiv(
        fragmentShader,
        GL_COMPILE_STATUS,
        &success
    );
    
    if (success == GL_FALSE)
    {
        char infoLog[512];
        glGetShaderInfoLog(
            fragmentShader,
            512,
            nullptr,
            infoLog
        );

        std::cerr << "Fragment shader compilation failed:\n"
                  << infoLog << std::endl;

        glDeleteShader(fragmentShader);
        glDeleteShader(vertexShader);

        return false;
    }

    // Create shader program
    m_ID = glCreateProgram();
    glAttachShader(m_ID, vertexShader);
    glAttachShader(m_ID, fragmentShader);
    glLinkProgram(m_ID);

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

        glDeleteShader(fragmentShader);
        glDeleteShader(vertexShader);
        glDeleteProgram(m_ID);
        m_ID = 0;

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