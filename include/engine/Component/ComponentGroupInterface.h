#pragma once
#include "engine/Object/Object.h"

class ComponentGroupInterface {
public:
	virtual void remove(ObjectId id) = 0;
	virtual void clear() = 0;
protected:
	ComponentGroupInterface();
};