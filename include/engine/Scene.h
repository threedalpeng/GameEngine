#pragma once
#include <string>
#include <unordered_set>
#include "engine/Object/GameObject.h"

class Scene {
public:
	Scene() {}
	virtual void init();

	void addObject(GameObject* obj) {
		_rootObjects.insert(obj);
	}
	std::unordered_set<GameObject*>& getRootObjects() {
		return _rootObjects;
	}

	std::string vert_shader_path;
	std::string frag_shader_path;

private:
	std::unordered_set<GameObject*> _rootObjects;
};
