#pragma once
#include "engine/Component/Component.h"
#include "Script.h"

class ScriptLoader : public Component
{
public:
	ScriptLoader(std::shared_ptr<GameObject> obj) : Component(obj) {}
	std::vector<Script*>& getScripts() {
		return scripts;
	}
	void addScript(Script* script) {
		script->setObject(getCurrentObject().get());
		scripts.push_back(script);
	}
private:
	std::vector<Script*> scripts;
};
