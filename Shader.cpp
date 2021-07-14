#include "Shader.h"
#include<fstream>
#include<sstream>
#include<string>
#include<GL/glew.h>

Shader::Shader(const std::string path)
	:filepath(path), m_RendererID(0)
{
	ShaderSource source = ParseShader();
	m_RendererID = CreateShader(source.VertexShader, source.FragmentShader);
}

ShaderSource Shader::ParseShader()
{
	std::ifstream stream(this->filepath);
	if (!stream.is_open())
	{
		std::cout << "file '" << filepath << "' is still closed!" << std::endl;
		return ShaderSource();
	}

	std::stringstream ss[2];
	std::string line;

	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	ShaderType type = ShaderType::NONE;

	while (getline(stream, line))
	{
		if (line.find("#shader vertex") != std::string::npos)
			type = ShaderType::VERTEX;
		else if (line.find("#shader fragment") != std::string::npos)
			type = ShaderType::FRAGMENT;
		else
		{
			ss[(int)type] << line << std::endl;
		}
	}

	stream.close();
	return { ss[0].str(), ss[1].str() };
}

unsigned int Shader::CreateShader(std::string& VertexShader, std::string& FragmentShader)
{
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(VertexShader, GL_VERTEX_SHADER);
	unsigned int fs = CompileShader(FragmentShader, GL_FRAGMENT_SHADER);
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	return program;
}

unsigned int Shader::CompileShader(std::string& shaderSource, unsigned int type)
{
	unsigned int shader = glCreateShader(type);
	const char* src = shaderSource.c_str();
	glShaderSource(shader, 1, &src, NULL);
	glCompileShader(shader);

	int result;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		int length;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length);
		glGetShaderInfoLog(shader, length, &length, message);
		std::cout << "Compile Error -> " << (type == GL_VERTEX_SHADER ? "Vertex" : "Fragment") << " Shader" << std::endl;
		std::cout << message << std::endl;
		return 0;
	}
	return shader;
}

void Shader::Bind() const
{
	glUseProgram(m_RendererID);
}

void Shader::Unbind() const
{
	glUseProgram(0);
}

void Shader::SetUniformVec3(const char* name, glm::vec3 vec) const
{
	if (uniformList.find(name) != uniformList.end())
	{
		int location = uniformList[name];
		if(location != -1)
			glUniform3f(location, vec.x, vec.y, vec.z);
	}
	else
	{
		int location = glGetUniformLocation(m_RendererID, name);
		if (location == -1)
		{
			std::cout << "No uniform '" << name << "' found" << std::endl;
			uniformList[name] = location;
		}
		else
		{
			uniformList[name] = location;
			glUniform3f(location, vec.x, vec.y, vec.z);
		}
	}
}

void Shader::SetUniform1i(const char* name, int i0) const
{
	if (uniformList.find(name) != uniformList.end())
	{
		int location = uniformList[name];
		if (location != -1)
			glUniform1i(location, i0);
	}
	else
	{
		int location = glGetUniformLocation(m_RendererID, name);
		if (location == -1)
		{
		//	std::cout << "No uniform '" << name << "' found" << std::endl;
			uniformList[name] = location;
		}
		else
		{
			uniformList[name] = location;
			glUniform1i(location, i0);
		}
	}
}

void Shader::SetUniform1f(const char* name, float f0) const
{
	if (uniformList.find(name) != uniformList.end())
	{
		int location = uniformList[name];
		if (location != -1)
			glUniform1f(location, f0);
	}
	else
	{
		int location = glGetUniformLocation(m_RendererID, name);
		if (location == -1)
		{
			std::cout << "No uniform '" << name << "' found" << std::endl;
			uniformList[name] = location;
		}
		else
		{
			uniformList[name] = location;
			glUniform1f(location, f0);
		}
	}
}

void Shader::SetUniformMatrix4fv(const char* name, glm::mat4 mat) const
{
	if (uniformList.find(name) != uniformList.end())
	{
		int location = uniformList[name];
		if (location != -1)
			glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat));
	}
	else
	{
		int location = glGetUniformLocation(m_RendererID, name);
		if (location == -1)
		{
			std::cout << "No uniform '" << name << "' found" << std::endl;
			uniformList[name] = location;
		}
		else
		{
			uniformList[name] = location;
			glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat));
		}
	}
}
