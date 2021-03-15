#pragma once
#include <GLM/common.hpp>

//Copy this struct into your shader and you can create Uniform Buffers that include this type (see example including DirectionalLight)
struct PointLight
{
	//***SAME TYPES SHOULD BE GROUPED TOGETHER***\\
	//VEC4s 
	//SHOULD ALWAYS USE VEC4s (Vec3s get upscaled to Vec4s anyways, using anything less is a waste of memory)
	glm::vec4 _lightPos = glm::vec4(0,10,0,0);
	glm::vec4 _lightCol = glm::vec4(100.0f, 100.0f, 100.0f, 0.f);

	//FLOATS
	float _lightSpecularPow = 1.0f;
};