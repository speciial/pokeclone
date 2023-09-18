#ifndef MATHS_H
#define MATHS_H

#include <stdint.h>

typedef struct 
{
    int32_t x;
    int32_t y;
} Vec2i;

typedef struct 
{
    int32_t x;
    int32_t y;
    int32_t z;
} Vec3i;

typedef struct 
{
    float x;
    float y;
} Vec2f;

typedef struct 
{
    float x;
    float y;
    float z;
} Vec3f;

typedef struct 
{
    float x;
    float y;
    float z;
    float w;
} Vec4f;

typedef struct 
{
    float x;
    float y;
    float width;
    float height;
} Rect;

static inline Vec2f Vec2fInit(float x, float y) { return (Vec2f){x, y}; }
static inline Vec3f Vec3fInit(float x, float y, float z) { return (Vec3f){x, y, z}; }
static inline Vec4f Vec4fInit(float x, float y, float z, float w) { return (Vec4f){x, y, z, w}; }

static inline Rect RectInit(float x, float y, float width, float height) { return (Rect){x, y, width, height}; }

#endif /* MATHS_H */