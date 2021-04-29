#include "Shader.h"

Shader::Shader() {}

Shader::~Shader() {
	clear();
}

void Shader::clear()
{
	if (program) {
		glDeleteProgram(program);
		program = NULL;
	}
}

bool Shader::loadFrom(std::string vertex_shader_path, std::string index_shader_path)
{
	// TODO: Implement with my own code
	program = cg_create_program(vertex_shader_path.c_str(), index_shader_path.c_str());
	return (program ? true : false);
}

GLuint Shader::getProgram()
{
	if (program)
		return program;
	else
		return 0;
}

GLint Shader::getUniformLocation(std::string uniform_name)
{
	if (program) {
		return glGetUniformLocation(program, uniform_name.c_str());
	}
	else {
		return 0;
	}
}