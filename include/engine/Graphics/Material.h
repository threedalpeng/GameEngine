#pragma once
#include "cg/cgmath.h"

class Material
{
public:
	vec4 ambient = vec4(0.2f, 0.2f, 0.2f, 1.0f);
	vec4 diffuse = vec4(0.8f, 0.8f, 0.8f, 1.0f);
	vec4 specular = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	float shininess = 1000.f;
};
