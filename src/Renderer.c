#include "Renderer.h"

#include <gl.h>
#include <stb_image.h>

#include <stddef.h>
#include <stdbool.h>

#include "System.h"

uint32_t CreateShaderProgram(char *vertexShaderSource, char *fragmentShaderSource)
{
    uint32_t vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, 0);
    glCompileShader(vertexShader);
    
    int32_t shaderStatus;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &shaderStatus);
    if(!shaderStatus)
    {
        glGetShaderInfoLog(vertexShader, 512, 0, infoLog);
        printf("Failed to compile vertex shader:\n %s", infoLog);
    }

    uint32_t fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, 0);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &shaderStatus);
    if(!shaderStatus)
    {
        glGetShaderInfoLog(fragmentShader, 512, 0, infoLog);
        printf("Failed to compile fragment shader:\n %s", infoLog);
    }

    uint32_t shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &shaderStatus);
    if(!shaderStatus) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        printf("Failed to link shader program:\n %s", infoLog);
    }

    glDetachShader(shaderProgram, vertexShader);
    glDetachShader(shaderProgram, fragmentShader);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

void RendererInit(RenderData* renderData, Vec2f windowDimensions)
{
    // TODO(speciial): this implementation of a renderer only allows 1 shader to be bound
    //                 per "instance". In the future I might want to have multiple shaders
    //                 doing different things.
    glGenVertexArrays(1, &renderData->vao);
    glBindVertexArray(renderData->vao);

    glGenBuffers(1, &renderData->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, renderData->vbo);
    glBufferData(GL_ARRAY_BUFFER, MAX_VERTICES * sizeof(VertexData), 0, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void *)offsetof(VertexData, position));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void *)offsetof(VertexData, color));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void *)offsetof(VertexData, uv));
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void *)offsetof(VertexData, textureIndex));


    char *vertexShaderSource = 0;
    vertexShaderSource = ReadFile("data/shaders/vertex.glsl");
    char *fragmentShaderSource = 0;
    fragmentShaderSource = ReadFile("data/shaders/fragment.glsl");
    renderData->shaderProgram = CreateShaderProgram(vertexShaderSource, fragmentShaderSource);
    FreeFile(vertexShaderSource);
    FreeFile(fragmentShaderSource);


    glUseProgram(renderData->shaderProgram);
    int32_t windowDimensionsLocation = glGetUniformLocation(renderData->shaderProgram, "uWindowDimensions");
    glUniform2f(windowDimensionsLocation, windowDimensions.x, windowDimensions.y);
    
    uint32_t textureLocation = glGetUniformLocation(renderData->shaderProgram, "uTexture");
    int32_t textures[8] = { 0, 1, 2, 3, 4, 5, 6, 7 };
    glUniform1iv(textureLocation, 8, textures); 

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    glUseProgram(0);
}

void RendererBeginFrame(RenderData* renderData)
{
    glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    renderData->triangleCount = 0;
    renderData->textureCount = 0;
}

void RendererPushTriangle(RenderData* renderData,
                          Vec3f v1, Vec3f v2, Vec3f v3,
                          Vec4f color1, Vec4f color2, Vec4f color3,
                          Vec2f uv1, Vec2f uv2, Vec2f uv3,
                          float textureIndex)
{
    // STUDY(speciial): I'm not sure if I like this texture setup from VoxelRifts. I might 
    //                  look into other ways of dealing with multiple textures.
    uint32_t tempTextureIndex = INVALID_TEXTURE_COUNT;
    for (uint32_t i = 0; i < renderData->textureCount; i++)
    {
        if (renderData->textures[i] == textureIndex)
        {
            tempTextureIndex = i;
            break;
        }
    }

    if (tempTextureIndex == INVALID_TEXTURE_COUNT && renderData->textureCount < 8)
    {
        renderData->textures[renderData->textureCount] = textureIndex;
        tempTextureIndex = renderData->textureCount;
        renderData->textureCount++;
    }

    // TODO(speciial): replace this with dynamic render batches.
    if (renderData->triangleCount == MAX_TRIANGLES || tempTextureIndex == INVALID_TEXTURE_COUNT)
    {
        RendererEndFrame(renderData);
        RendererBeginFrame(renderData);
    }

    renderData->vertexData[renderData->triangleCount * 3 + 0].position = v1;
    renderData->vertexData[renderData->triangleCount * 3 + 0].color = color1;
    renderData->vertexData[renderData->triangleCount * 3 + 0].uv = uv1;
    renderData->vertexData[renderData->triangleCount * 3 + 0].textureIndex = tempTextureIndex;
    
    renderData->vertexData[renderData->triangleCount * 3 + 1].position = v2;
    renderData->vertexData[renderData->triangleCount * 3 + 1].color = color2;
    renderData->vertexData[renderData->triangleCount * 3 + 1].uv = uv2;
    renderData->vertexData[renderData->triangleCount * 3 + 1].textureIndex = tempTextureIndex;

    renderData->vertexData[renderData->triangleCount * 3 + 2].position = v3;
    renderData->vertexData[renderData->triangleCount * 3 + 2].color = color3;
    renderData->vertexData[renderData->triangleCount * 3 + 2].uv = uv3;
    renderData->vertexData[renderData->triangleCount * 3 + 2].textureIndex = tempTextureIndex;

    renderData->triangleCount++;
}

