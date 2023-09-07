#ifndef GAME_H
#define GAME_H

#include "main.h"
#include "fps.h"
#include "clock.h"

struct Game {
    SDL_Event event;
    SDL_Window *window;
    SDL_Renderer *renderer;
    struct Clock *clock;
    struct Fps *fps;
    bool playing;
    double delta_time;
};

bool game_new(struct Game **game);
void game_free(struct Game **game);
Uint32 game_time_event(Uint32 interval, void *param);
bool game_run(struct Game *g);

#endif
