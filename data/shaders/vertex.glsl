#version 460 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTextureCoords;

uniform vec2 uWindowDimensions;

out vec3 vColor;
out vec2 vTextureCoords;

void main()
{
    float vertX = (aPosition.x / (uWindowDimensions.x / 2)) - 1.0f;
    float vertY = (aPosition.y / (uWindowDimensions.y / 2)) - 1.0f;
    gl_Position = vec4(vertX, vertY, aPosition.z, 1.0f);

    vColor = aColor;
    vTextureCoords = aTextureCoords;
} 
