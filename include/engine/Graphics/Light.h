#pragma once
#include "engine/Component/Component.h"
#include "engine/Transform/Transform.h"
#include "engine/Graphics/Shader.h"

class Light : public Component
{
public:
	enum class Type {
		Directional,
		Point
	};

	vec4 ambient = vec4(0.2f, 0.2f, 0.2f, 1.0f);
	vec4 diffuse = vec4(0.8f, 0.8f, 0.8f, 1.0f);
	vec4 specular = vec4(1.0f, 1.0f, 1.0f, 1.0f);

	Light(std::shared_ptr<GameObject> obj) : Component(obj) {}

	void setType(Type type) {
		_lightType = type;
	}

	void render(Shader& shader) {
		vec4 position = vec4(getComponent<Transform>()->worldPosition, 0.f);
		glUniform4fv(shader.getUniformLocation("light_position"), 1, position);
		glUniform4fv(shader.getUniformLocation("Ia"), 1, ambient);
		glUniform4fv(shader.getUniformLocation("Id"), 1, diffuse);
		glUniform4fv(shader.getUniformLocation("Is"), 1, specular);
	}

private:
	Type _lightType = Type::Directional;
};
