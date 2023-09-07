#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#define SDL_FLAGS (SDL_INIT_VIDEO|SDL_INIT_AUDIO|SDL_INIT_TIMER)
#define WINDOW_TITLE "Super Clock"
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define CLOCK_TIMER_EVENT SDL_USEREVENT
#define TARGET_FPS 60
#define SIZE 36
#define FONT_SIZE 24

// Length of array macro.
#define LEN(x) (sizeof(x)/sizeof(x[0]))

#endif
