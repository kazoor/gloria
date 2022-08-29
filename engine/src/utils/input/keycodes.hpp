#pragma once

/* Printable keys */
#define GLORIA_KEY_SPACE              32
#define GLORIA_KEY_APOSTROPHE         39  /* ' */
#define GLORIA_KEY_COMMA              44  /* , */
#define GLORIA_KEY_MINUS              45  /* - */
#define GLORIA_KEY_PERIOD             46  /* . */
#define GLORIA_KEY_SLASH              47  /* / */
#define GLORIA_KEY_0                  48
#define GLORIA_KEY_1                  49
#define GLORIA_KEY_2                  50
#define GLORIA_KEY_3                  51
#define GLORIA_KEY_4                  52
#define GLORIA_KEY_5                  53
#define GLORIA_KEY_6                  54
#define GLORIA_KEY_7                  55
#define GLORIA_KEY_8                  56
#define GLORIA_KEY_9                  57
#define GLORIA_KEY_SEMICOLON          59  /* ; */
#define GLORIA_KEY_EQUAL              61  /* = */
#define GLORIA_KEY_A                  65
#define GLORIA_KEY_B                  66
#define GLORIA_KEY_C                  67
#define GLORIA_KEY_D                  68
#define GLORIA_KEY_E                  69
#define GLORIA_KEY_F                  70
#define GLORIA_KEY_G                  71
#define GLORIA_KEY_H                  72
#define GLORIA_KEY_I                  73
#define GLORIA_KEY_J                  74
#define GLORIA_KEY_K                  75
#define GLORIA_KEY_L                  76
#define GLORIA_KEY_M                  77
#define GLORIA_KEY_N                  78
#define GLORIA_KEY_O                  79
#define GLORIA_KEY_P                  80
#define GLORIA_KEY_Q                  81
#define GLORIA_KEY_R                  82
#define GLORIA_KEY_S                  83
#define GLORIA_KEY_T                  84
#define GLORIA_KEY_U                  85
#define GLORIA_KEY_V                  86
#define GLORIA_KEY_W                  87
#define GLORIA_KEY_X                  88
#define GLORIA_KEY_Y                  89
#define GLORIA_KEY_Z                  90
#define GLORIA_KEY_LEFT_BRACKET       91  /* [ */
#define GLORIA_KEY_BACKSLASH          92  /* \ */
#define GLORIA_KEY_RIGHT_BRACKET      93  /* ] */
#define GLORIA_KEY_GRAVE_ACCENT       96  /* ` */
#define GLORIA_KEY_WORLD_1            161 /* non-US #1 */
#define GLORIA_KEY_WORLD_2            162 /* non-US #2 */

