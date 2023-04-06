#shader vertex
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;
layout(location = 3) in ivec4 aBoneIDs;
layout(location = 4) in vec4 aWeights;
layout(location = 5) in vec3 aColors;

out vec2 texCoords;
out vec3 normal;
out vec3 fragPos;
flat out ivec4 boneIDs;
out vec4 weights;
out vec3 color;

const int MAX_BONES = 250;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 gBones[MAX_BONES];

void main()
{
	mat4 BoneTransform = gBones[aBoneIDs[0]] * aWeights[0];
	BoneTransform += gBones[aBoneIDs[1]] * aWeights[1];
	BoneTransform += gBones[aBoneIDs[2]] * aWeights[2];
	BoneTransform += gBones[aBoneIDs[3]] * aWeights[3];

	vec4 PosL = BoneTransform * vec4(aPos, 1.0);
	gl_Position = projection * view * model * PosL;

	texCoords = aTexCoords;
	fragPos = vec3(model * BoneTransform * vec4(aPos, 1.0));
	normal = mat3(transpose(inverse(model))) * aNormal;
	boneIDs = aBoneIDs;
	weights = aWeights;
	color = aColors;
}

#shader fragment
#version 330 core

struct DirLight {
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewPos);

out vec4 FragColor;
in vec2 texCoords;
in vec3 normal;
in vec3 fragPos;
flat in ivec4 boneIDs;
in vec4 weights;
in vec3 color;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform vec3 viewPos;
uniform DirLight dirLight;

void main()
{
	vec3 result;
	result = color * CalcDirLight(dirLight, normal, viewPos);
	FragColor = vec4(result, 1.0f);
	//FragColor = vec4(1.0f, 0.5f, 0.5f, 1.0f);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewPos) {
	vec3 ambient = vec3(texture(texture_diffuse1, texCoords)) * light.ambient;

	vec3 lightDir = normalize(-light.direction);
	float diff = max(dot(lightDir, normalize(normal)), 0.0f);
	vec3 diffuse = vec3(texture(texture_diffuse1, texCoords)) * diff * light.diffuse;

	vec3 reflectDir = reflect(-lightDir, normalize(normal));
	vec3 viewDir = normalize(viewPos - fragPos);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0f);
	vec3 specular = vec3(texture(texture_specular1, texCoords)) * spec * light.specular;

	return ambient + diffuse + specular;
}