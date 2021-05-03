#include "GraphicsManager.h"
#include <iostream>

GraphicsManager::GraphicsManager()
{
}

void GraphicsManager::init(Scene* scene)
{
	glEnable(GL_CULL_FACE);									   // turn on backface culling
	glEnable(GL_DEPTH_TEST);								   // turn on depth tests

	_rendererList = ServiceLocator::getService<ComponentManager>()->createComponentList<Renderer>();
	std::cout << "Loading shader..." << std::endl
		<< "- Vertex: " << scene->vert_shader_path << std::endl
		<< "- Fragment: " << scene->frag_shader_path << std::endl;
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