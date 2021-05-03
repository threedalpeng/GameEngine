#pragma once

class GameObject;
#include <memory>
#include "engine/Component/ComponentGroupInterface.h"

template <typename T>
class ComponentGroup : public ComponentGroupInterface {
public:
	static_assert(std::is_base_of<Component, T>::value, "T must be a component.");

	ComponentGroup();

	using ComponentListIterator = typename std::unordered_multimap<ObjectId, std::shared_ptr<T>>::iterator;
	using ComponentListConstIterator = typename std::unordered_multimap<ObjectId, std::shared_ptr<T>>::const_iterator;

	std::shared_ptr<T> create(const std::shared_ptr<GameObject>& obj);
	std::shared_ptr<T> find(ObjectId id);
	void remove(ObjectId id);
	void clear();

	ComponentListIterator begin();
	ComponentListIterator end();
	ComponentListConstIterator cbegin();
	ComponentListConstIterator cend();

private:
	std::unordered_multimap<ObjectId, std::shared_ptr<T>> _componentList;
};

#include "engine/Object/GameObject.h"

template<typename T>
ComponentGroup<T>::ComponentGroup() : ComponentGroupInterface() {
	_componentList = std::unordered_multimap<ObjectId, std::shared_ptr<T>>();
}

template<typename T>
std::shared_ptr<T> ComponentGroup<T>::create(const std::shared_ptr<GameObject>& obj) {
	auto it = _componentList.emplace(obj->objectId(), std::make_shared<T>(obj));
	return it->second;
}

template<typename T>
std::shared_ptr<T> ComponentGroup<T>::find(ObjectId id) {
	auto it = _componentList.find(id);
	if (it == _componentList.cend()) { // not found
		return nullptr;
	}
	else {
		return it->second;
	}
}

template<typename T>
void ComponentGroup<T>::remove(ObjectId id) {
	_componentList.erase(id);
}

template<typename T>
void ComponentGroup<T>::clear() {
	_componentList.clear();
}

template<typename T>
typename ComponentGroup<T>::ComponentListIterator ComponentGroup<T>::begin() {
	return _componentList.begin();
}
template<typename T>
typename ComponentGroup<T>::ComponentListIterator ComponentGroup<T>::end() {
	return _componentList.end();
}
template<typename T>
typename ComponentGroup<T>::ComponentListConstIterator ComponentGroup<T>::cbegin() {
	return _componentList.cbegin();
}
template<typename T>
typename ComponentGroup<T>::ComponentListConstIterator ComponentGroup<T>::cend() {
	return _componentList.cend();
}
