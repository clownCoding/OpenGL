#version 330 core
in vec2 texCoords;
in vec3 Colors;
out vec4 FragColor;

uniform sampler2D texture_diffuse1;

void main()
{
    FragColor = texture(texture_diffuse1, texCoords) * vec4(Colors, 1.0f);
}