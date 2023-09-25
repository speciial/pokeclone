#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define GLAD_GL_IMPLEMENTATION 
#include <gl.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "Maths.h"
#include "Input.h"
#include "Renderer.h"

#include "TextureAtlasInfo.h"

#define WINDOW_WIDTH 960
#define WINDOW_HEIGHT 640

uint32_t TILE_MAP[15 * 10] = 
{
    1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1
};

typedef struct 
{
    float deltaTimeInSeconds;

    float playerSpeed;
    Vec2f playerPosition;

    Vec2f mousePosition;
} GameData;

void updateGame(GameData* gameData, InputData* inputData)
{
    Vec2f playerDirection = Vec2fInit(0, 0);
    if (inputData->buttons[KEY_A].isDown)
    {
        playerDirection.x = -1; 
    }
    if (inputData->buttons[KEY_S].isDown)
    {
        playerDirection.y = -1; 
    }
    if (inputData->buttons[KEY_D].isDown)
    {
        playerDirection.x = 1; 
    }
    if (inputData->buttons[KEY_W].isDown)
    {
        playerDirection.y = 1; 
    }

    if (playerDirection.x != 0 || playerDirection.y != 0)
    {
        playerDirection = Vec2fNormalize(playerDirection);
        gameData->playerPosition = Vec2fAdd(gameData->playerPosition, 
                                            Vec2fScale(playerDirection, gameData->playerSpeed * gameData->deltaTimeInSeconds));
    }

    gameData->mousePosition = Vec2fInit(inputData->mousePosition.x, WINDOW_HEIGHT - inputData->mousePosition.y);
}   

int main()
{
    if(!glfwInit())
    {
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Pokeclone", NULL, NULL);
    if(!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    if(!gladLoadGL((GLADloadfunc) glfwGetProcAddress)) 
    {
        printf("Failed to load glad.");
    }
    glfwSwapInterval(1);
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    InputData inputData = {0};
    InputInit(window, &inputData);

    GameData gameData = {0};
    gameData.playerSpeed = 200;
    gameData.playerPosition.x = 64;
    gameData.playerPosition.y = 64;

    RenderData renderData;
    RendererInit(&renderData, Vec2fInit(WINDOW_WIDTH, WINDOW_HEIGHT));

    uint32_t grassTexture = RendererLoadTexture("data/textures/grass_tile.png");
    uint32_t textureAtlas = RendererLoadTexture("data/textures/texture_atlas.png");

    float now = (float)glfwGetTime();
    float lastTime = (float)glfwGetTime();
    
    while(!glfwWindowShouldClose(window))
    {
        updateGame(&gameData, &inputData);
        
        RendererBeginFrame(&renderData);

        Vec4f blue = {0.0f, 0.0f, 1.0f, 1.0f};
        Vec4f red = {1.0f, 0.0f, 0.0f, 1.0f};
        Vec4f white = {1.0f, 1.0f, 1.0f, 1.0f};

        uint32_t whiteTexture = RendererGetWhiteTexture();

        // RendererPushTriangle(&renderData, 
        //                      Vec3fInit(30.0f, 30.0f, 0.0f), Vec3fInit(130.0f, 30.0f, 0.0f), Vec3fInit(80.0f, 130.0f, 0.0f),
        //                      blue, red, blue,
        //                      Vec2fInit(0, 0), Vec2fInit(1, 0), Vec2fInit(1, 1),
        //                      whiteTexture);

        for(int32_t y = 0; y < 10; y++)
        {
            for(int32_t x = 0; x < 15; x++)
            {
                float xPos = (float)x * 64.0f;
                float yPos = (float)y * 64.0f;
                if(TILE_MAP[((9 - y) * 15) + x] == 0) 
                {
                    // RendererPushColoredQuad(&renderData, 
                    //                         RectInit(xPos, (WINDOW_HEIGHT - yPos - 64.0f), 64.0f, 64.0f), 
                    //                         blue);
                    RendererPushTexturedQuad(&renderData, 
                                             RectInit(xPos, yPos, 64.0f, 64.0f),
                                             white, grassTexture);
                }
                else if(TILE_MAP[((9 - y) * 15) + x] == 1)
                {
                    // RendererPushColoredQuad(&renderData, 
                    //                         RectInit(xPos, (WINDOW_HEIGHT - yPos - 64.0f), 64.0f, 64.0f),
                    //                         red);
                    RendererPushSubTexturedQuad(&renderData, 
                                                RectInit(xPos, yPos, 64.0f, 64.0f),
                                                RectInit(0.1f, 0.0f, 0.1f, 0.1f),
                                                white, textureAtlas);    
                }
            }
        }

        RendererPushColoredQuad(&renderData, 
                                RectInit(gameData.playerPosition.x, gameData.playerPosition.y, 64.0f, 64.0f),
                                blue);

        RendererPushColoredQuad(&renderData, 
                                RectInit(gameData.mousePosition.x, gameData.mousePosition.y, 16.0f, 16.0f),
                                white);

        RendererEndFrame(&renderData);
        glfwSwapBuffers(window);
        glfwPollEvents();

        now = (float)glfwGetTime();
        gameData.deltaTimeInSeconds = now - lastTime;
        lastTime = now;
    }

    RendererFree(&renderData);
    glfwTerminate();

    return 0;
}