#pragma once
#include <string>
#include <set>
#include "engine/Object/GameObject.h"

class Scene {
public:
	Scene() {}
	virtual void init();
	virtual void update();

	void addObject(GameObject* obj) {
		_rootObjects.insert(obj);
	}

	std::string vert_shader_path;
	std::string frag_shader_path;

private:
	std::multiset<GameObject*> _rootObjects;
};
