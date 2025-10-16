#version 460 core

in vec4 vertexColor;
in vec2 textureCoord;

out vec4 FragColor;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float mixAmount;

void main()
{
    FragColor = mix(texture(texture1, textureCoord), texture(texture2, textureCoord), mixAmount);
}
