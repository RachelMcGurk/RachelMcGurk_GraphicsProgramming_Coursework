#version 330

// layout qualifiers
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normal;

out vec3 Normal;
out vec2 texCoord0;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 viewProjection;

void main()
{
	texCoord0 = texCoord;
	gl_Position = viewProjection * model * vec4(position, 1.0f);
	FragPos = vec3(model * vec4(position, 1.0f));
	Normal = mat3(transpose(inverse(model))) * normal;
}
