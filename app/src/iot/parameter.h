
#ifndef IOT_PARAMETER_H
#define IOT_PARAMETER_H

typedef struct {
    const char * name;
    const char * description;
    ValueType type;
    bool required;
    void *value;
} IOT_Parameter;

typedef struct {
    rt_list_t list;
    IOT_Parameter parameter;
} IOT_ParameterList;

cJSON* IOT_ParameterList_GetDescriptorJson(IOT_ParameterList *parameter_list);
int IOT_Thing_AddParameter(IOT_ParameterList *parameter_list, const char* name, const char* description, ValueType type, bool required);
void IOT_Thing_SetParameterList(IOT_ParameterList *parameter_list, const cJSON *input_params);
void* IOT_Thing_ParameterList_GetParameter(IOT_ParameterList *parameter_list, const char *name, ValueType *type);

#endif
