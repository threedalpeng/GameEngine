#pragma once

class ComponentManager;

#include <unordered_map>
#include <typeindex>
#include "engine/Component/ComponentGroup.h"

class ComponentManager
{
public:
	ComponentManager();

	template <typename T>
	ComponentGroup<T>* createComponentList();

	template <typename T>
	std::shared_ptr<T> create(const std::shared_ptr<GameObject>& obj);
	template <typename T>
	std::shared_ptr<T> find(ObjectId id);

	template <typename T>
	typename ComponentGroup<T>::ComponentListIterator begin();
	template <typename T>
	typename ComponentGroup<T>::ComponentListIterator end();
	template <typename T>
	typename ComponentGroup<T>::ComponentListConstIterator cbegin();
	template <typename T>
	typename ComponentGroup<T>::ComponentListConstIterator cend();

	void clear();

private:
	using ComponentGroupList = std::unordered_map<std::type_index, ComponentGroupInterface*>;
	ComponentGroupList _componentGroupList;
};

#include <cassert>
#include "engine/Script/Script.h"

template <typename T>
ComponentGroup<T>* ComponentManager::createComponentList() {
	auto result = _componentGroupList.emplace(typeid(T), new ComponentGroup<T>());
	return reinterpret_cast<ComponentGroup<T> *>((result.first)->second);
}

template<typename T>
std::shared_ptr<T> ComponentManager::create(const std::shared_ptr<GameObject>& obj) {
	auto it = _componentGroupList.find(typeid(T));
	ComponentGroup<T>* componentGroup;
	if (it == _componentGroupList.cend()) { // not found
		componentGroup = createComponentList<T>();
	}
	else {
		componentGroup = reinterpret_cast<ComponentGroup<T> *>(it->second);
	}
	return componentGroup->create(obj);
}

template<typename T>
std::shared_ptr<T> ComponentManager::find(ObjectId id) {
	auto it = _componentGroupList.find(typeid(T));
	if (it == _componentGroupList.cend()) { // not found
		return nullptr;
	}
	ComponentGroup<T>* componentGroup = reinterpret_cast<ComponentGroup<T> *>(it->second);
	return componentGroup->find(id);
}

template <typename T>
typename ComponentGroup<T>::ComponentListIterator ComponentManager::begin() {
	auto it = _componentGroupList.find(typeid(T));
	assert((it == _componentGroupList.cend()));
	ComponentGroup<T>* componentGroup = reinterpret_cast<ComponentGroup<T> *>(it->second);
	return componentGroup->begin();
}

template <typename T>
typename ComponentGroup<T>::ComponentListIterator ComponentManager::end() {
	auto it = _componentGroupList.find(typeid(T));
	assert((it == _componentGroupList.cend()));
	ComponentGroup<T>* componentGroup = reinterpret_cast<ComponentGroup<T> *>(it->second);
	return componentGroup->end();
}

template <typename T>
typename ComponentGroup<T>::ComponentListConstIterator ComponentManager::cbegin() {
	auto it = _componentGroupList.find(typeid(T));
	assert((it == _componentGroupList.cend()));
	ComponentGroup<T>* componentGroup = reinterpret_cast<ComponentGroup<T> *>(it->second);
	return componentGroup->cbegin();
}

template <typename T>
typename ComponentGroup<T>::ComponentListConstIterator ComponentManager::cend() {
	auto it = _componentGroupList.find(typeid(T));
	assert((it == _componentGroupList.cend()));
	ComponentGroup<T>* componentGroup = reinterpret_cast<ComponentGroup<T> *>(it->second);
	return componentGroup->cend();
}