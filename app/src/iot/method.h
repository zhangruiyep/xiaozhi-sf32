
#ifndef IOT_METHOD_H
#define IOT_METHOD_H

#include "parameter.h"

typedef void (*IOT_Method_callback)(IOT_ParameterList *parameters);

typedef struct {
    const char * name;
    const char * description;
    IOT_ParameterList parameters;
    IOT_Method_callback callback;
} IOT_Method;

typedef struct {
    rt_list_t list;
    IOT_Method method;
} IOT_MethodList;

cJSON *IOT_Methods_GetDescriptorJson(IOT_MethodList* methods);
IOT_Method* IOT_Thing_AddMethod(IOT_MethodList *method_list, const char* name, const char* description, IOT_Method_callback callback);
IOT_Method* IOT_Thing_GetMethodByName(IOT_MethodList *method_list, const char* name);

#endif
