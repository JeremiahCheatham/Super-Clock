#include "game.h"
#include "initialize.h"

bool game_new(struct Game **game) {
    *game = calloc(1, sizeof(struct Game));
    if (*game == NULL) {
        fprintf(stderr, "Error in calloc of new game.\n");
        return true;
    }
    struct Game *g = *game;

    if (game_initilize(g)) {
        return true;
    }

    if (clock_new(&g->clock, g->window, g->renderer)) {
        return true;
    }

    if (fps_new(&g->fps)) {
        return true;
    }

    g->playing = true;

    return false;
}

void game_free(struct Game **game) {
    fps_free(&(*game)->fps);
    clock_free(&(*game)->clock);

    SDL_DestroyRenderer((*game)->renderer);
    (*game)->renderer = NULL;
    SDL_DestroyWindow((*game)->window);
    (*game)->window = NULL;
    TTF_Quit();
    SDL_Quit();
    free(*game);
    *game = NULL;
}

bool game_run(struct Game *g) {

    while (true) {
        while (SDL_PollEvent(&g->event)) {
            switch (g->event.type) {
                case SDL_QUIT:
                    return false;
                    break;
                case CLOCK_TIMER_EVENT:
                    clock_hide_time(g->clock);
                    break;
                case SDL_KEYDOWN:
                    switch (g->event.key.keysym.scancode) {
                        case SDL_SCANCODE_ESCAPE:
                            return false;
                            break;
                        case SDL_SCANCODE_F:
                            fps_toggle_display(g->fps);
                            break;
                        case SDL_SCANCODE_S:
                            clock_update_layout(g->clock);
                            break;
                        case SDL_SCANCODE_T:
                        case SDL_SCANCODE_SPACE:
                            clock_show_time(g->clock);
                            break;
                        default:
                            break;
                    }
                default:
                    break;
            }
        }

        clock_update(g->clock);

        if (SDL_RenderClear(g->renderer)) {
            fprintf(stderr, "Error while clearing renderer: %s\n", SDL_GetError());
            return true;
        }
        
        if (clock_draw(g->clock)) {
            return true;
        }
        SDL_RenderPresent(g->renderer);

        g->delta_time = fps_update(g->fps);
    }
    
    return false;
}
