#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 aNormal;

out vec3 fragPos;
out vec3 normal;
out vec2 texCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
	gl_Position = projection * view * model * vec4(position, 1.0);

	fragPos = vec3(model * vec4(position, 1.0));
	normal = mat3(transpose(inverse(model))) * aNormal;
	texCoords = texCoord;
}

#shader fragment
#version 330 core

#define NR_POINT_LIGHTS 4

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct DirLight {
	vec3 direction;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
struct PointLight {
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constantK;
	float linearK;
	float quadraticK;
};
struct SpotLight {
	vec3 direction;
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float cutOff;
	float outCutOff;
};

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewPos);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewPos);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewPos);

out vec4 color;
in vec3 fragPos;
in vec3 normal;
in vec2 texCoords;

uniform Material material;
uniform vec3 viewPos;
uniform DirLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLight;

void main() {
	vec3 result;
	result = CalcDirLight(dirLight, normal, viewPos);

	for (int i = 0; i < NR_POINT_LIGHTS; i++)
		result += CalcPointLight(pointLights[i], normal, fragPos, viewPos);

	result += CalcSpotLight(spotLight, normal, fragPos, viewPos);

	color = vec4(result, 1.0f);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewPos) {
	vec3 ambient = vec3(texture(material.diffuse, texCoords)) * light.ambient;

	vec3 lightDir = normalize(-light.direction);
	float diff = max(dot(lightDir, normalize(normal)), 0.0f);
	vec3 diffuse = vec3(texture(material.diffuse, texCoords)) * diff * light.diffuse;

	vec3 reflectDir = reflect(-lightDir, normal);
	vec3 viewDir = normalize(viewPos - fragPos);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = vec3(texture(material.specular, texCoords)) * spec * light.specular;

	return ambient + diffuse + specular;
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewPos) {
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, texCoords));

	vec3 lightDir = normalize((light.position - fragPos));
	float diff = max(dot(lightDir, normalize(normal)), 0.0);
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, texCoords));

	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * spec * vec3(texture(material.specular, texCoords));

	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constantK + light.linearK * distance + light.quadraticK * distance * distance);

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return ambient + diffuse + specular;
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewPos) {
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, texCoords));

	vec3 lightDir = normalize((light.position - fragPos));
	float diff = max(dot(lightDir, normalize(normal)), 0.0);
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, texCoords));

	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * spec * vec3(texture(material.specular, texCoords));

	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.cutOff - light.outCutOff;
	float intensity = clamp((theta - light.outCutOff) / epsilon, 0.0, 1.0);

	vec3 result;
	if (theta > light.outCutOff)
		result = (ambient + diffuse + specular) * intensity;
	else
		result = ambient;
	return result;
}