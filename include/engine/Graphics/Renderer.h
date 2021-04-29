#pragma once
#include <string>
#include "engine/Component/Component.h"
#include "engine/Graphics/Mesh.h"
#include "engine/Graphics/Shader.h"

class Renderer : public Component
{
public:
	void loadMesh(std::string vertex_binary_path, std::string index_binary_path);
	void render(Shader& shader);
private:
	Mesh* mesh = nullptr;
};