/* Funct keys */
#define GLORIA_KEY_ESCAPE             256
#define GLORIA_KEY_ENTER              257
#define GLORIA_KEY_TAB                258
#define GLORIA_KEY_BACKSPACE          259
#define GLORIA_KEY_INSERT             260
#define GLORIA_KEY_DELETE             261
#define GLORIA_KEY_RIGHT              262
#define GLORIA_KEY_LEFT               263
#define GLORIA_KEY_DOWN               264
#define GLORIA_KEY_UP                 265
#define GLORIA_KEY_PAGE_UP            266
#define GLORIA_KEY_PAGE_DOWN          267
#define GLORIA_KEY_HOME               268
#define GLORIA_KEY_END                269
#define GLORIA_KEY_CAPS_LOCK          280
#define GLORIA_KEY_SCROLL_LOCK        281
#define GLORIA_KEY_NUM_LOCK           282
#define GLORIA_KEY_PRINT_SCREEN       283
#define GLORIA_KEY_PAUSE              284
#define GLORIA_KEY_F1                 290
#define GLORIA_KEY_F2                 291
#define GLORIA_KEY_F3                 292
#define GLORIA_KEY_F4                 293
#define GLORIA_KEY_F5                 294
#define GLORIA_KEY_F6                 295
#define GLORIA_KEY_F7                 296
#define GLORIA_KEY_F8                 297
#define GLORIA_KEY_F9                 298
#define GLORIA_KEY_F10                299
#define GLORIA_KEY_F11                300
#define GLORIA_KEY_F12                301
#define GLORIA_KEY_F13                302
#define GLORIA_KEY_F14                303
#define GLORIA_KEY_F15                304
#define GLORIA_KEY_F16                305
#define GLORIA_KEY_F17                306
#define GLORIA_KEY_F18                307
#define GLORIA_KEY_F19                308
#define GLORIA_KEY_F20                309
#define GLORIA_KEY_F21                310
#define GLORIA_KEY_F22                311
#define GLORIA_KEY_F23                312
#define GLORIA_KEY_F24                313
#define GLORIA_KEY_F25                314
#define GLORIA_KEY_KP_0               320
#define GLORIA_KEY_KP_1               321
#define GLORIA_KEY_KP_2               322
#define GLORIA_KEY_KP_3               323
#define GLORIA_KEY_KP_4               324
#define GLORIA_KEY_KP_5               325
#define GLORIA_KEY_KP_6               326
#define GLORIA_KEY_KP_7               327
#define GLORIA_KEY_KP_8               328
#define GLORIA_KEY_KP_9               329
#define GLORIA_KEY_KP_DECIMAL         330
#define GLORIA_KEY_KP_DIVIDE          331
#define GLORIA_KEY_KP_MULTIPLY        332
#define GLORIA_KEY_KP_SUBTRACT        333
#define GLORIA_KEY_KP_ADD             334
#define GLORIA_KEY_KP_ENTER           335
#define GLORIA_KEY_KP_EQUAL           336
#define GLORIA_KEY_LEFT_SHIFT         340
#define GLORIA_KEY_LEFT_CONTROL       341
#define GLORIA_KEY_LEFT_ALT           342
#define GLORIA_KEY_LEFT_SUPER         343
#define GLORIA_KEY_RIGHT_SHIFT        344
#define GLORIA_KEY_RIGHT_CONTROL      345
#define GLORIA_KEY_RIGHT_ALT          346
#define GLORIA_KEY_RIGHT_SUPER        347
#define GLORIA_KEY_MENU               348

enum Key {
    /* PRINTABLE KEYS */
    SPACE = GLORIA_KEY_SPACE,
    APOSTROPHE = GLORIA_KEY_APOSTROPHE, /* ' */
    COMMA = GLORIA_KEY_COMMA, /* , */
    MINUS = GLORIA_KEY_MINUS, /* - */
    PERIOD = GLORIA_KEY_PERIOD, /* . */
    SLASH = GLORIA_KEY_SLASH, /* / */
    SEMICOLON = GLORIA_KEY_SEMICOLON,
    NUM_0 = GLORIA_KEY_0,
    NUM_1 = GLORIA_KEY_1,
    NUM_2 = GLORIA_KEY_2,
    NUM_3 = GLORIA_KEY_3,
    NUM_4 = GLORIA_KEY_4,
    NUM_5 = GLORIA_KEY_5,
    NUM_6 = GLORIA_KEY_6,
    NUM_7 = GLORIA_KEY_7,
    NUM_8 = GLORIA_KEY_8,
    NUM_9 = GLORIA_KEY_9,
    A = GLORIA_KEY_A,
    B = GLORIA_KEY_B,
    C = GLORIA_KEY_C,
    D = GLORIA_KEY_D,
    E = GLORIA_KEY_E,
    F = GLORIA_KEY_F,
    G = GLORIA_KEY_G,
    H = GLORIA_KEY_H,
    I = GLORIA_KEY_I,
    J = GLORIA_KEY_J,
    K = GLORIA_KEY_K,
    L = GLORIA_KEY_L,
    M = GLORIA_KEY_M,
    N = GLORIA_KEY_N,
    O = GLORIA_KEY_O,
    P = GLORIA_KEY_P,
    Q = GLORIA_KEY_Q,
    R = GLORIA_KEY_R,
    S = GLORIA_KEY_S,
    T = GLORIA_KEY_T,
    U = GLORIA_KEY_U,
    V = GLORIA_KEY_V,
    W = GLORIA_KEY_W,
    X = GLORIA_KEY_X,
    Y = GLORIA_KEY_Y,
    Z = GLORIA_KEY_Z,
    LEFT_BRACKET  = GLORIA_KEY_LEFT_BRACKET, /* [ */
    RIGHT_BRACKET  = GLORIA_KEY_RIGHT_BRACKET, /* ] */
    BACKSLASH  = GLORIA_KEY_BACKSLASH, /* \ */
    ACCENT  = GLORIA_KEY_GRAVE_ACCENT, /* ` */
    WORLD_1  = GLORIA_KEY_WORLD_1, /* non-US #1 */
    WORLD_2  = GLORIA_KEY_WORLD_2, /* non-US #2 */
    /* PRINTABLE KEYS */

