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
out vec4 fragPosLightSpace;

const int MAX_BONES = 250;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 gBones[MAX_BONES];
uniform mat4 lightSpaceMatrix;

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
	fragPosLightSpace = lightSpaceMatrix * vec4(fragPos, 1.0);
}

#shader fragment
#version 330 core

struct DirLight {
	//vec3 direction;
	vec3 lightPos;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewPos);
float ShadowCalculation(vec4 fragPosLightSpace);

out vec4 FragColor;
in vec2 texCoords;
in vec3 normal;
in vec3 fragPos;
flat in ivec4 boneIDs;
in vec4 weights;
in vec3 color;
in vec4 fragPosLightSpace;

uniform sampler2D texture_diffuse1;
uniform sampler2D depthMap;
uniform sampler2D texture_specular1;
uniform vec3 viewPos;
uniform DirLight dirLight;
uniform samplerCube skybox;
uniform int mirror;

void main()
{
	vec3 I = normalize(fragPos - viewPos);
	vec3 R = reflect(I, normalize(normal));
	vec3 result;
	if (mirror == 1) {
		result = 0.6 * color * CalcDirLight(dirLight, normal, viewPos) + 0.4 * texture(skybox, R).rgb;
	}
	else {
		result = color * CalcDirLight(dirLight, normal, viewPos);
	}

	FragColor = vec4(result, 1.0f);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewPos) {
	vec3 texColor = vec3(texture(texture_diffuse1, texCoords));
	vec3 ambient = light.ambient;

	//vec3 lightDir = normalize(-light.direction);
	vec3 lightDir = normalize(light.lightPos - fragPos);
	float diff = max(dot(lightDir, normalize(normal)), 0.0f);
	vec3 diffuse =  diff * light.diffuse;

	//vec3 reflectDir = reflect(-lightDir, normalize(normal));
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(normalize(normal), halfwayDir), 0.0), 32.0f);
	vec3 specular = spec * light.specular;

	float shadow = ShadowCalculation(fragPosLightSpace);
	return (ambient + (1.0 - shadow) * (diffuse + specular)) * texColor;
}

float ShadowCalculation(vec4 fragPosLightSpace) {
	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
	projCoords = projCoords * 0.5 + 0.5;
	float closestDepth = texture(depthMap, projCoords.xy).r;
	float currentDepth = projCoords.z;
	float shadow = currentDepth > closestDepth ? 1.0 : 0.0;

	return shadow;
}