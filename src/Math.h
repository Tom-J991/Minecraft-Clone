#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct Vector2F
{
	union
	{
		struct { float x; float y; };
		float v[2];
	};

	operator glm::vec2() const { return glm::vec2(x, y); }
};

struct Vector3F
{
	union
	{
		struct { float x; float y; float z; };
		float v[3];
	};

	operator glm::vec3() const { return glm::vec3(x, y, z); }
};
