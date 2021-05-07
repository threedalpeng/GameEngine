#pragma once
#include "engine/Component/Component.h"
#include "engine/Object/GameObject.h"
class ScriptLoader;

class Script
{
public:
	friend ScriptLoader;
	virtual void init();
	virtual void update();
	virtual void fixedUpdate();
protected:
	template <typename T>
	T* getComponent() {
		return gameObject->getComponent<T>();
	}
private:
	void setObject(GameObject* obj) {
		gameObject = obj;
	}
	GameObject* gameObject;
};
