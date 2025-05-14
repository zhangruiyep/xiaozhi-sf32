/* thing.c */
#include "thing.h"
#include <stdlib.h>
#include <string.h>

/* JSON Generation */
cJSON* IOT_Thing_GetDescriptorJson(const IOT_Thing* thing) {
    cJSON* root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "name", thing->name);
    cJSON_AddStringToObject(root, "description", thing->description);

    /* Properties */
    cJSON_AddItemToObject(root, "properties", IOT_PropertyList_GetDescriptorJson((IOT_PropertyList *)&thing->properties));

    /* Methods */
    cJSON_AddItemToObject(root, "methods", IOT_Methods_GetDescriptorJson((IOT_MethodList*)&thing->methods));
    return root;
}

cJSON* IOT_Thing_GetStateJson(const IOT_Thing* thing) {
    cJSON* root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "name", thing->name);
    cJSON_AddItemToObject(root, "state", IOT_PropertyList_GetStateJson((IOT_PropertyList *)&thing->properties));
    return root;
}

/* Command Execution */
void IOT_Thing_Invoke(IOT_Thing* thing, const cJSON* command) {
    const cJSON* method_name = cJSON_GetObjectItem(command, "method");
    const cJSON* input_params = cJSON_GetObjectItem(command, "parameters");

    if (!method_name || !input_params) return;

    rt_kprintf("%s method %s\n", __func__, method_name->valuestring);
    IOT_Method* method = IOT_Thing_GetMethodByName(&thing->methods, method_name->valuestring);
    if (method)
    {
        /* Parameter Processing */
        IOT_Thing_SetParameterList(&method->parameters, input_params);
        /* Method Execution */
        if (method->callback) {
            method->callback(&method->parameters);
        }
    }
}

#if 0
/* Memory Cleanup */
void IOT_FreeThing(IOT_Thing* thing) {
    //free(thing->name);
    //free(thing->description);

    /* Free properties */
    for (size_t i = 0; i < thing->properties.count; i++) {
        IOT_Property* prop = thing->properties.properties[i];
        free(prop->name);
        free(prop->description);
        free(prop);
    }
    free(thing->properties.properties);

    /* Free methods */
    for (size_t i = 0; i < thing->methods.count; i++) {
        IOT_Method* method = thing->methods.methods[i];
        free(method->name);
        free(method->description);

        /* Free parameters */
        for (size_t j = 0; j < method->params.count; j++) {
            IOT_Parameter* param = method->params.parameters[j];
            free(param->name);
            free(param->description);
            if (param->type == VALUE_TYPE_STRING) free(param->string_value);
            free(param);
        }
        free(method->params.parameters);
        free(method);
    }
    free(thing->methods.methods);

    free(thing);
}
#endif
