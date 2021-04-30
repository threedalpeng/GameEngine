#include "Component.h"
#include "engine/Object/GameObject.h"

Component::Component(const std::shared_ptr<GameObject>& obj) : Object(), _gameObject(obj) {}

Component::~Component()
{
}

std::shared_ptr<GameObject> Component::getCurrentObject() {
	return _gameObject.lock();
}