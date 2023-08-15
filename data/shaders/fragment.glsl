#version 460 core

in vec3 vColor;
in vec2 vTextureCoords;

uniform sampler2D uTextureSampler;

out vec4 fragmentColor;

void main()
{
    fragmentColor = texture(uTextureSampler, vTextureCoords) * vec4(vColor, 1.0f);
}
