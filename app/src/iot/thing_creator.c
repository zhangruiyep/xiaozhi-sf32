
#include <stdlib.h>
#include <string.h>
#include "thing_creator.h"

extern IOT_Thing* Speaker_creator(void);

ThingCreator thing_creators[] = {
    {"Speaker", Speaker_creator},
};

IOT_Thing* IOT_CreateThing(const char* type) {
    for (size_t i = 0; i < sizeof(thing_creators)/sizeof(thing_creators[0]); i++) {
        if (strcmp(thing_creators[i].type, type) == 0) {
            rt_kprintf("%s: create %s\n", __FUNCTION__, type);
            return thing_creators[i].creator();
        }
    }
    rt_kprintf("%s: %s not found\n", __FUNCTION__, type);
    return NULL;
}

