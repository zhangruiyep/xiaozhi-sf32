
#include "global.h"
#include "thing.h"


void IOT_Parameter_Init(IOT_Parameter *parameter, const char *name, const char *description, ValueType type, bool required)
{
    parameter->name = name;
    parameter->description = description;
    parameter->type = type;
    parameter->required = required;
    parameter->value = NULL;
}

cJSON * IOT_Parameter_GetDescriptorJson(IOT_Parameter *param) {
    cJSON *json = cJSON_CreateObject();
    cJSON_AddStringToObject(json, "description", param->description);
    switch (param->type)
    {
        case kValueTypeBoolean:
            cJSON_AddStringToObject(json, "type", "boolean");
            break;
        case kValueTypeNumber:
            cJSON_AddStringToObject(json, "type", "number");
            break;
        case kValueTypeString:
            cJSON_AddStringToObject(json, "type", "string");
            break;
    }
    return json;
}

cJSON* IOT_ParameterList_GetDescriptorJson(IOT_ParameterList *parameter_list) {
    cJSON* json = cJSON_CreateObject();
    if (json == NULL)
    {
        return NULL;
    }
    rt_list_t *node = NULL;
    for (node = parameter_list->list.next; node != &parameter_list->list; node = node->next)
    {
        IOT_ParameterList *parameter_node = rt_list_entry(node, IOT_ParameterList, list);
        cJSON_AddItemToObject(json, parameter_node->parameter.name, IOT_Parameter_GetDescriptorJson(&parameter_node->parameter));
    }
    return json;
}

int IOT_Thing_AddParameter(IOT_ParameterList *parameter_list, const char* name, const char* description, ValueType type, bool required)
{
    IOT_ParameterList *node = (IOT_ParameterList *)rt_malloc(sizeof(IOT_ParameterList));
    rt_list_init(&node->list);
    IOT_Parameter_Init(&node->parameter, name, description, type, required);
    rt_list_insert_after(&parameter_list->list, &node->list);
    return rt_list_len(&parameter_list->list);
}

void IOT_Thing_SetParameterList(IOT_ParameterList *parameter_list, const cJSON *input_params)
{
    rt_list_t *node = NULL;
    for (node = parameter_list->list.next; node != &parameter_list->list; node = node->next)
    {
        IOT_ParameterList *parameter_node = rt_list_entry(node, IOT_ParameterList, list);
        IOT_Parameter* param = &parameter_node->parameter;
        const cJSON* input_param = cJSON_GetObjectItem(input_params, param->name);

        if (param->required && !input_param) return;

        switch(param->type) {
            case kValueTypeNumber:
                if (param->value == NULL)
                {
                    param->value = (int *)malloc(sizeof(int));
                }
                *(int *)(param->value) = input_param->valueint;
                break;
            case kValueTypeBoolean:
                if (param->value == NULL)
                {
                    param->value = (bool *)malloc(sizeof(bool));
                }
                *(bool *)(param->value) = cJSON_IsTrue(input_param);
                break;
            case kValueTypeString:
                if (param->value) free(param->value);
                param->value = (char *)malloc(strlen(input_param->valuestring) + 1);
                strcpy(param->value, input_param->valuestring);
                break;
            default:
                break;
        }
    }
}

void* IOT_Thing_ParameterList_GetParameter(IOT_ParameterList *parameter_list, const char *name, ValueType *type)
{
    rt_list_t *node = NULL;
    for (node = parameter_list->list.next; node != &parameter_list->list; node = node->next)
    {
        IOT_ParameterList *parameter_node = rt_list_entry(node, IOT_ParameterList, list);
        IOT_Parameter* param = &parameter_node->parameter;
        if (strcmp(param->name, name) == 0)
        {
            *type = param->type;
            return param->value;
        }
    }
    return NULL;
}
