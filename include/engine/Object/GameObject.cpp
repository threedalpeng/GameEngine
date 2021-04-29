#include "GameObject.h"

GameObject::GameObjectList GameObject::_gameObjectList = GameObjectList();

GameObject::GameObject(std::string name) : Object() {
	setName(name);
	_gameObjectList.insert(std::make_pair(_name, shared_from_this()));
}

GameObject::GameObject(std::string name, const std::shared_ptr<GameObject>& parent) : Object(), _parent(parent) {
	setName(name);
	_gameObjectList.insert(std::make_pair(_name, shared_from_this()));
}

GameObject::~GameObject()
{
}

void GameObject::setParent(std::shared_ptr<GameObject> obj) {
	_parent = obj;
}

void GameObject::addChildren(const std::shared_ptr<GameObject>& obj) {
	_children.insert(obj);
}

void GameObject::removeChildren(const std::shared_ptr<GameObject>& obj) {
	_children.erase(obj);
}

std::string GameObject::name() {
	return _name;
}

void GameObject::setName(std::string name) {
	// trim whitespaces of name
	name = name.erase(name.find_last_not_of(" \t\n") + 1);
	name = name.erase(0, name.find_first_not_of(" \t\n"));

	// append number for preventing duplication
	std::string finalName = name;
	int countName = 0;
	GameObjectList::iterator it;
	while ((it = _gameObjectList.find(finalName)) == _gameObjectList.cend()) {
		finalName = name + " (" + std::to_string(++countName) + ")";
	}
	_name = finalName;
}

std::shared_ptr<GameObject> GameObject::find(std::string name) {
	auto it = _gameObjectList.find(name);
	if (it == _gameObjectList.cend()) { // not found
		return nullptr;
	}
	return it->second;
}