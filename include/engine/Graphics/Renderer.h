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

	bool isShaded = true;

	void loadMesh(Mesh* mesh);
	void loadMaterial(Material* material);
	void loadTexture(std::string texturePath, int textureType = 0);
	void render(Shader& shader);
private:
	Mesh* _mesh = nullptr;
	Material* _material = nullptr;
	GLuint textures[3] = { 0, };
};
