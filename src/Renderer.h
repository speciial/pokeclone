#ifndef RENDERER_H
#define RENDERER_H

#include <stdint.h>

#include "Maths.h"

// TODO(speciial): how many triangles?
// TODO(speciial): dynamically allocate space for the triangles?

// NOTE(speciial): since I don't know how much space I need, I'll just copy
//                 what voxel rifts uses and adjust if needed.
#define MAX_TRIANGLES 2048
#define MAX_VERTICES  MAX_TRIANGLES * 3

#define MAX_TEXTURES 8
#define INVALID_TEXTURE_COUNT 3366

typedef struct 
{
    Vec3f position;
    Vec4f color;
    Vec2f uv;
    float textureIndex;
} VertexData;

typedef struct 
{
    uint32_t vao;
    uint32_t vbo;
    uint32_t shaderProgram;

    VertexData vertexData[MAX_VERTICES];
    uint32_t triangleCount;

    uint32_t textures[MAX_TEXTURES];
    uint32_t textureCount;
} RenderData;

void RendererInit(RenderData* renderData, Vec2f windowDimensions);

void RendererBeginFrame(RenderData* renderData);

void RendererPushTriangle(RenderData* renderData,
                          Vec3f v1, Vec3f v2, Vec3f v3,
                          Vec4f color1, Vec4f color2, Vec4f color3,
                          Vec2f uv1, Vec2f uv2, Vec2f uv3,
                          float textureIndex);

void RendererPushColoredQuad(RenderData* renderData, Rect quad, Vec4f color);

void RendererPushTexturedQuad(RenderData* renderData, Rect quad, Vec4f color, uint32_t texture);

void RendererPushSubTexturedQuad(RenderData* renderData, Rect quad, Rect uvQuad, Vec4f color, uint32_t texture);

void RendererEndFrame(RenderData* renderData);

void RendererFree(RenderData* renderData);

void RendererFree(RenderData* renderData);

uint32_t RendererGetWhiteTexture();

uint32_t RendererLoadTexture(char * filePath);

#endif /* RENDERER_H */