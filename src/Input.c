#include "Input.h"

void InputInit(GLFWwindow *window)
{
    if (glfwRawMouseMotionSupported())
    {
        glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    }
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    // TODO(speciial): check if the cursor is inside the window or not.
    // TODO(speciial): the mouse position seems to lag behind the actual mouse cursor.

    glfwSetCursorPosCallback(window, MousePositionCallback);
    glfwSetMouseButtonCallback(window, MouseButtonCallback);
    glfwSetKeyCallback(window, KeyInputCallback);

}

void UpdateKey(KeyCode key, int action)
{
    if (action == GLFW_PRESS)
    {
        inputData.buttons[key].isDown = true;
    }
    else if (action == GLFW_RELEASE)
    {
        inputData.buttons[key].isDown = false;
    }
}

void MousePositionCallback(GLFWwindow* window, double xpos, double ypos)
{
    inputData.mousePosition.x = (float)xpos;
    inputData.mousePosition.y = (float)ypos;

    printf("Mouse X: %f, Mouse Y: %f\n", inputData.mousePosition.x, inputData.mousePosition.y);
}

void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    switch (button)
    {
    case GLFW_MOUSE_BUTTON_LEFT:
        {
            UpdateKey(KEY_MOUSE_LEFT, action);
        } break;
    case GLFW_MOUSE_BUTTON_RIGHT:
        {
            UpdateKey(KEY_MOUSE_RIGHT, action);
        } break;
    }
}

void KeyInputCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    switch (key)
    {
    case GLFW_KEY_W:
        {
            UpdateKey(KEY_W, action);
        } break;
    
    case GLFW_KEY_A:
        {
            UpdateKey(KEY_A, action);
        } break;

    case GLFW_KEY_S:
        {
            UpdateKey(KEY_S, action);
        } break;

    case GLFW_KEY_D:
        {
            UpdateKey(KEY_D, action);
        } break;

    default:
        break;
    }
}