#include "Renderer.h"
#include "engine/Transform/Transform.h"
#include "engine/Object/GameObject.h"

void Renderer::loadMesh(std::string vertex_binary_path, std::string index_binary_path)
{
	mesh->loadMesh(vertex_binary_path, index_binary_path);
}

void Renderer::render(Shader& shader)
{
	if (mesh && mesh->getVertexArray())
		glBindVertexArray(mesh->getVertexArray());

	Transform* transform = getCurrentObject()->getComponent<Transform>();
	mat4 model_matrix = transform->getModelMatrix();

	glUniformMatrix4fv(shader.getUniformLocation("model_matrix"), 1, GL_TRUE, model_matrix);
	glDrawElements(GL_TRIANGLES, mesh->index_list.size(), GL_UNSIGNED_INT, nullptr);
}