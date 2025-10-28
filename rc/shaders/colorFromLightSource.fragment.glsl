#version 460 core

out vec4 FragColor;

uniform vec3 u_Color;
uniform vec3 u_LightSourceColor;

void main()
{
    FragColor = vec4(u_Color * u_LightSourceColor, 1.f);
}
