#include "./window_event_handler.h"

#define EVENT_HANDLER_PARAMS ClientCtx* ctx, SDL_Event event

void quitEvent(EVENT_HANDLER_PARAMS)
{
    ctx->quit = 1;
}

void (*handlers[])(EVENT_HANDLER_PARAMS) = {
    [SDL_QUIT] = quitEvent,
};

void handleWindowEvent(ClientCtx* ctx, SDL_Event event)
{
    int maxHandledType = sizeof(handlers) / sizeof(handlers[0]) - 1;
    if(event.type > maxHandledType) return;
    void (*handler)(EVENT_HANDLER_PARAMS);
    handler = handlers[event.type];
    if(handler == NULL) return;
    handler(ctx, event);
}
