#pragma once
#include <string>
#include "engine/Component/Component.h"
#include "engine/Graphics/Mesh.h"
#include "engine/Graphics/Material.h"
#include "engine/Graphics/Shader.h"

class Renderer : public Component
{
public:
	Renderer(std::shared_ptr<GameObject> obj);

	void loadMesh(Mesh* mesh);
	void loadMaterial(Material* material);
	void render(Shader& shader);
private:
	Mesh* _mesh = nullptr;
	Material* _material = nullptr;
};
