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
protected:
	std::shared_ptr<GameObject> getCurrentObject();
private:
	std::weak_ptr<GameObject> _gameObject;
};
