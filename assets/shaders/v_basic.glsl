#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormals;
layout(location = 2) in vec2 aUVCoord;

out vec3 Vertices;
out vec3 Normals;
out vec2 UVCoord;

out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
	Vertices = aPos;
	Normals = mat3(transpose(inverse(model))) * aNormals;
	UVCoord = aUVCoord;

	FragPos = vec3(model * vec4(aPos, 1.0f));

	gl_Position = (proj * view * model) * vec4(aPos, 1.0f);
}
