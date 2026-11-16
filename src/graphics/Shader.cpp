#include "Shader.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <utility>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

Shader::Shader() = default;

Shader::~Shader()
{
    Release();
}

Shader::Shader(Shader&& other) noexcept
    : m_ID(std::exchange(other.m_ID, 0))
    , m_UniformLocations(std::move(other.m_UniformLocations))
{
}

Shader& Shader::operator=(Shader&& other) noexcept
{
    if (this != &other)
    {
        Release();
        m_ID = std::exchange(other.m_ID, 0);
        m_UniformLocations = std::move(other.m_UniformLocations);
    }

    return *this;
}

void Shader::Release()
{
    if (m_ID != 0)
    {
        glDeleteProgram(m_ID);
        m_ID = 0;
    }

    // cached locations belong to the old program
    m_UniformLocations.clear();
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

    // Allow re-creating a shader without leaking the previous program
    Release();

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

void Shader::SetInt(const std::string& name, int value) const
{
    glProgramUniform1i(m_ID, GetUniformLocation(name), value);
}

void Shader::SetFloat(const std::string& name, float value) const
{
    glProgramUniform1f(m_ID, GetUniformLocation(name), value);
}

void Shader::SetVec3(const std::string& name, const glm::vec3& value) const
{
    glProgramUniform3fv(m_ID, GetUniformLocation(name), 1, glm::value_ptr(value));
}

void Shader::SetVec4(const std::string& name, const glm::vec4& value) const
{
    glProgramUniform4fv(m_ID, GetUniformLocation(name), 1, glm::value_ptr(value));
}

void Shader::SetMat4(const std::string& name, const glm::mat4& value) const
{
    // GLM matrices are column-major like OpenGL's, so no transpose is needed
    glProgramUniformMatrix4fv(m_ID, GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
}

int Shader::GetUniformLocation(const std::string& name) const
{
    auto it = m_UniformLocations.find(name);

    if (it != m_UniformLocations.end())
    {
        return it->second;
    }

    int location = glGetUniformLocation(m_ID, name.c_str());

    // Setting location -1 is a silent no-op in OpenGL, so warn (once, thanks to the cache).
    if (location == -1)
    {
        std::cerr << "Warning: uniform '" << name << "' not found in shader program "
                  << m_ID << " (missing or unused in GLSL)" << std::endl;
    }

    m_UniformLocations.emplace(name, location);
    return location;
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