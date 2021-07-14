#pragma once
#include<vector>
#include<iostream>

class Skybox
{
private:
	unsigned int m_RendererID;
	unsigned int TextureUnit;

public:
	Skybox(std::vector<std::string> faces, unsigned int texUnit);
	~Skybox();

	void Bind() const;
	void Unbind() const;
};
