#version 330 core

out vec4 output_color;

in vec3 Vertices;
in vec3 Normals;
in vec2 UVCoord;
in vec4 Colors;

in vec3 FragPos;

uniform vec3 cameraPos;

uniform sampler2D mainTexture;

void main()
{
	vec3 norm = normalize(Normals);

	vec3 lightPos = cameraPos + (4.0f, 16.0f, 8.0f);
	vec3 lightDir = normalize(lightPos - FragPos);

	vec3 ambient = vec3(0.1f);
	vec3 diffuse = vec3(max(dot(norm, lightDir), 0.0));

	vec3 result = vec3(1.0f);

	output_color = vec4(result, 1.0f) * texture(mainTexture, UVCoord) * Colors;
}
