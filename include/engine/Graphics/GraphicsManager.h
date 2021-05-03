#pragma once

#include "cg/cgut.h"
#include "engine/Scene.h"
#include "engine/Graphics/Shader.h"
#include "engine/Graphics/Renderer.h"
#include "engine/Graphics/Camera.h"
#include "engine/ServiceLocator.h"
#include "engine/Component/ComponentManager.h"

class GraphicsManager
{
public:
	GraphicsManager();

	void init(Scene* scene);
	void render();

private:
	Shader shader;
	ComponentGroup<Renderer>* _rendererList = nullptr;
};
