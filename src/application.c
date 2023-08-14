#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define GLAD_GL_IMPLEMENTATION 
#include <glad/gl.h>

#include <GLFW/glfw3.h>

#define WINDOW_WIDTH 960
#define WINDOW_HEIGHT 640

static void readFullFile(const char *fileName, char **fileContent) 
{
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

    // TODO(speciial): detach and delete shaders

    // draw quad
    float vertices[] = 
    {
        -0.5f, -0.5f, 0.0f, // bottom left
         0.5f,  0.5f, 0.0f, // top right
        -0.5f,  0.5f, 0.0f, // top left
    
        -0.5f, -0.5f, 0.0f, // bottom left
         0.5f, -0.5f, 0.0f, // bottom right
         0.5f,  0.5f, 0.0f  // top right 
    };

    uint32_t vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    uint32_t vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    while(!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
    
        glClearColor(0.015f, 0.140f, 0.140f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw quad
        glUseProgram(shaderProgram);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glfwSwapBuffers(window);
    }

    glfwTerminate();

    return 0;
}