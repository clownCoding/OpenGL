#version 330 core
in vec2 texCoords;
in vec3 Colors;
out vec4 FragColor;

void main()
{
    FragColor = vec4(Colors, 1.0f);
}