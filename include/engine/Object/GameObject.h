#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include <typeindex>

class Component;
class ComponentManager;
#include "Object.h"

class GameObject : public Object, public std::enable_shared_from_this<GameObject>
{
public:
	GameObject(std::string name);
	virtual ~GameObject();

	static GameObject* create(std::string name);
	static GameObject* find(std::string name);

	template <typename T>
	T* getComponent();
	template <typename T>
	std::vector<T*> getComponents();
	template <typename T>
	T* addComponent();

	GameObject* getParent();
	std::multiset<std::shared_ptr<GameObject>>& getChildren();

	void addChildren(GameObject* obj);
	void removeChildren(GameObject* obj);

	std::string name();
	void setName(std::string name);

private:
	using ComponentList = std::unordered_multimap<std::type_index, std::shared_ptr<Component>>;
	using GameObjectList = std::unordered_map<std::string, std::shared_ptr<GameObject>>;

	std::string _name;
	ComponentList _components;
	std::weak_ptr<GameObject> _thisPtr;
	std::weak_ptr<GameObject> _parent;
	std::multiset<std::shared_ptr<GameObject>> _children;
	static GameObjectList _gameObjectList;

	void setParent(const std::shared_ptr<GameObject>& obj);
};

#include "engine/Component/Component.h"
#include "engine/ServiceLocator.h"

template<typename T>
T* GameObject::getComponent() {
	static_assert(std::is_base_of<Component, T>::value, "T must be a Component.");
	auto it = _components.find(typeid(T));
	if (it == _components.cend()) { // not found;
		return nullptr;
	}
	return reinterpret_cast<T*>(it->second.get());
}

template<typename T>
std::vector<T*> GameObject::getComponents()
{
	static_assert(std::is_base_of<Component, T>::value, "T must be a Component.");
	auto itPair = _components.equal_range(typeid(T));
	std::vector<T*> v;
	for (auto it = itPair.first; it != itPair.second; it++) {
		v.push_back(reinterpret_cast<T*>(it->second.get()));
	}
	return v;
}

template<typename T>
T* GameObject::addComponent() {
	static_assert(std::is_base_of<Component, T>::value, "T must be a Component.");

	std::shared_ptr<T> component = ServiceLocator::getService<ComponentManager>()->create<T>(shared_from_this());
	_components.emplace(typeid(T), component);
	return component.get();
}