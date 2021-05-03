#pragma once
#include <string>
#include "engine/Component/Component.h"
#include "engine/Graphics/Mesh.h"
#include "engine/Graphics/Shader.h"

class Renderer : public Component
{
public:
	Renderer(std::shared_ptr<GameObject> obj);

	void loadMesh(Mesh* mesh);
	void render(Shader& shader);
private:
	Mesh* _mesh = nullptr;
};
