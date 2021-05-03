#include "Renderer.h"
#include "engine/Transform/Transform.h"
#include "engine/Object/GameObject.h"

Renderer::Renderer(std::shared_ptr<GameObject> obj) : Component(obj) {}

void Renderer::loadMesh(Mesh* mesh)
{
	_mesh = mesh;
}

void Renderer::render(Shader& shader)
{
	if (_mesh && _mesh->getVertexArray())
		glBindVertexArray(_mesh->getVertexArray());

	Transform* transform = getComponent<Transform>();
	mat4 model_matrix = transform->getModelMatrix();

	glUniformMatrix4fv(shader.getUniformLocation("model_matrix"), 1, GL_TRUE, model_matrix);
	glDrawElements(GL_TRIANGLES, _mesh->index_list.size(), GL_UNSIGNED_INT, nullptr);
}