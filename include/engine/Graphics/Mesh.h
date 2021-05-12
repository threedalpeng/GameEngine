#pragma once
#include "cg/cgut.h"

class Mesh
{
public:
	Mesh();
	~Mesh();

	void loadMesh(std::string vertex_binary_path, std::string index_binary_path);
	GLuint getVertexArray();

	std::vector<vertex>	vertex_list;
	std::vector<uint>	index_list;
	GLuint	vertex_buffer = 0;
	GLuint	index_buffer = 0;
	GLuint	vertex_array = 0;
};
