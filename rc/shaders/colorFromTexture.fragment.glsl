#version 460 core

in vec4 vertexColor;
in vec2 textureCoord;

out vec4 FragColor;

uniform sampler2D u_CustomTexture;

void main()
{
    FragColor = texture(u_CustomTexture, textureCoord);
}
