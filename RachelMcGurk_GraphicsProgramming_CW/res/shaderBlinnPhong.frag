#version 330
out vec4 colour;

in vec3 FragPos;
in vec2 texCoord0;
in vec3 Normal;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColour;

uniform sampler2D diffuse;

void main()
{
	//ambient
	float ambientStrength = 0.1f;
	vec3 ambientLight = ambientStrength * lightColour;

	//diffuse
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuseLight = diff * lightColour;

	//specular
	float specularStrength = 0.8f;
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(norm, halfwayDir), 0.0), 16.0);
	vec3 specularLight = specularStrength * spec * lightColour;

	vec3 result = (ambientLight + diffuseLight + specularLight);
	colour = texture2D(diffuse, texCoord0) * vec4(result, 1.0f);
}



