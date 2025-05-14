#ifndef THING_CREATOR_H
#define THING_CREATOR_H

#include "thing.h"
#include "thing_manager.h"

typedef IOT_Thing* (*ThingCreator_func)(void);

/* Thing Creator Registry */
typedef struct {
    const char* type;
    ThingCreator_func creator;
} ThingCreator;


#endif
