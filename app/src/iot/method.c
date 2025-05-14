
#include "global.h"
#include "thing.h"

void IOT_Method_Init(IOT_Method *method, const char *name, const char *description, IOT_Method_callback callback)
{
    method->name = name;
    method->description = description;
    method->callback = callback;
    rt_list_init(&method->parameters.list);
}

cJSON *IOT_Method_GetDescriptorJson(IOT_Method *method) {
    cJSON *json = cJSON_CreateObject();
    cJSON_AddStringToObject(json, "description", method->description);
    cJSON_AddItemToObject(json, "parameters", IOT_ParameterList_GetDescriptorJson(&method->parameters));
    return json;
}

void IOT_Method_Invoke(IOT_Method *method) {
    method->callback(&method->parameters);
}

cJSON *IOT_Methods_GetDescriptorJson(IOT_MethodList* methods)
{
    cJSON* json = cJSON_CreateObject();
    rt_list_t *node = NULL;
    for (node = methods->list.next; node != &methods->list; node = node->next)
    {
        IOT_MethodList* method_node = rt_list_entry(node, IOT_MethodList, list);
        cJSON_AddItemToObject(json, method_node->method.name, IOT_Method_GetDescriptorJson(&method_node->method));
    }
    return json;
}

IOT_Method* IOT_Thing_AddMethod(IOT_MethodList *method_list, const char* name, const char* description, IOT_Method_callback callback)
{
    IOT_MethodList *node = (IOT_MethodList *)rt_malloc(sizeof(IOT_MethodList));
    rt_list_init(&node->list);
    IOT_Method_Init(&node->method, name, description, callback);
    rt_list_insert_after(&method_list->list, &node->list);
    return &node->method;
}


IOT_Method* IOT_Thing_GetMethodByName(IOT_MethodList *method_list, const char* name)
{
    rt_list_t *node = NULL;
    for (node = method_list->list.next; node != &method_list->list; node = node->next)
    {
        IOT_MethodList* method_node = rt_list_entry(node, IOT_MethodList, list);
        if (strcmp(method_node->method.name, name) == 0)
        {
            return &method_node->method;
        }
    }
    return NULL;
}
