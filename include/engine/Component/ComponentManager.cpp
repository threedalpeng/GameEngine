#include "ComponentManager.h"
#include "engine/Component/Component.h"

ComponentManager::ComponentManager() {}

void ComponentManager::clear() {
	for (auto it : _componentGroupList) {
		it.second->clear();
	}
	_componentGroupList.clear();
}