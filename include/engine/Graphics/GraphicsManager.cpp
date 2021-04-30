#include "GraphicsManager.h"
#include <iostream>

GraphicsManager::GraphicsManager()
{
}

void GraphicsManager::init(Scene* scene)
{
	_rendererList = ServiceLocator::getService<ComponentManager>()->createComponentList<Renderer>();
	shader.loadFrom(scene->vert_shader_path, scene->frag_shader_path);
}

void GraphicsManager::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(shader.getProgram());
	if (_rendererList) {
		for (auto it = _rendererList->cbegin(); it != _rendererList->cend(); it++) {
			Renderer* renderer = (*it).second.get();
			renderer->render(shader);
		}
	}
}