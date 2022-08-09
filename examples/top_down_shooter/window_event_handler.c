#include "./window_event_handler.h"

#define EVENT_HANDLER_PARAMS ClientCtx* ctx, SDL_Event* event

void quitHandler(EVENT_HANDLER_PARAMS)
{
    ctx->quit = 1;
}

void keyDownHandler(EVENT_HANDLER_PARAMS)
{
    SDL_KeyboardEvent* key;
    key = (SDL_KeyboardEvent*)event;

    switch(key->keysym.sym) {
        case SDLK_LEFT:
            ctx->currentInput.inputFlags |= RTGN_INPUT_FLAG_LEFT;
            break;
        case SDLK_RIGHT:
            ctx->currentInput.inputFlags |= RTGN_INPUT_FLAG_RIGHT;
            break;
    }
}

void keyUpHandler(EVENT_HANDLER_PARAMS)
{
    SDL_KeyboardEvent* key;
    key = (SDL_KeyboardEvent*)event;

    switch(key->keysym.sym) {
        case SDLK_LEFT:
            ctx->currentInput.inputFlags &= ~RTGN_INPUT_FLAG_LEFT;
            break;
        case SDLK_RIGHT:
            ctx->currentInput.inputFlags &= ~RTGN_INPUT_FLAG_RIGHT;
            break;
    }
}

void (*handlers[])(EVENT_HANDLER_PARAMS) = {
    [SDL_QUIT] = quitHandler,
    [SDL_KEYDOWN] = keyDownHandler,
    [SDL_KEYUP] = keyUpHandler,
};

void handleWindowEvent(ClientCtx* ctx, SDL_Event* event)
{
    int maxHandledType = sizeof(handlers) / sizeof(handlers[0]) - 1;
    if(event->type > maxHandledType) return;
    void (*handler)(EVENT_HANDLER_PARAMS);
    handler = handlers[event->type];
    if(handler == NULL) return;
    handler(ctx, event);
}
