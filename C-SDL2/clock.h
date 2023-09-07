#ifndef CLOCK_H
#define CLOCK_H

#include "main.h"

struct Digit {
    SDL_Rect rect;
    bool digit;
};

struct Clock {
    SDL_Window *window;
    SDL_Renderer *renderer;
    TTF_Font *font;
    SDL_Surface *target_surf;
    SDL_Surface *text_surf;
    SDL_Texture *images[4];
    struct Digit digits[24];
    int theme;
    int style;
    SDL_TimerID timer;
    bool show_time;
};

bool clock_new(struct Clock **clock, SDL_Window *window, SDL_Renderer *renderer);
void clock_free(struct Clock **clock);
bool clock_generate_images(struct Clock *c);
void clock_update_layout(struct Clock *c);
void clock_update(struct Clock *c);
void clock_show_time(struct Clock *c);
void clock_hide_time(struct Clock *c);
Uint32 clock_timer_event(Uint32 interval, void *param); 
bool clock_draw(struct Clock *c);

#endif
