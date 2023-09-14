#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define GLAD_GL_IMPLEMENTATION 
#include <gl.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "TextureAtlasInfo.h"

#define WINDOW_WIDTH 960
#define WINDOW_HEIGHT 640

uint32_t TILE_MAP[15 * 10] = 
{
    0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0
};

typedef struct
{
    uint32_t id;
    uint32_t windowDimensionsLocation;
    uint32_t textureSamplerLocation;
} ShaderProgram;

typedef struct 
{
    uint32_t vao;
    uint32_t vertexCount;    
} QuadMesh;

typedef uint32_t QuadTexture;

typedef struct 
{
    float r;
    float g; 
    float b;
} RGBColor;

static void 
readFullFile(char *fileName, char **fileContent) 
{
    // STUDY(speciial): what would a good api for reading files look like? should I introduce a 
    //                  string type and return a value instead of passing in a pointer to write 
    //                  to? 
    // TODO(speciial): the file contents are never freed
    char *result;
    FILE *file;
    fopen_s(&file, fileName, "rb");
    fseek(file, 0, SEEK_END);
    int64_t fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    result = (char *)malloc(fileSize + 1);
    fread(result, fileSize, 1, file);
    fclose(file);

    result[fileSize] = 0;
    *fileContent = result;
}

static ShaderProgram 
createShaderProgram(char *vertexShaderSource, char *fragmentShaderSource)
{
    uint32_t vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, 0);
    glCompileShader(vertexShader);
    uint32_t fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, 0);
    glCompileShader(fragmentShader);

    int32_t shaderStatus;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &shaderStatus);
    if(!shaderStatus)
    {
        glGetShaderInfoLog(vertexShader, 512, 0, infoLog);
        printf("Failed to compile vertex shader:\n %s", infoLog);
    }

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

    glUseProgram(shaderProgram);
    int32_t windowDimensionsLocation = glGetUniformLocation(shaderProgram, "uWindowDimensions");
    int32_t textureSamplerLocation = glGetUniformLocation(shaderProgram, "uTextureSampler");
    glUseProgram(0);

    ShaderProgram result;
    result.id = shaderProgram;
    result.windowDimensionsLocation = windowDimensionsLocation;
    result.textureSamplerLocation = textureSamplerLocation;
    return result;
}

static QuadMesh 
createQuadMesh(float x, float y, float width, float height, RGBColor color, TextureID texID)
{
    // STUDY(speciial): creating new vaos and drawing them individually in separate draw calls
    //                  is definitely not the most optimal way of rendering 2D quads. It's 
    //                  probably a good idea to batch the render data into a single draw call.
    //                  I could either look into how instancing works, or construct the vertex
    //                  data myself and update it.
    float textureAtalWidth = 640.0f;
    float textureAtalHeight = 640.0f;

    Sprite sprite = getSprite(texID);

    float uvMinX = (float)(sprite.offset.x) / textureAtalWidth;
    float uvMaxX = (float)(sprite.offset.x + sprite.size.x) / textureAtalWidth;
    float uvMinY = (float)(sprite.offset.y) / textureAtalHeight;
    float uvMaxY = (float)(sprite.offset.y + sprite.size.y) / textureAtalHeight;

    float vertices[] = 
    {
                x,           y, 0.0f, color.r, color.g, color.b, uvMinX, uvMinY, // bottom left
        x + width,  y + height, 0.0f, color.r, color.g, color.b, uvMaxX, uvMaxY, // top right
                x,  y + height, 0.0f, color.r, color.g, color.b, uvMinX, uvMaxY, // top left
    
                x,           y, 0.0f, color.r, color.g, color.b, uvMinX, uvMinY, // bottom left
        x + width,           y, 0.0f, color.r, color.g, color.b, uvMaxX, uvMinY, // bottom right
        x + width,  y + height, 0.0f, color.r, color.g, color.b, uvMaxX, uvMaxY  // top right 
    };

    uint32_t vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    uint32_t vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0);

    // color
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

    // uv
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    QuadMesh result;
    result.vao = vao;
    result.vertexCount = 6;
    return result;
}

static void 
drawQuad(ShaderProgram program, QuadMesh mesh, QuadTexture texture)
{
    glUseProgram(program.id);
    glUniform2f(program.windowDimensionsLocation, WINDOW_WIDTH, WINDOW_HEIGHT);
    glUniform1i(program.textureSamplerLocation, 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glBindVertexArray(mesh.vao);
    glDrawArrays(GL_TRIANGLES, 0, mesh.vertexCount);
}

int main()
{
    if(!glfwInit())
    {
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
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

    // compile shader
    char *vertexShaderSource = 0;
    readFullFile("data/shaders/vertex.glsl", &vertexShaderSource);
    char *fragmentShaderSource = 0;
    readFullFile("data/shaders/fragment.glsl", &fragmentShaderSource);

    ShaderProgram shaderProgram = createShaderProgram(vertexShaderSource, fragmentShaderSource);

    // create texture
    QuadTexture texture;
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
    uint8_t *imageData = stbi_load("data/textures/texture_atlas.png", &imageWidth, &imageHeight, &channelCount, 0);
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

    // create quad data
    RGBColor quadColor = {.r = 1.0f, .g = 0.0f, .b = 1.0f};

    QuadMesh *tiles = (QuadMesh*)malloc(15 * 10 * sizeof(QuadMesh));
    for(int32_t y = 0; y < 10; ++y)
    {
        for(int32_t x = 0; x < 15; ++x)
        {
            float xPos = (float)x * 64.0f;
            float yPos = (float)y * 64.0f;
            if(TILE_MAP[(y * 15) + x] == 0) 
            {
                tiles[(y * 15) + x] = createQuadMesh(xPos, yPos, 64.0f, 64.0f, quadColor, GRASS);
            }
            else if(TILE_MAP[(y * 15) + x] == 1)
            {
                tiles[(y * 15) + x] = createQuadMesh(xPos, yPos, 64.0f, 64.0f, quadColor, ROAD);    
            }
        }
    }

    while(!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
    
        glClearColor(0.015f, 0.140f, 0.140f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        for(int32_t tileIndex = 0; tileIndex < 15 * 10; ++tileIndex)
        {
            drawQuad(shaderProgram, tiles[tileIndex], texture);
        }

        glfwSwapBuffers(window);
    }

    glfwTerminate();

    return 0;
}