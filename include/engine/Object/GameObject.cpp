#include "GameObject.h"
#include "engine/Transform/Transform.h"
#include "engine/Component/ComponentManager.h"

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
	obj->_thisPtr = obj;
	obj->addComponent<Transform>();
	auto resultPair = _gameObjectList.insert(std::make_pair(obj->name(), obj));
	return resultPair.first->second.get();
}

void GameObject::setParent(const std::shared_ptr<GameObject>& obj) {
	if (!_parent.expired()) {
		_parent.lock()->removeChildren(this);
	}
	_parent = obj;
}

GameObject* GameObject::getParent()
{
	return _parent.lock().get();
}

std::multiset<std::shared_ptr<GameObject>>& GameObject::getChildren()
{
	return _children;
}

void GameObject::addChildren(GameObject* obj) {
	obj->setParent(shared_from_this());
	_children.insert(obj->shared_from_this());
}

void GameObject::removeChildren(GameObject* obj) {
	_children.erase(obj->shared_from_this());
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