    /* FUNCTION KEYS */
    ESCAPE = GLORIA_KEY_ESCAPE,
    ENTER = GLORIA_KEY_ENTER,
    TAB = GLORIA_KEY_TAB,
    BACKSPACE = GLORIA_KEY_BACKSPACE,
    INSERT = GLORIA_KEY_INSERT,
    DELETE = GLORIA_KEY_DELETE,
    RIGHT_ARROW = GLORIA_KEY_RIGHT,
    LEFT_ARROW = GLORIA_KEY_LEFT,
    DOWN_ARROW = GLORIA_KEY_DOWN,
    UP_ARROW = GLORIA_KEY_UP,
    PAGE_UP = GLORIA_KEY_PAGE_UP,
    PAGE_DOWN = GLORIA_KEY_PAGE_DOWN,
    HOME = GLORIA_KEY_HOME,
    END = GLORIA_KEY_END,
    CAPSLOCK = GLORIA_KEY_CAPS_LOCK,
    SCROLL_LOCK = GLORIA_KEY_SCROLL_LOCK,
    NUM_LOCK = GLORIA_KEY_NUM_LOCK,
    PRINTSCREEN = GLORIA_KEY_PRINT_SCREEN,
    PAUSE = GLORIA_KEY_PAUSE,
    F1 = GLORIA_KEY_F1,
    F2 = GLORIA_KEY_F2,
    F3 = GLORIA_KEY_F3,
    F4 = GLORIA_KEY_F4,
    F5 = GLORIA_KEY_F5,
    F6 = GLORIA_KEY_F6,
    F7 = GLORIA_KEY_F7,
    F8 = GLORIA_KEY_F8,
    F9 = GLORIA_KEY_F9,
    F10 = GLORIA_KEY_F10,
    F11 = GLORIA_KEY_F11,
    F12 = GLORIA_KEY_F12,
    F13 = GLORIA_KEY_F13,
    F14 = GLORIA_KEY_F14,
    F15 = GLORIA_KEY_F15,
    F16 = GLORIA_KEY_F16,
    F17 = GLORIA_KEY_F17,
    F18= GLORIA_KEY_F18,
    F19 = GLORIA_KEY_F19,
    F20 = GLORIA_KEY_F20,
    F21 = GLORIA_KEY_F21,
    F22 = GLORIA_KEY_F22,
    F23 = GLORIA_KEY_F23,
    F24 = GLORIA_KEY_F24,
    F25 = GLORIA_KEY_F25,
    F26 = GLORIA_KEY_KP_0,
    KP_1 = GLORIA_KEY_KP_1,
    KP_2 = GLORIA_KEY_KP_2,
    KP_3 = GLORIA_KEY_KP_3,
    KP_4 = GLORIA_KEY_KP_4,
    KP_5 = GLORIA_KEY_KP_5,
    KP_6 = GLORIA_KEY_KP_6,
    KP_7 = GLORIA_KEY_KP_7,
    KP_8 = GLORIA_KEY_KP_8,
    KP_9 = GLORIA_KEY_KP_9,
    KP_DECIMAL = GLORIA_KEY_KP_DECIMAL,
    KP_DIVIDE = GLORIA_KEY_KP_DIVIDE,
    KP_MULTIPLY = GLORIA_KEY_KP_MULTIPLY,
    KP_SUBTRACT = GLORIA_KEY_KP_SUBTRACT,
    KP_ADD = GLORIA_KEY_KP_ADD,
    KP_ENTER = GLORIA_KEY_KP_ENTER,
    KP_EQUAL = GLORIA_KEY_KP_EQUAL,
    LSHIFT = GLORIA_KEY_LEFT_SHIFT,
    LCTRL = GLORIA_KEY_LEFT_CONTROL,
    LALT = GLORIA_KEY_LEFT_ALT,
    LEFT_WINDOWS = GLORIA_KEY_LEFT_SUPER,
    RSHIFT = GLORIA_KEY_RIGHT_SHIFT,
    RCTRL = GLORIA_KEY_RIGHT_CONTROL,
    RALT = GLORIA_KEY_RIGHT_ALT,
    RIGHT_WINDOWS = GLORIA_KEY_RIGHT_SUPER,
    MENU = GLORIA_KEY_MENU
};