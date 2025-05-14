/* thing.h */
#ifndef THING_H
#define THING_H

#include <stdbool.h>
#include <cJSON.h>

typedef enum {
    kValueTypeBoolean,
    kValueTypeNumber,
    kValueTypeString
} ValueType;

#include "property.h"
#include "parameter.h"
#include "method.h"

/* Main Thing Structure */
typedef struct {
    char* name;
    char* description;
    IOT_PropertyList properties;
    IOT_MethodList methods;
} IOT_Thing;

/* Registration and Creation API */
void IOT_RegisterThing(const char* type, IOT_Thing* (*creator)(void));
IOT_Thing* IOT_CreateThing(const char* type);

/* JSON Operations */
cJSON* IOT_Thing_GetDescriptorJson(const IOT_Thing* thing);
cJSON* IOT_Thing_GetStateJson(const IOT_Thing* thing);
void IOT_Thing_Invoke(IOT_Thing* thing, const cJSON* command);

/* Memory Management */
void IOT_FreeThing(IOT_Thing* thing);

#endif // THING_H
