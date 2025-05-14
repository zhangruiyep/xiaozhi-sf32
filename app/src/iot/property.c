
#include "global.h"
#include "thing.h"

/* name, description should have been allocated */
void IOT_Property_Init(IOT_Property *property, const char *name, const char *description, ValueType type, IOT_Property_getter_cb getter)
{
    property->name = name;
    property->description = description;
    property->type = type;
    property->getter = getter;
}

cJSON* IOT_Property_GetDescriptorJson(IOT_Property *property) {
    cJSON* json = cJSON_CreateObject();
    if (json == NULL)
    {
        return NULL;
    }

    cJSON_AddStringToObject(json, "description", property->description);
    if (property->type == kValueTypeBoolean) {
        cJSON_AddStringToObject(json, "type", "boolean");
    } else if (property->type == kValueTypeNumber) {
        cJSON_AddStringToObject(json, "type", "number");
    } else if (property->type == kValueTypeString) {
        cJSON_AddStringToObject(json, "type", "string");
    }
    return json;
}

void* IOT_Property_GetStateJson(IOT_Property *property) {
    return (void *)property->getter();
}

int IOT_Thing_AddProperty(IOT_PropertyList *property_list, const char* name, const char* description, ValueType type, IOT_Property_getter_cb getter)
{
    IOT_PropertyList *node = (IOT_PropertyList *)rt_malloc(sizeof(IOT_PropertyList));
    rt_list_init(&node->list);
    IOT_Property_Init(&node->property, name, description, type, getter);
    rt_list_insert_after(&property_list->list, &node->list);
    return rt_list_len(&property_list->list);
}

int IOT_Thing_AddNumberProperty(IOT_PropertyList *property_list, const char* name, const char* description, IOT_Property_getter_cb getter)
{
    return IOT_Thing_AddProperty(property_list, name, description, kValueTypeNumber, getter);
}

int IOT_Thing_AddStringProperty(IOT_PropertyList *property_list, const char* name, const char* description, IOT_Property_getter_cb getter)
{
    return IOT_Thing_AddProperty(property_list, name, description, kValueTypeString, getter);
}

int IOT_Thing_AddBooleanProperty(IOT_PropertyList *property_list, const char* name, const char* description, IOT_Property_getter_cb getter)
{
    return IOT_Thing_AddProperty(property_list, name, description, kValueTypeBoolean, getter);
}

cJSON* IOT_PropertyList_GetDescriptorJson(IOT_PropertyList *property_list) {
    cJSON* json = cJSON_CreateObject();
    if (json == NULL)
    {
        return NULL;
    }
    rt_list_t *node = NULL;
    for (node = property_list->list.next; node != &property_list->list; node = node->next)
    {
        IOT_PropertyList *property_node = rt_list_entry(node, IOT_PropertyList, list);
        cJSON_AddItemToObject(json, property_node->property.name, IOT_Property_GetDescriptorJson(&property_node->property));
    }
    return json;
}

cJSON* IOT_PropertyList_GetStateJson(IOT_PropertyList *property_list) {
    cJSON* json = cJSON_CreateObject();
    if (json == NULL)
    {
        return NULL;
    }
    rt_list_t *node = NULL;
    for (node = property_list->list.next; node != &property_list->list; node = node->next)
    {
        IOT_PropertyList *property_node = rt_list_entry(node, IOT_PropertyList, list);
        switch (property_node->property.type)
        {
            case kValueTypeNumber:
                cJSON_AddNumberToObject(json, property_node->property.name, *(int *)property_node->property.getter());
                break;
            case kValueTypeString:
                cJSON_AddStringToObject(json, property_node->property.name, (char *)property_node->property.getter());
                break;
            case kValueTypeBoolean:
                //bool value = *(bool *)property_node->property.getter();
                cJSON_AddBoolToObject(json, property_node->property.name, *(bool *)property_node->property.getter());
                break;
            default:
                break;
        }
    }
    return json;
}
