#pragma once

#include "asset/asset.h"
#include "battle/action.h"
#include "battle/combatant.h"
#include "common/list-macros-new.h"

#define MAX_EVENTS 5

typedef enum EventType
{
    EVENT_WAIT,
    EVENT_GLOBAL_MESSAGE,
    EVENT_FLASH,
    EVENT_MOVE,
    EVENT_MESSAGE,
    EVENT_ANIMATE,
    EVENT_FADE,
    EVENT_STATUS,
} EventType;

typedef struct Event
{
    Id id;
    float elapsed;
    float duration;
    EventType type;
    union
    {
        const char *globalMessage;
        Color flash;
        const char *message;
        const Animation *animate;
    } data;
} Event;

STATIC_LIST_TYPE(EventList, Event, MAX_EVENTS);

Event Event_GlobalMessage(const char *message, float duration);

Event Event_Wait(float duration);

Event Event_Flash(Id id, Color flash, float duration);

Event Event_Move(Id id, float duration);

Event Event_Message(Id id, const char *message, float duration);

Event Event_Animate(Id id, AnimationId animationId);

Event Event_Fade(Id id, float duration);

Event Event_Status(Id id, float duration);