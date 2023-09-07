#include "clock.h"

bool clock_new(struct Clock **clock, SDL_Window *window, SDL_Renderer *renderer) {
    *clock = calloc(1, sizeof(struct Clock));
    if (!*clock) {
        fprintf(stderr, "Error in calloc of clock!\n");
        return true;
    }

    struct Clock *c = *clock;

    c->window = window;
    c->renderer = renderer;
    c->theme = 0;

    c->font = TTF_OpenFont("fonts/freesansbold.ttf", SIZE);
    if (!c->font) {
        return true;
    }

    if (clock_generate_images(c)) {
        return true;
    }

    SDL_Rect rect;
    if (SDL_QueryTexture(c->images[0], NULL, NULL, &rect.w, &rect.h)) {
        fprintf(stderr, "Error while querying texture: %s\n", SDL_GetError());
        return true;
    }

    for (Uint32 i = 0; i < LEN(c->digits); i++) {
        c->digits[i].rect.w = rect.w;
        c->digits[i].rect.h = rect.h;
    }

    clock_update_layout(c);
    
    return false;
}
 
void clock_free(struct Clock **clock) {
    if (*clock) {
        SDL_FreeSurface((*clock)->target_surf);
        (*clock)->target_surf = NULL;
        SDL_FreeSurface((*clock)->text_surf);
        (*clock)->text_surf = NULL;

        for (Uint32 i = 0; i < LEN((*clock)->images); i++) {
            SDL_DestroyTexture((*clock)->images[i]);
            (*clock)->images[i] = NULL;
        }

        TTF_CloseFont((*clock)->font);
        (*clock)->font = NULL;
        (*clock)->renderer = NULL;
        (*clock)->window = NULL;
        free(*clock);
        *clock = NULL;
    }
    return;
}

bool clock_generate_images(struct Clock *c) {
    SDL_Color back_color = {31, 31, 31, 255};
    SDL_Color front_color = {223, 223, 223, 255};

    c->target_surf = SDL_CreateRGBSurface(0,SIZE, SIZE, 32,0,0,0,255);
    if (!c->target_surf) {
        fprintf(stderr, "Error creating a target text surface: %s\n", SDL_GetError());
        return true;
    }
    
    SDL_FillRect(c->target_surf, NULL, SDL_MapRGB(c->target_surf->format, front_color.r, front_color.g, front_color.b));
 
    c->images[1] = SDL_CreateTextureFromSurface(c->renderer, c->target_surf);
    if (!c->images[1]) {
        fprintf(stderr, "Error creating a texture: %s\n", SDL_GetError());
        return true;
    }

    SDL_FillRect(c->target_surf, NULL, SDL_MapRGB(c->target_surf->format, back_color.r, back_color.g, back_color.b));
 
    c->images[0] = SDL_CreateTextureFromSurface(c->renderer, c->target_surf);
    if (!c->images[0]) {
        fprintf(stderr, "Error creating a texture: %s\n", SDL_GetError());
        return true;
    }

    c->text_surf = TTF_RenderText_Blended(c->font, "0", front_color);
    if (!c->text_surf) {
        fprintf(stderr, "Error creating a surface: %s\n", SDL_GetError());
        return true;
    }

    SDL_BlitSurface(c->text_surf, NULL, c->target_surf,
        &(SDL_Rect){ (int)((SIZE - c->text_surf->w) / 2.0), (int)(SIZE / 12.0), c->text_surf->w, c->text_surf->h });

    c->images[2] = SDL_CreateTextureFromSurface(c->renderer, c->target_surf);
    if (!c->images[2]) {
        fprintf(stderr, "Error creating a texture: %s\n", SDL_GetError());
        return true;
    }

    SDL_FillRect(c->target_surf, NULL, SDL_MapRGB(c->target_surf->format, back_color.r, back_color.g, back_color.b));
 
    SDL_FreeSurface(c->text_surf);
    c->text_surf = NULL;

    c->text_surf = TTF_RenderText_Blended(c->font, "1", front_color);
    if (!c->text_surf) {
        fprintf(stderr, "Error creating a surface: %s\n", SDL_GetError());
        return true;
    }

    SDL_BlitSurface(c->text_surf, NULL, c->target_surf,
        &(SDL_Rect){ (int)((SIZE - c->text_surf->w) / 2.0), (int)(SIZE / 12.0), c->text_surf->w, c->text_surf->h });

    c->images[3] = SDL_CreateTextureFromSurface(c->renderer, c->target_surf);
    if (!c->images[3]) {
        fprintf(stderr, "Error creating a texture: %s\n", SDL_GetError());
        return true;
    }

    return false;
}

