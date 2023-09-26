#ifndef MATHS_H
#define MATHS_H

#include <math.h>

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

// TODO(speciial): should we call Init everywhere or does this have too much of a performance hit?

// NOTE(speciial): should these be moved to a cpp file?
static inline Vec2f Vec2fAdd(Vec2f v1, Vec2f v2) 
{ 
    return Vec2fInit((v1.x + v2.x), (v1.y + v2.y));
}

static inline Vec3f Vec3fAdd(Vec3f v1, Vec3f v2) 
{ 
    return Vec3fInit((v1.x + v2.x), (v1.y + v2.y), (v1.z + v2.z));
}

static inline Vec4f Vec4fAdd(Vec4f v1, Vec4f v2) 
{ 
    return Vec4fInit((v1.x + v2.x), (v1.y + v2.y), (v1.z + v2.z), (v1.w + v2.w));
}

static inline Vec2f Vec2fScale(Vec2f v, float scale) 
{ 
    return Vec2fInit((v.x * scale), (v.y * scale));
}

static inline Vec3f Vec3fScale(Vec3f v, float scale) 
{ 
    return Vec3fInit((v.x * scale), (v.y * scale), (v.z * scale));
}

static inline Vec4f Vec4fScale(Vec4f v, float scale) 
{ 
    return Vec4fInit((v.x * scale), (v.y * scale), (v.z * scale), (v.w * scale));
}

// TODO(speciial): should we check if the vector is 0, so we don't divide by 0? 
static inline Vec2f Vec2fNormalize(Vec2f v) 
{ 
    float magnitude = sqrtf((v.x * v.x) + (v.y * v.y));
    return Vec2fInit((v.x / magnitude), (v.y / magnitude));
}

static inline Vec3f Vec3fNormalize(Vec3f v) 
{ 
    float magnitude = sqrtf((v.x * v.x) + (v.y * v.y) + (v.z * v.z));
    return Vec3fInit((v.x / magnitude), (v.y / magnitude), (v.z / magnitude));
}

static inline Vec4f Vec4fNormalize(Vec4f v) 
{ 
    float magnitude = sqrtf((v.x * v.x) + (v.y * v.y) + (v.z * v.z) + (v.w * v.w));
    return Vec4fInit((v.x / magnitude), (v.y / magnitude), (v.z / magnitude), (v.w / magnitude));
}

static inline float RoundValuef(float v)
{
    return roundf(v);
}

static inline float MinValuef(float v, float min)
{
    return fminf(v, min);
}

static inline float AbsoluteValuef(float v)
{
    return fabsf(v);
}

static inline float Lerp(float v0, float v1, float t) 
{
    return (1 - t) * v0 + t * v1;
}

#endif /* MATHS_H */