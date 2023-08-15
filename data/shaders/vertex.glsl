#version 460 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aColor;

uniform vec2 uWindowDimensions;

out vec3 vColor;

void main()
{
    vColor = aColor;

    float vertX = (aPosition.x / (uWindowDimensions.x / 2)) - 1.0f;
    float vertY = (aPosition.y / (uWindowDimensions.y / 2)) - 1.0f;
    gl_Position = vec4(vertX, vertY, aPosition.z, 1.0f);
} 
