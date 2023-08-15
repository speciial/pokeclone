#ifndef TEXTURE_ATLAS_INFO_H
#define TEXTURE_ATLAS_INFO_H

#include <stdio.h>

#include "Maths.h"

typedef enum  
{
    GRASS,
    ROAD
} TextureID;

typedef struct 
{
    Vec3i offset;
    Vec3i size;
} Sprite;

static Sprite 
getSprite(TextureID id)
{
    Sprite result = {0};

    switch(id)
    {
        case GRASS:
        {
            result.offset.x = 0;
            result.offset.y = 0;
            result.size.x = 64;
            result.size.y = 64;
        } break;
        case ROAD:
        {
            result.offset.x = 64;
            result.offset.y = 0;
            result.size.x = 64;
            result.size.y = 64;
        } break;
    }

    return result;
}

#endif /* TEXTURE_ATLAS_INFO_H */