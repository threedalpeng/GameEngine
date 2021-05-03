#pragma once
#include <string>
#include "engine/Object/Factory.h"
#include "engine/Transform/Transform.h"
#include "engine/Graphics/Renderer.h"

class DragonFactory : public Factory {
public:
	static void init() {
		mesh->loadMesh(mesh_vertex_path, mesh_index_path);
	}

	static GameObject* create() {
		GameObject* dragon = GameObject::create("dragon");
		dragon->addComponent<Transform>();
		Renderer* renderer = dragon->addComponent<Renderer>();
		renderer->loadMesh(mesh);

		return dragon;
	}
private:
	static const std::string name;
	static const std::string mesh_vertex_path;
	static const std::string mesh_index_path;
	static Mesh* mesh;
};

const std::string DragonFactory::name = "Dragon";
const std::string DragonFactory::mesh_vertex_path = "mesh/dragon.vertex.bin";
const std::string DragonFactory::mesh_index_path = "mesh/dragon.index.bin";
Mesh* DragonFactory::mesh = new Mesh();