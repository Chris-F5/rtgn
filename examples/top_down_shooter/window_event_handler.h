#ifndef WINDOW_EVENT_HANDLER_H
#define WINDOW_EVENT_HANDLER_H

#include <SDL2/SDL.h>

#include "./client_ctx.h"

void handleWindowEvent(ClientCtx* ctx, SDL_Event* event);

#endif
