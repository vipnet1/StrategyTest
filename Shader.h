#pragma once

#include<iostream>
#include<map>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct ShaderSource
{
	std::string VertexShader;
	std::string FragmentShader;
};

class Shader
{
private:
	unsigned int m_RendererID;
	const std::string filepath;
	mutable std::map<std::string, int> uniformList;

public:
	Shader(const std::string path);

	void Bind() const;
	void Unbind() const;

	void SetUniformVec3(const char* name, glm::vec3 vec) const;
	void SetUniform1i(const char* name, int i0) const;
	void SetUniform1f(const char* name, float f0) const;
	void SetUniformMatrix4fv(const char* name, glm::mat4 mat) const;

private:
	ShaderSource ParseShader();
	unsigned int CreateShader(std::string& VertexShader, std::string& FragmentShader);
	unsigned int CompileShader(std::string& shaderSource, unsigned int type);
};

