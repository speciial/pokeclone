#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define GLAD_GL_IMPLEMENTATION 
#include <glad/gl.h>

#include <GLFW/glfw3.h>

#define WINDOW_WIDTH 960
#define WINDOW_HEIGHT 640

typedef struct
{
    uint32_t id;
    uint32_t windowDimensionsLocation;
} ShaderProgram;

typedef struct 
{
    uint32_t vao;
    uint32_t vertexCount;    
} QuadMesh;

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
    glUseProgram(0);

    ShaderProgram result;
    result.id = shaderProgram;
    result.windowDimensionsLocation = windowDimensionsLocation;
    return result;
}

static QuadMesh 
createQuadMesh(float x, float y, float width, float height, RGBColor color)
{
    // STUDY(speciial): creating new vaos and drawing them individually in separate draw calls
    //                  is definitely not the most optimal way of rendering 2D quads. It's 
    //                  probably a good idea to batch the render data into a single draw call.
    //                  I could either look into how instancing works, or construct the vertex
    //                  data myself and update it.

    float vertices[] = 
    {
                x,           y, 0.0f, color.r, color.g, color.b, // bottom left
        x + width,  y + height, 0.0f, color.r, color.g, color.b, // top right
                x,  y + height, 0.0f, color.r, color.g, color.b, // top left
    
                x,           y, 0.0f, color.r, color.g, color.b, // bottom left
        x + width,           y, 0.0f, color.r, color.g, color.b, // bottom right
        x + width,  y + height, 0.0f, color.r, color.g, color.b  // top right 
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);

    // color
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    QuadMesh result;
    result.vao = vao;
    result.vertexCount = 6;
    return result;
}

static void 
drawQuad(ShaderProgram program, QuadMesh mesh)
{
    glUseProgram(program.id);
    glUniform2f(program.windowDimensionsLocation, WINDOW_WIDTH, WINDOW_HEIGHT);
    
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
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    // compile shader
    char *vertexShaderSource = 0;
    readFullFile("../data/shaders/vertex.glsl", &vertexShaderSource);
    char *fragmentShaderSource = 0;
    readFullFile("../data/shaders/fragment.glsl", &fragmentShaderSource);

    ShaderProgram shaderProgram = createShaderProgram(vertexShaderSource, fragmentShaderSource);

    // draw quad
    RGBColor quadColor = {.r = 0.3f, .g = 0.4f, .b = 0.8f};
    QuadMesh quad = createQuadMesh(10.0f, 10.0f, 64.0f, 64.0f, quadColor);
    RGBColor anotherQuadColor = {.r = 0.6f, .g = 0.2f, .b = 0.1f};
    QuadMesh anotherQuad = createQuadMesh(WINDOW_WIDTH - 74.0f, WINDOW_HEIGHT - 74.0f, 64.0f, 64.0f, anotherQuadColor);

    while(!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
    
        glClearColor(0.015f, 0.140f, 0.140f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        drawQuad(shaderProgram, quad);
        drawQuad(shaderProgram, anotherQuad);

        glfwSwapBuffers(window);
    }

    glfwTerminate();

    return 0;
}