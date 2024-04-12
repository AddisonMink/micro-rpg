#pragma once

#include "asset/asset.h"
#include "battle/action.h"
#include "battle/combatant.h"

#define MAX_EVENTS 5

typedef enum EventType
{
    EVENT_WAIT,
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
        Color flash;
        const char *message;
        const Animation *animate;
    } data;
} Event;

typedef struct EventList
{
    Event data[MAX_EVENTS];
    int capacity;
    int count;
} EventList;

Event Event_Wait(Id id, float duration);

Event Event_Flash(Id id, Color flash, float duration);

Event Event_Move(Id id, float duration);

Event Event_Message(Id id, const char *message, float duration);

Event Event_Animate(Id id, AnimationId animationId);

Event Event_Fade(Id id, float duration);

Event Event_Status(Id id, float duration);