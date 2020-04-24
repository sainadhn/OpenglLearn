#include "Shader.h"
#include <fstream>
#include <string>
#include <sstream>
#include <glad\glad.h>
#include "Renderer.h"

Shader::Shader(const std::string& filePath)
	:m_FilePath(filePath), m_RendererID(0)
{
    std::string vertexShader, fragmentShader;
    ParseShader(vertexShader, fragmentShader);
    m_RendererID = CreateShader(vertexShader, fragmentShader);
}
Shader::~Shader()
{
    GLCall(glDeleteProgram(m_RendererID));
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));

        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Compile error  " << ((type == GL_VERTEX_SHADER) ? "Vertex shader" : "Fragment Shader");
        std::cout << message;
    }

    return id;
}


void Shader::ParseShader(std::string& vs, std::string& fs)
{
    std::ifstream   stream(m_FilePath);

    enum class ShaderType
    {
        NONE = -1,
        VERTEX = 0,
        FRAGMENT = 1
    };

    ShaderType  type = ShaderType::NONE;
    std::string line;
    std::stringstream ss[2];

    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;
            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;
        }
        else
        {
            ss[(int)type] << line << '\n';
        }
    }

    vs = ss[0].str();
    fs = ss[1].str();
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);

    glBindAttribLocation(program, 0, "aPos");

    glLinkProgram(program);
    glValidateProgram(program);

    return program;
}

void Shader::Bind() const
{
    GLCall(glUseProgram(m_RendererID));
}
void Shader::Unbind() const
{
    GLCall(glUseProgram(0));
}

void Shader::SetUniform1i(const std::string& name, int v0)
{
    glUniform1i(GetUniformLocation(name), v0);
}
//Set Uniforms
void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    glUniform4f(GetUniformLocation(name), v0, v1, v2, v3);
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix)
{
    GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

int Shader::GetUniformLocation(const std::string& name)
{
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
        return m_UniformLocationCache[name];
    int location;
    GLCall(location = glGetUniformLocation(m_RendererID, name.c_str()));
    if (location == -1)
        std::cout << "Uniform doesn't exist " << std::endl;
    

    m_UniformLocationCache[name] = location;

    return location;
}
