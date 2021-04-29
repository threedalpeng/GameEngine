#include "GraphicsManager.h"

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

	for (auto renderer_pair : *_rendererList) {
		Renderer* renderer = renderer_pair.second.get();
		renderer->render(shader);
	}
}