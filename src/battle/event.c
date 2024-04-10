#include "battle/event.h"

Event Event_Wait(Id id, float duration)
{
    return (Event){
        .id = id,
        .elapsed = 0,
        .duration = duration,
        .type = EVENT_WAIT,
    };
}

Event Event_Flash(Id id, Color flash, float duration)
{
    return (Event){
        .id = id,
        .elapsed = 0,
        .duration = duration,
        .type = EVENT_FLASH,
        .data.flash = flash,
    };
}

Event Event_Move(Id id, Direction move, float duration)
{
    return (Event){
        .id = id,
        .elapsed = 0,
        .duration = duration,
        .type = EVENT_MOVE,
        .data.move = move,
    };
}

Event Event_Message(Id id, const char *message, float duration)
{
    return (Event){
        .id = id,
        .elapsed = 0,
        .duration = duration,
        .type = EVENT_MESSAGE,
        .data.message = message,
    };
}

Event Event_Animate(Id id, AnimationId animationId, float duration)
{
    return (Event){
        .id = id,
        .elapsed = 0,
        .duration = duration,
        .type = EVENT_ANIMATE,
        .data.animate = Asset_Animation(animationId),
    };
}

Event Event_Fade(Id id, float duration)
{
    return (Event){
        .id = id,
        .elapsed = 0,
        .duration = duration,
        .type = EVENT_FADE,
    };
}