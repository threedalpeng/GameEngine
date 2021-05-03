#pragma once
#include <string>
#include "cg/cgut.h"

class Shader
{
public:
	Shader();
	~Shader();
	void clear();
	bool loadFrom(std::string vertex_shader_path, std::string index_shader_path);
	GLuint getProgram();
	GLint getUniformLocation(std::string uniform_name);

private:
	GLuint program = 0;
};