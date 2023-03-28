#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 aNormal;

out vec3 fragPos;
out vec3 normal;
out vec3 lightPos;

uniform vec3 lPosition;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
	gl_Position = projection * view * model * vec4(position, 1.0);

	fragPos = vec3(model * vec4(position, 1.0));
	normal = mat3(transpose(inverse(model))) * aNormal;
	lightPos = vec3(vec4(lPosition, 1.0));
}

#shader fragment
#version 330 core

out vec4 color;
in vec3 fragPos;
in vec3 normal;
in vec3 lightPos;

uniform vec3 lightColor;
uniform vec3 objColor;
uniform vec3 viewPos;

void main() {
	float ambientStrength = 0.3;
	vec3 ambient = ambientStrength * lightColor;

	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(lightPos -fragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	float specularStrength = 0.5;
	vec3 reflectDir = reflect(-lightDir, norm);
	vec3 viewDir = normalize(viewPos - fragPos);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 128);
	vec3 specular = specularStrength * spec * lightColor;

	vec3 result = (ambient + diffuse + specular) * objColor;
	color = vec4(result, 1.0f);
}