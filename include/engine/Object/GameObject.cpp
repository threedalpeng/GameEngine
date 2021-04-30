#include "GameObject.h"

GameObject::GameObjectList GameObject::_gameObjectList = GameObjectList();

GameObject::GameObject(std::string name) : Object() {
	setName(name);
}

GameObject::~GameObject()
{
}

GameObject* GameObject::create(std::string name)
{
	std::shared_ptr<GameObject> obj = std::make_shared<GameObject>(name);
	_gameObjectList.insert(std::make_pair(obj->name(), obj));
	return obj.get();
}

void GameObject::setParent(const std::shared_ptr<GameObject>& obj) {
	_parent = obj;
}

void GameObject::addChildren(const std::shared_ptr<GameObject>& obj) {
	obj->setParent(shared_from_this());
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
	while ((it = _gameObjectList.find(finalName)) != _gameObjectList.cend()) { // until not found
		finalName = name + "(" + std::to_string(++countName) + ")";
	}
	_name = finalName;
}

GameObject* GameObject::find(std::string name) {
	auto it = _gameObjectList.find(name);
	if (it == _gameObjectList.cend()) { // not found
		return nullptr;
	}
	return it->second.get();
}