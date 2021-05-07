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
	ComponentGroup<T>* getComponentList();

	template <typename T>
	std::shared_ptr<T> create(const std::shared_ptr<GameObject>& obj);
	template <typename T>
	std::shared_ptr<T> find(ObjectId id);

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
ComponentGroup<T>* ComponentManager::getComponentList()
{
	auto it = _componentGroupList.find(typeid(T));
	if (it == _componentGroupList.cend()) {
		return nullptr;
	}
	else {
		return reinterpret_cast<ComponentGroup<T> *>(it->second);
	}
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