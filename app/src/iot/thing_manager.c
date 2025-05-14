/* thing_manager.c */
#include "thing_manager.h"
#include <stdlib.h>
#include <string.h>
#include <cJSON.h>

#define INITIAL_CAPACITY 8
#define TAG "ThingManager"

static IOT_ThingManager g_thing_manager = {
    .count = 0,
    .capacity = 0,
    .last_states = {
        .keys = NULL,
        .values = NULL,
        .count = 0,
        .capacity = 0
    }
};

IOT_ThingManager* IOT_GetThingManager(void) {
    return &g_thing_manager;
}

void IOT_ThingManager_AddThing(IOT_ThingManager* manager, IOT_Thing* thing) {
    if ((manager == NULL) || (thing == NULL))
    {
        return;
    }

    IOT_ThingList *node = (IOT_ThingList *)rt_malloc(sizeof(IOT_ThingList));
    rt_list_init(&node->list);
    node->thing = thing;
    rt_list_insert_after(&manager->things.list, &node->list);
    return;
}

cJSON* IOT_ThingManager_GetDescriptorsJson(IOT_ThingManager* manager) {
    cJSON* root = cJSON_CreateArray();
    rt_list_t *node = NULL;
    for (node = manager->things.list.next; node != &manager->things.list; node = node->next)
    {
        IOT_ThingList *thing_node = rt_list_entry(node, IOT_ThingList, list);
        cJSON_AddItemToArray(root, IOT_Thing_GetDescriptorJson(thing_node->thing));
    }
    return root;
}

static bool update_state(IOT_ThingManager* manager, const char* name, const char* state) {
    // Update state storage
    for (size_t i = 0; i < manager->last_states.count; i++) {
        if (strcmp(manager->last_states.keys[i], name) == 0) {
            if (strcmp(manager->last_states.values[i], state) == 0) {
                return false; // No change
            }
            free(manager->last_states.values[i]);
            manager->last_states.values[i] = malloc(strlen(state) + 1);
            if (manager->last_states.values[i]) {
                strcpy(manager->last_states.values[i], state);
            }
            return true;
        }
    }

    // Add new entry
    if (manager->last_states.count >= manager->last_states.capacity) {
        size_t new_cap = manager->last_states.capacity == 0 ?
            INITIAL_CAPACITY : manager->last_states.capacity * 2;
        char** new_keys = realloc(manager->last_states.keys, new_cap * sizeof(char*));
        char** new_vals = realloc(manager->last_states.values, new_cap * sizeof(char*));
        if (!new_keys || !new_vals) return false;

        manager->last_states.keys = new_keys;
        manager->last_states.values = new_vals;
        manager->last_states.capacity = new_cap;
    }

    manager->last_states.keys[manager->last_states.count] = malloc(strlen(name) + 1);
    if (manager->last_states.keys[manager->last_states.count]) {
        strcpy(manager->last_states.keys[manager->last_states.count], name);
    }
    manager->last_states.values[manager->last_states.count] = malloc(strlen(state) + 1);
    if (manager->last_states.values[manager->last_states.count]) {
        strcpy(manager->last_states.values[manager->last_states.count], state);
    }
    manager->last_states.count++;
    return true;
}

cJSON* IOT_ThingManager_GetStatesJson(IOT_ThingManager* manager, bool delta, bool *changed) {
    cJSON* root = cJSON_CreateArray();

    rt_list_t *node = NULL;
    for (node = manager->things.list.next; node != &manager->things.list; node = node->next)
    {
        IOT_ThingList *thing_node = rt_list_entry(node, IOT_ThingList, list);
        cJSON_AddItemToArray(root, IOT_Thing_GetStateJson(thing_node->thing));
    }
    *changed = true;

    return root;
}

void IOT_ThingManager_Invoke(IOT_ThingManager* manager, const cJSON* command) {
    const cJSON* name = cJSON_GetObjectItem(command, "name");
    if (!name || !name->valuestring) return;

    rt_list_t *node = NULL;
    for (node = manager->things.list.next; node != &manager->things.list; node = node->next)
    {
        IOT_ThingList *thing_node = rt_list_entry(node, IOT_ThingList, list);
        if (strcmp(thing_node->thing->name, name->valuestring) == 0) {
            rt_kprintf("%s Invoke %s\n", __func__, name->valuestring);
            IOT_Thing_Invoke(thing_node->thing, command);
            break;
        }
    }
}

void IOT_FreeThingManager(IOT_ThingManager* manager) {
#if 0
    // Free things array
    free(manager->things);

    // Free state history
    for (size_t i = 0; i < manager->last_states.count; i++) {
        free(manager->last_states.keys[i]);
        free(manager->last_states.values[i]);
    }
    free(manager->last_states.keys);
    free(manager->last_states.values);

    // Reset structure
    memset(manager, 0, sizeof(IOT_ThingManager));
#endif
}