void RendererPushColoredQuad(RenderData* renderData, Rect quad, Vec4f color)
{
    uint32_t whiteTexture = RendererGetWhiteTexture();
    RendererPushTriangle(renderData,
                         Vec3fInit(quad.x, quad.y, 0.0f), Vec3fInit((quad.x + quad.width), (quad.y + quad.height), 0.0f), Vec3fInit(quad.x, (quad.y + quad.height), 0.0f),
                         color, color, color,
                         Vec2fInit(0, 0), Vec2fInit(1, 1), Vec2fInit(0, 1), whiteTexture);
    RendererPushTriangle(renderData,
                         Vec3fInit(quad.x, quad.y, 0.0f), Vec3fInit((quad.x + quad.width), quad.y, 0.0f), Vec3fInit((quad.x + quad.width), (quad.y + quad.height), 0.0f),
                         color, color, color,
                         Vec2fInit(0, 0), Vec2fInit(1, 1), Vec2fInit(0, 1), whiteTexture);
}

void RendererPushTexturedQuad(RenderData* renderData, Rect quad, Vec4f color, uint32_t texture)
{
    RendererPushTriangle(renderData,
                         Vec3fInit(quad.x, quad.y, 0.0f), Vec3fInit((quad.x + quad.width), (quad.y + quad.height), 0.0f), Vec3fInit(quad.x, (quad.y + quad.height), 0.0f),
                         color, color, color,
                         Vec2fInit(0, 0), Vec2fInit(1, 1), Vec2fInit(0, 1), texture);
    RendererPushTriangle(renderData,
                         Vec3fInit(quad.x, quad.y, 0.0f), Vec3fInit((quad.x + quad.width), quad.y, 0.0f), Vec3fInit((quad.x + quad.width), (quad.y + quad.height), 0.0f),
                         color, color, color,
                         Vec2fInit(0, 0), Vec2fInit(1, 0), Vec2fInit(1, 1), texture);
}

void RendererPushSubTexturedQuad(RenderData* renderData, Rect quad, Rect uvQuad, Vec4f color, uint32_t texture)
{
    RendererPushTriangle(renderData,
                         Vec3fInit(quad.x, quad.y, 0.0f), Vec3fInit((quad.x + quad.width), (quad.y + quad.height), 0.0f), Vec3fInit(quad.x, (quad.y + quad.height), 0.0f),
                         color, color, color,
                         Vec2fInit(uvQuad.x, uvQuad.y), Vec2fInit((uvQuad.x + uvQuad.width), (uvQuad.y + uvQuad.height)), Vec2fInit(uvQuad.x, (uvQuad.y + uvQuad.height)), 
                         texture);
    RendererPushTriangle(renderData,
                         Vec3fInit(quad.x, quad.y, 0.0f), Vec3fInit((quad.x + quad.width), quad.y, 0.0f), Vec3fInit((quad.x + quad.width), (quad.y + quad.height), 0.0f),
                         color, color, color,
                         Vec2fInit(uvQuad.x, uvQuad.y), Vec2fInit((uvQuad.x + uvQuad.width), uvQuad.y), Vec2fInit((uvQuad.x + uvQuad.width), (uvQuad.y + uvQuad.height)),
                         texture);
}

void RendererEndFrame(RenderData* renderData)
{
    for (uint32_t i = 0; i < renderData->textureCount; i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, renderData->textures[i]);
    }

    glBindVertexArray(renderData->vao);
    glBindBuffer(GL_ARRAY_BUFFER, renderData->vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, renderData->triangleCount * 3 * sizeof(VertexData), 
                    renderData->vertexData);
    
    glUseProgram(renderData->shaderProgram);
    glDrawArrays(GL_TRIANGLES, 0, renderData->triangleCount * 3);
}

void RendererFree(RenderData* renderData)
{
    // TODO(speciial): do I have to remove the textures too?

    glDeleteBuffers(1, &renderData->vbo);
    glDeleteVertexArrays(1, &renderData->vao);

    glDeleteProgram(renderData->shaderProgram);
}

// Helper Functions 
uint32_t _CachedWhite = 4096;
uint32_t RendererGetWhiteTexture() 
{
	if (_CachedWhite == 4096) 
    {
		uint32_t texture;
		uint8_t image[4] = { 255, 255, 255, 255 };
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		_CachedWhite = texture;
	}
	return _CachedWhite;
}

uint32_t RendererLoadTexture(char * filePath)
{
    // create texture
    uint32_t texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    stbi_set_flip_vertically_on_load(true);
    int32_t imageWidth;
    int32_t imageHeight;
    int32_t channelCount;
    uint8_t *imageData = stbi_load(filePath, &imageWidth, &imageHeight, &channelCount, 0);
    if(imageData)
    {
        if(channelCount == 3)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
        }
        else if(channelCount == 4)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
        }
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else 
    {
        printf("Failed to load image");
    }
    stbi_image_free(imageData);

    return texture;
}