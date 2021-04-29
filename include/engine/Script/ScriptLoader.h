#pragma once
#include "engine/Component/Component.h"

class ScriptLoader : public Component
{
public:
	ScriptLoader(GameObject* obj) : Component(obj) {}
	void loadScript() {
	}
private:
};
