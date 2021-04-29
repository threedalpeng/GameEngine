#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include <typeindex>
class ComponentManager;
#include "Object.h"
#include "engine/Component/Component.h"

class GameObject : public Object, public std::enable_shared_from_this<GameObject>
{
public:
	GameObject(std::string name);
	GameObject(std::string name, const std::shared_ptr<GameObject>& parent);
	virtual ~GameObject();

	template <typename T>
	std::shared_ptr<T> getComponent();
	template <typename T>
	std::shared_ptr<T> addComponent();

	void setParent(std::shared_ptr<GameObject> obj);

	void addChildren(const std::shared_ptr<GameObject>& obj);
	void removeChildren(const std::shared_ptr<GameObject>& obj);

	std::string name();
	void setName(std::string name);

	static std::shared_ptr<GameObject> find(std::string name);

private:
	using ComponentList = std::unordered_map<std::type_index, std::shared_ptr<Component>>;
	using GameObjectList = std::unordered_map<std::string, std::shared_ptr<GameObject>>;

	std::string _name;
	ComponentList _components;
	std::weak_ptr<GameObject> _parent;
	std::multiset<std::shared_ptr<GameObject>> _children;
	static GameObjectList _gameObjectList;
};

#include "engine/ServiceLocator.h"

template<typename T>
std::shared_ptr<T> GameObject::getComponent() {
	static_assert(std::is_base_of<Component, T>::value, "T must be a Component.");
	auto it = _components.find(typeid(T));
	if (it == _components.cend()) { // not found;
		return nullptr;
	}
	return std::static_pointer_cast<T>(it->second);
}

template<typename T>
std::shared_ptr<T> GameObject::addComponent() {
	static_assert(std::is_base_of<Component, T>::value, "T must be a Component.");
	auto it = _components.find(typeid(T));
	if (it == _components.cend()) { // not found
		std::shared_ptr<T> component = ServiceLocator::getService<ComponentManager>()->create<T>(shared_from_this());
		_components.insert(std::make_pair(typeid(T), component));
		return reinterpret_cast<T*>(component);
	}
	return nullptr;
}