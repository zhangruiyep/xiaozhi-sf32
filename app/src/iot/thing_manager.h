/* thing_manager.h */
#ifndef THING_MANAGER_H
#define THING_MANAGER_H

#include "thing.h"
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    rt_list_t list;
    IOT_Thing* thing;
} IOT_ThingList;

typedef struct {
    IOT_ThingList things;
    size_t count;
    size_t capacity;

    struct {
        char** keys;
        char** values;
        size_t count;
        size_t capacity;
    } last_states;
} IOT_ThingManager;

IOT_ThingManager* IOT_GetThingManager(void);
void IOT_ThingManager_AddThing(IOT_ThingManager* manager, IOT_Thing* thing);
cJSON* IOT_ThingManager_GetDescriptorsJson(IOT_ThingManager* manager);
cJSON* IOT_ThingManager_GetStatesJson(IOT_ThingManager* manager, bool delta, bool *changed);
void IOT_ThingManager_Invoke(IOT_ThingManager* manager, const cJSON* command);
void IOT_FreeThingManager(IOT_ThingManager* manager);

#ifdef __cplusplus
}
#endif

#endif // THING_MANAGER_H