#ifndef INPUT_H
#define INPUT_H

#include <GLFW/glfw3.h>
#include <stdbool.h>

#include "Maths.h"

typedef enum 
{
    KEY_W,
    KEY_A,
    KEY_S,
    KEY_D,
    KEY_MOUSE_LEFT,
    KEY_MOUSE_RIGHT
} KeyCode;

typedef struct 
{
    bool isDown;
} Button;

typedef struct 
{
    Vec2f mousePosition;
    Button buttons[6];
} InputData;

InputData inputData;

static inline InputData GetInputData() { return inputData; }

void InputInit(GLFWwindow *window);

void UpdateKey(KeyCode key, int action);

void MousePositionCallback(GLFWwindow* window, double xpos, double ypos);

void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

void KeyInputCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

#endif /* INPUT_H */