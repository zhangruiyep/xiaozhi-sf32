
#ifndef IOT_PROPERTY_H
#define IOT_PROPERTY_H

#include "global.h"

typedef void *(*IOT_Property_getter_cb)(void);

/* Property Structure */
typedef struct {
    const char* name;
    const char* description;
    ValueType type;
    IOT_Property_getter_cb getter;
} IOT_Property;

typedef struct {
    rt_list_t list;
    IOT_Property property;
} IOT_PropertyList;

cJSON* IOT_PropertyList_GetDescriptorJson(IOT_PropertyList *property_list);
cJSON* IOT_PropertyList_GetStateJson(IOT_PropertyList *property_list);

int IOT_Thing_AddProperty(IOT_PropertyList *property_list, const char* name, const char* description, ValueType type, IOT_Property_getter_cb getter);
int IOT_Thing_AddNumberProperty(IOT_PropertyList *property_list, const char* name, const char* description, IOT_Property_getter_cb getter);
int IOT_Thing_AddStringProperty(IOT_PropertyList *property_list, const char* name, const char* description, IOT_Property_getter_cb getter);
int IOT_Thing_AddBooleanProperty(IOT_PropertyList *property_list, const char* name, const char* description, IOT_Property_getter_cb getter);


#endif
