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
    bool isInTransition;
    float transitionTime;
    Vec2f transitionMoveBy;
    Vec2f playerPosition;
    Vec2f relativeWorldPosition;
    Vec2i playerOrientation;

    Vec2f mousePosition;
} GameData;

void updateGame(GameData* gameData, InputData* inputData)
{
    // Player Movement:
    //  - if a button is pressed, start a transition
    //      - if the player has the right orientation, move the background in the opposite direction
    //      - if the player has the wrong orientation, turn the player

    float timeToFinishTransition = 0.2f;

    if (!gameData->isInTransition)
    {
        if (inputData->buttons[KEY_A].isDown)
        {
            // check if the player has the right orientation 
            if (gameData->playerOrientation.x == -1 && gameData->playerOrientation.y == 0)
            {
                // start the transition
                gameData->isInTransition = true;
                gameData->transitionTime = 0.0f;
            }
            else 
            {
                // update player orientation
                // TODO(speciial): there is a small transition happening when the player is turning
                gameData->playerOrientation.x = -1;
                gameData->playerOrientation.y = 0;
            }
        }
        if (inputData->buttons[KEY_S].isDown)
        {
            // check if the player has the right orientation 
            if (gameData->playerOrientation.x == 0 && gameData->playerOrientation.y == -1)
            {
                // start the transition
                gameData->isInTransition = true;
                gameData->transitionTime = 0.0f;
            }
            else 
            {
                // update player orientation
                // TODO(speciial): there is a small transition happening when the player is turning
                gameData->playerOrientation.x = 0;
                gameData->playerOrientation.y = -1;
            }
        }
        if (inputData->buttons[KEY_D].isDown)
        {
            // check if the player has the right orientation 
            if (gameData->playerOrientation.x == 1 && gameData->playerOrientation.y == 0)
            {
                // start the transition
                gameData->isInTransition = true;
                gameData->transitionTime = 0.0f;
            }
            else 
            {
                // update player orientation
                // TODO(speciial): there is a small transition happening when the player is turning
                gameData->playerOrientation.x = 1;
                gameData->playerOrientation.y = 0;
            }
        }
        if (inputData->buttons[KEY_W].isDown)
        {
            // check if the player has the right orientation 
            if (gameData->playerOrientation.x == 0 && gameData->playerOrientation.y == 1)
            {
                // start the transition
                gameData->isInTransition = true;
                gameData->transitionTime = 0.0f;
            }
            else 
            {
                // update player orientation
                // TODO(speciial): there is a small transition happening when the player is turning
                gameData->playerOrientation.x = 0;
                gameData->playerOrientation.y = 1;
            }
        }
    }

    if (gameData->isInTransition)
    {
        if (gameData->transitionTime <= timeToFinishTransition)
        {
            float transitionFactor = 1 / timeToFinishTransition;
            if (gameData->playerOrientation.x != 0)
            {
                float transitionFactor = 1 / timeToFinishTransition;
                gameData->transitionMoveBy.x = -1.0f * gameData->playerOrientation.x * Lerp(0.0f, 64.0f, gameData->transitionTime * transitionFactor); 
            } 
            else if (gameData->playerOrientation.y != 0)
            {
                gameData->transitionMoveBy.y = -1.0f * gameData->playerOrientation.y * Lerp(0.0f, 64.0f, gameData->transitionTime * transitionFactor);
            }
        
            gameData->transitionTime += gameData->deltaTimeInSeconds;
        } 
        else
        {
            // TODO(speciial): potentially correct the final position of the palyer?
            gameData->isInTransition = false;

            // NOTE(speciial): we need to flip the orienation because the world is moving in the 
            //                 opposite direction of the player
            gameData->relativeWorldPosition.x += -1.0f * gameData->playerOrientation.x * 64.0f;
            gameData->relativeWorldPosition.y += -1.0f * gameData->playerOrientation.y * 64.0f;

            gameData->transitionMoveBy.x = 0;
            gameData->transitionMoveBy.y = 0;
        } 
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
    gameData.isInTransition = false;
    gameData.playerPosition = Vec2fInit(64*7, 64*4);
    gameData.playerOrientation.x = 0;
    gameData.playerOrientation.y = 1;

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
                float xPos = (float)x * 64.0f + gameData.relativeWorldPosition.x;
                float yPos = (float)y * 64.0f + gameData.relativeWorldPosition.y;
                if(TILE_MAP[((9 - y) * 15) + x] == 0) 
                {
                    // RendererPushColoredQuad(&renderData, 
                    //                         RectInit(xPos, (WINDOW_HEIGHT - yPos - 64.0f), 64.0f, 64.0f), 
                    //                         blue);
                    RendererPushTexturedQuad(&renderData, 
                                             RectInit(xPos + gameData.transitionMoveBy.x, 
                                                      yPos + gameData.transitionMoveBy.y, 
                                                      64.0f, 64.0f),
                                             white, grassTexture);
                }
                else if(TILE_MAP[((9 - y) * 15) + x] == 1)
                {
                    // RendererPushColoredQuad(&renderData, 
                    //                         RectInit(xPos, (WINDOW_HEIGHT - yPos - 64.0f), 64.0f, 64.0f),
                    //                         red);
                    RendererPushSubTexturedQuad(&renderData, 
                                                RectInit(xPos + gameData.transitionMoveBy.x, 
                                                         yPos + gameData.transitionMoveBy.y, 
                                                         64.0f, 64.0f),
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