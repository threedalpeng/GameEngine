#pragma once

#include <memory>
class GameObject;
#include "engine/Object/Object.h"

class Component : public Object
{
public:
	Component() = delete;
	Component(const std::shared_ptr<GameObject>& obj);
	virtual ~Component();

	template <typename T>
	T* getComponent();
	template <typename T>
	std::vector<T*> getComponents();

	std::shared_ptr<GameObject> getCurrentObject();
private:
	std::weak_ptr<GameObject> _gameObject;
};

#include "engine/Object/GameObject.h"

template<typename T>
T* Component::getComponent()
{
	return (_gameObject.lock())->getComponent<T>();
}

template<typename T>
std::vector<T*> Component::getComponents()
{
	return _gameObject.lock()->getComponents<T>();
}
