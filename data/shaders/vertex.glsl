#version 330 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aUV;
layout (location = 3) in float aTextureIndex;

uniform vec2 uWindowDimensions;

out vec4 vColor;
out vec2 vUV;
out float vTextureIndex;

void main()
{
    float vertX = (aPosition.x / (uWindowDimensions.x / 2)) - 1.0;
    float vertY = (aPosition.y / (uWindowDimensions.y / 2)) - 1.0;
    gl_Position = vec4(vertX, vertY, aPosition.z, 1.0);

    vColor = aColor;
    vUV = aUV;
    vTextureIndex = aTextureIndex;
} 
