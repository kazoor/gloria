#pragma once
#include <GLFW/glfw3.h>

#define GLORIA_MOUSE_BUTTON_1         0
#define GLORIA_MOUSE_BUTTON_2         1
#define GLORIA_MOUSE_BUTTON_3         2
#define GLORIA_MOUSE_BUTTON_4         3
#define GLORIA_MOUSE_BUTTON_5         4
#define GLORIA_MOUSE_BUTTON_6         5
#define GLORIA_MOUSE_BUTTON_7         6
#define GLORIA_MOUSE_BUTTON_8         7
#define GLORIA_MOUSE_BUTTON_LAST      GLFW_MOUSE_BUTTON_8
#define GLORIA_MOUSE_BUTTON_LEFT      GLFW_MOUSE_BUTTON_1
#define GLORIA_MOUSE_BUTTON_RIGHT     GLFW_MOUSE_BUTTON_2
#define GLORIA_MOUSE_BUTTON_MIDDLE    GLFW_MOUSE_BUTTON_3

enum MouseButton {
    BUTTON_1 = GLORIA_MOUSE_BUTTON_1,
    BUTTON_2 = GLORIA_MOUSE_BUTTON_2,
    BUTTON_3 = GLORIA_MOUSE_BUTTON_3,
    BUTTON_4 = GLORIA_MOUSE_BUTTON_4,
    BUTTON_5 = GLORIA_MOUSE_BUTTON_5,
    BUTTON_6 = GLORIA_MOUSE_BUTTON_6,
    BUTTON_7 = GLORIA_MOUSE_BUTTON_7,
    BUTTON_8 = GLORIA_MOUSE_BUTTON_8,

    BUTTON_LAST = GLORIA_MOUSE_BUTTON_LAST,
    BUTTON_LEFT = GLORIA_MOUSE_BUTTON_LEFT,
    BUTTON_RIGHT = GLORIA_MOUSE_BUTTON_RIGHT,
    BUTTON_MIDDLE = GLORIA_MOUSE_BUTTON_MIDDLE
};