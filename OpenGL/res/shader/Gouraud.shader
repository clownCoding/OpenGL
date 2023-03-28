#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 aNormal;
//out vec2 v_TexCoord;

out vec3 resColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightModel;
uniform vec3 lightColor;
uniform vec3 objColor;
uniform vec3 lPosition;
uniform vec3 viewPos;

void main() {
	gl_Position = projection * view * model * vec4(position, 1.0);

	vec3 fragPos = vec3(model * vec4(position, 1.0));
	vec3 normal = mat3(transpose(inverse(model))) * aNormal;
	vec3 lightPos = vec3(lightModel * vec4(lPosition, 1.0));

	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * lightColor;

	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(lightPos - fragPos);
	float diff = max(dot(lightDir, norm), 0.0);
	vec3 diffuse = diff * lightColor;

	float specularStrength = 0.5;
	vec3 reflectDir = reflect(-lightDir, norm);
	vec3 viewDir = normalize(viewPos - fragPos);
	float spec = pow(max(dot(reflectDir, viewDir), 0.0), 32);
	vec3 specular = specularStrength * spec * lightColor;

	resColor = (ambient + diffuse + specular) * objColor;
}

#shader fragment
#version 330 core

out vec4 color;
in vec3 resColor;

uniform vec3 viewPos;

void main() {
	
	color = vec4(resColor, 1.0f);
}