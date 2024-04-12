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

Event Event_Move(Id id, float duration)
{
    return (Event){
        .id = id,
        .elapsed = 0,
        .duration = duration,
        .type = EVENT_MOVE,
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

Event Event_Animate(Id id, AnimationId animationId)
{
    const Animation *animation = Asset_Animation(animationId);

    return (Event){
        .id = id,
        .elapsed = 0,
        .duration = Asset_AnimationDuration(animation),
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

Event Event_Status(Id id, float duration)
{
    return (Event){
        .id = id,
        .elapsed = 0,
        .duration = duration,
        .type = EVENT_STATUS,
    };
}