void clock_update_layout(struct Clock *c) {
    c->theme = c->theme % 6 + 1;
    c->style = (c->theme < 4) ? 0 : 2;

    Uint32 i = 0;
    int offset = 6;
    switch (c->theme) {
        case 1:
        case 4:
            SDL_SetWindowSize(c->window, 294, 174);
            for (int column = 0; column < 6; column++) {
                if (column > 3) {
                    offset = 42;
                } else if (column > 1) {
                    offset = 24;
                }
                for (int row = 0; row < 4; row++) {
                    if (i < LEN(c->digits)) {
                        c->digits[i].rect.x = column * (SIZE + 6) + offset;
                        c->digits[i].rect.y = row * (SIZE + 6) + 6;
                        i++;
                    }
                }
            }
            break;
        case 2:
        case 5:
            SDL_SetWindowSize(c->window, 168, 342);
            for (int column = 0; column < 3; column++) {
                if (column > 1) {
                    offset = 42;
                } else if (column > 0) {
                    offset = 24;
                }
                for (int row = 0; row < 8; row++) {
                    if (i < LEN(c->digits)) {
                        c->digits[i].rect.x = column * (SIZE + 6) + offset;
                        c->digits[i].rect.y = row * (SIZE + 6) + 6;
                        i++;
                    }
                }
            }
            break;
        case 3:
        case 6:
            SDL_SetWindowSize(c->window, 342, 168);
            for (int row = 0; row < 3; row++) {
                if (row > 1) {
                    offset = 42;
                } else if (row > 0) {
                    offset = 24;
                }
                for (int column = 0; column < 8; column++) {
                    if (i < LEN(c->digits)) {
                        c->digits[i].rect.x = column * (SIZE + 6) + 6;
                        c->digits[i].rect.y = row * (SIZE + 6) + offset;
                        i++;
                    }
                }
            }
            break;
    }
}

void clock_update(struct Clock *c) {
    time_t raw_time;
    time(&raw_time);
    struct tm *current_time = localtime(&raw_time);

    int n = current_time->tm_hour;
    for (int i = 7;i >= 0; i--) {
        c->digits[i].digit = n % 2;
        n /= 2;
    }

    n = current_time->tm_min;
    for (int i = 15;i >= 8; i--) {
        c->digits[i].digit = n % 2;
        n /= 2;
    }

    n = current_time->tm_sec;
    for (int i = 23;i >= 16; i--) {
        c->digits[i].digit = n % 2;
        n /= 2;
    }
    
    if (c->show_time) {
        char time_strg[9];
        snprintf(time_strg, LEN(time_strg), "%02d:%02d:%02d", current_time->tm_hour, current_time->tm_min, current_time->tm_sec);
        SDL_SetWindowTitle(c->window, time_strg);
    }
}

void clock_show_time(struct Clock *c) {
    if (c->show_time) {
        SDL_RemoveTimer(c->timer);
    }
    c->show_time = true;
    c->timer = SDL_AddTimer(5000, clock_timer_event, NULL);
}

void clock_hide_time(struct Clock *c) {
    c->show_time = false;
    SDL_SetWindowTitle(c->window, WINDOW_TITLE);
}

Uint32 clock_timer_event(Uint32 interval, void *param) {
    (void)interval;
    (void)param;
    SDL_Event event;
    event.type = CLOCK_TIMER_EVENT;
    event.user.code = 0;
    event.user.data1 = NULL;
    event.user.data2 = NULL;
    SDL_PushEvent(&event);
    return 0;
}

bool clock_draw(struct Clock *c) {
    for (Uint32 i = 0; i < LEN(c->digits); i++) {
        if (SDL_RenderCopy(c->renderer, c->images[(c->digits[i].digit + c->style)], NULL, &c->digits[i].rect)) {
            fprintf(stderr, "Error while rendering texture: %s\n", SDL_GetError());
            return true;
        }
    }

    return false;
}


