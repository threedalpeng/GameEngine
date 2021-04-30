#pragma once
#include "engine/Object/GameObject.h"
class Factory
{
public:
	virtual GameObject* create() = 0;
	size_t countCreatedObject() {
		return _objectCount;
	};
private:
	size_t _objectCount = 0;
};
