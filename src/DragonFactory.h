#pragma once
#include <string>
#include "engine/Object/Factory.h"
#include "engine/Transform/Transform.h"

class DragonFactory : public Factory {
public:
	static GameObject* create() {
		GameObject* dragon = GameObject::create("dragon");
		dragon->addComponent<Transform>();
		//Renderer* renderer = dragon->addComponent<Renderer>();

		return dragon;
	}
private:
	static const std::string name;
	static const std::string mesh_vertex_path;
	static const std::string mesh_index_path;
};

const std::string DragonFactory::name = "Dragon";
const std::string DragonFactory::mesh_vertex_path = "mesh/dragon.vertex.bin";
const std::string DragonFactory::mesh_index_path = "mesh/dragon.index.bin";