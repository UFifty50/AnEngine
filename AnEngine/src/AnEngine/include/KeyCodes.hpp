#ifndef KEYCODES_HPP
#define KEYCODES_HPP


// extracted from glfw3.h

/* The unknown key */
#define AE_KEY_UNKNOWN            -1

/* Printable keys */
#define AE_KEY_SPACE              32
#define AE_KEY_APOSTROPHE         39  /* ' */
#define AE_KEY_COMMA              44  /* , */
#define AE_KEY_MINUS              45  /* - */
#define AE_KEY_PERIOD             46  /* . */
#define AE_KEY_SLASH              47  /* / */
#define AE_KEY_0                  48
#define AE_KEY_1                  49
#define AE_KEY_2                  50
#define AE_KEY_3                  51
#define AE_KEY_4                  52
#define AE_KEY_5                  53
#define AE_KEY_6                  54
#define AE_KEY_7                  55
#define AE_KEY_8                  56
#define AE_KEY_9                  57
#define AE_KEY_SEMICOLON          59  /* ; */
#define AE_KEY_EQUAL              61  /* = */
#define AE_KEY_A                  65
#define AE_KEY_B                  66
#define AE_KEY_C                  67
#define AE_KEY_D                  68
#define AE_KEY_E                  69
#define AE_KEY_F                  70
#define AE_KEY_G                  71
#define AE_KEY_H                  72
#define AE_KEY_I                  73
#define AE_KEY_J                  74
#define AE_KEY_K                  75
#define AE_KEY_L                  76
#define AE_KEY_M                  77
#define AE_KEY_N                  78
#define AE_KEY_O                  79
#define AE_KEY_P                  80
#define AE_KEY_Q                  81
#define AE_KEY_R                  82
#define AE_KEY_S                  83
#define AE_KEY_T                  84
#define AE_KEY_U                  85
#define AE_KEY_V                  86
#define AE_KEY_W                  87
#define AE_KEY_X                  88
#define AE_KEY_Y                  89
#define AE_KEY_Z                  90
#define AE_KEY_LEFT_BRACKET       91  /* [ */
#define AE_KEY_BACKSLASH          92  /* \ */
#define AE_KEY_RIGHT_BRACKET      93  /* ] */
#define AE_KEY_BACKTICK           96  /* ` */
#define AE_KEY_WORLD_1            161 /* non-US #1 */
#define AE_KEY_WORLD_2            162 /* non-US #2 */

/* Function keys */
#define AE_KEY_ESCAPE             256
#define AE_KEY_ENTER              257
#define AE_KEY_TAB                258
#define AE_KEY_BACKSPACE          259
#define AE_KEY_INSERT             260
#define AE_KEY_DELETE             261
#define AE_KEY_RIGHT              262
#define AE_KEY_LEFT               263
#define AE_KEY_DOWN               264
#define AE_KEY_UP                 265
#define AE_KEY_PAGE_UP            266
#define AE_KEY_PAGE_DOWN          267
#define AE_KEY_HOME               268
#define AE_KEY_END                269
#define AE_KEY_CAPS_LOCK          280
#define AE_KEY_SCROLL_LOCK        281
#define AE_KEY_NUM_LOCK           282
#define AE_KEY_PRINT_SCREEN       283
#define AE_KEY_PAUSE              284
#define AE_KEY_F1                 290
#define AE_KEY_F2                 291
#define AE_KEY_F3                 292
#define AE_KEY_F4                 293
#define AE_KEY_F5                 294
#define AE_KEY_F6                 295
#define AE_KEY_F7                 296
#define AE_KEY_F8                 297
#define AE_KEY_F9                 298
#define AE_KEY_F10                299
#define AE_KEY_F11                300
#define AE_KEY_F12                301
#define AE_KEY_F13                302
#define AE_KEY_F14                303
#define AE_KEY_F15                304
#define AE_KEY_F16                305
#define AE_KEY_F17                306
#define AE_KEY_F18                307
#define AE_KEY_F19                308
#define AE_KEY_F20                309
#define AE_KEY_F21                310
#define AE_KEY_F22                311
#define AE_KEY_F23                312
#define AE_KEY_F24                313
#define AE_KEY_F25                314
#define AE_KEY_KP_0               320
#define AE_KEY_KP_1               321
#define AE_KEY_KP_2               322
#define AE_KEY_KP_3               323
#define AE_KEY_KP_4               324
#define AE_KEY_KP_5               325
#define AE_KEY_KP_6               326
#define AE_KEY_KP_7               327
#define AE_KEY_KP_8               328
#define AE_KEY_KP_9               329
#define AE_KEY_KP_DECIMAL         330
#define AE_KEY_KP_DIVIDE          331
#define AE_KEY_KP_MULTIPLY        332
#define AE_KEY_KP_SUBTRACT        333
#define AE_KEY_KP_ADD             334
#define AE_KEY_KP_ENTER           335
#define AE_KEY_KP_EQUAL           336
#define AE_KEY_LEFT_SHIFT         340
#define AE_KEY_LEFT_CONTROL       341
#define AE_KEY_LEFT_ALT           342
#define AE_KEY_LEFT_SUPER         343
#define AE_KEY_RIGHT_SHIFT        344
#define AE_KEY_RIGHT_CONTROL      345
#define AE_KEY_RIGHT_ALT          346
#define AE_KEY_RIGHT_SUPER        347
#define AE_KEY_MENU               348

#define AE_KEY_LAST               AE_KEY_MENU

/*! @brief If this bit is set one or more Shift keys were held down.
 *
 *  If this bit is set one or more Shift keys were held down.
 */
#define AE_MOD_SHIFT           0x0001
 /*! @brief If this bit is set one or more Control keys were held down.
  *
  *  If this bit is set one or more Control keys were held down.
  */
#define AE_MOD_CONTROL         0x0002
  /*! @brief If this bit is set one or more Alt keys were held down.
   *
   *  If this bit is set one or more Alt keys were held down.
   */
#define AE_MOD_ALT             0x0004
   /*! @brief If this bit is set one or more Super keys were held down.
    *
    *  If this bit is set one or more Super keys were held down.
    */
#define AE_MOD_SUPER           0x0008
    /*! @brief If this bit is set the Caps Lock key is enabled.
     *
     *  If this bit is set the Caps Lock key is enabled and the @ref
     *  AE_LOCK_KEY_MODS input mode is set.
     */
#define AE_MOD_CAPS_LOCK       0x0010
     /*! @brief If this bit is set the Num Lock key is enabled.
      *
      *  If this bit is set the Num Lock key is enabled and the @ref
      *  AE_LOCK_KEY_MODS input mode is set.
      */
#define AE_MOD_NUM_LOCK        0x0020

#endif
