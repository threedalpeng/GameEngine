#include "Mesh.h"
#include <iostream>

Mesh::Mesh() {
	vertex_list = std::vector<vertex>();
	index_list = std::vector<uint>();
}

Mesh::~Mesh()
{
	if (vertex_buffer) glDeleteBuffers(1, &vertex_buffer);
	if (index_buffer) glDeleteBuffers(1, &index_buffer);
	if (vertex_array) glDeleteVertexArrays(1, &vertex_array);
}

void Mesh::loadMesh(std::string vertex_binary_path, std::string index_binary_path)
{
	/* source code from "cg/cgut.h"*/
	// load vertex buffer
	mem_t v = cg_read_binary(vertex_binary_path.c_str());
	if (v.size % sizeof(vertex)) { printf("%s is not a valid vertex binary file\n", vertex_binary_path.c_str()); return; }
	vertex_list.resize(v.size / sizeof(vertex));
	memcpy((void*)&vertex_list[0], v.ptr, v.size);

	// load index buffer
	mem_t i = cg_read_binary(index_binary_path.c_str());
	if (i.size % sizeof(uint)) { printf("%s is not a valid index binary file\n", index_binary_path.c_str()); return; }
	index_list.resize(v.size / sizeof(uint));
	memcpy((void*)&index_list[0], i.ptr, i.size);

	// release memory
	if (v.ptr) free(v.ptr);
	if (i.ptr) free(i.ptr);

	// create a vertex buffer
	glGenBuffers(1, &vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex) * vertex_list.size(), &vertex_list[0], GL_STATIC_DRAW);

	// create a index buffer
	glGenBuffers(1, &index_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * index_list.size(), &index_list[0], GL_STATIC_DRAW);

	// generate vertex array object, which is mandatory for OpenGL 3.3 and higher
	vertex_array = cg_create_vertex_array(vertex_buffer, index_buffer);
	if (!vertex_array) { printf("%s(): failed to create vertex aray\n", __func__); return; }
}

GLuint Mesh::getVertexArray()
{
	return vertex_array;
}