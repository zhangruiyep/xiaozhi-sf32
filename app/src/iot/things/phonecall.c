
#include "iot/thing.h"
#include "app_phonecall_data.h"
#include "app_phonecall_dial.h"
#include "app_phonecall_calling.h"

#define TAG "PhoneCall"

// PhoneCall structure definition
typedef struct {
    IOT_Thing *thing;
    char phone_number[20];
} PhoneCall;

static PhoneCall g_phone_call = {0};

// Property getter function
static void * get_number_property(void) {
    return g_phone_call.phone_number;
}

// Method implementation
void xz_phone_call(void)
{
    known_contact_t contact = {0};
    rt_memcpy(&contact.num, g_phone_call.phone_number, strlen(g_phone_call.phone_number));
    if (0 == phonecall_get_contacts(&contact, 0))
        contact.name[0] = 0;

    if (phonecall_data_init(APP_PHONECALL_STATE_DIALING_WATCH, &contact))
    {
#if defined(RT_USING_BT) && !defined(BSP_USING_PC_SIMULATOR)
        phonecall_data_make_call(g_phone_call.phone_number, (uint16_t)rt_strlen(g_phone_call.phone_number));
#endif
        phonecall_calling_entry();
    }
    else
    {
        rt_kprintf("%s: can't call num %s , init failed", __func__, g_phone_call.phone_number);
    }
}

static void set_number_method(IOT_ParameterList* parameters) {
    ValueType type;
    char *number_string = (char *)IOT_Thing_ParameterList_GetParameter(parameters, "phone_number", &type);
    if (number_string == NULL)
    {
        rt_kprintf("[PhoneCall] get phone_number failed\n");
        return;
    }
    strcpy(g_phone_call.phone_number, number_string);
    if (rt_strlen(g_phone_call.phone_number) < 3)
    {
        rt_kprintf("%s: can't call num %s , too short", __func__, g_phone_call.phone_number);
        return;
    }
    /* send msg to lv task, run phonecall app */
    hl_if_data_t *msg = service_fill_msg(APP_INT, APP_PHONECALL_XZ_MO, NULL, 0);
    send_msg_to_ui_task(msg, sizeof(msg), NEED_WAKEUP_UI, ;);
}

// PhoneCall initialization
IOT_Thing* PhoneCall_creator(void) {
    rt_kprintf("%s in\n", __func__);
    if (g_phone_call.thing == NULL)
    {
        g_phone_call.thing = (IOT_Thing*)rt_malloc(sizeof(IOT_Thing));
        if (g_phone_call.thing == NULL)
        {
            rt_kprintf("[PhoneCall] malloc failed\n");
            return NULL;
        }
    }
    IOT_Thing* thing = g_phone_call.thing;
    thing->name = "PhoneCall";
    thing->description = "电话拨号应用程序，可以拨打电话";

    // Define volume property
    rt_kprintf("%s add propertys\n", __func__);
    rt_list_init(&g_phone_call.thing->properties.list);
    IOT_Thing_AddProperty(&g_phone_call.thing->properties, "phone_number", "电话号码，由数字组成的字符串", kValueTypeString, get_number_property);

    // Define SetVolume method
    rt_kprintf("%s add methods\n", __func__);
    rt_list_init(&g_phone_call.thing->methods.list);
    IOT_Method* method = IOT_Thing_AddMethod(&g_phone_call.thing->methods, "SetNumber", "设置需要拨打的电话号码，设置完成后将自动拨打", set_number_method);
    rt_kprintf("%s add parameters\n", __func__);
    IOT_Thing_AddParameter(&method->parameters, "phone_number", "电话号码，由数字组成的字符串格式", kValueTypeString, true);

    rt_kprintf("%s out\n", __func__);
    return thing;
}

// Thing declaration
//THING_REGISTER(PhoneCall, Speaker_Init);
