
#include "iot/thing.h"
#include "audio_server.h"

#define TAG "Speaker"

// Speaker structure definition
typedef struct {
    IOT_Thing *thing;
    int standard_volume;
} Speaker;

static Speaker g_speaker = {0};

/* audio_server: 0~15 standard: 0~100 */
int volume_to_standard(uint8_t volume)
{
    return volume * 100 / 15;
}

uint8_t standard_to_volume(int standard)
{
    return (uint8_t)(standard * 15 / 100);
}

// Property getter function
static void * get_volume_property(void) {
    uint8_t volume = audio_server_get_private_volume(AUDIO_TYPE_LOCAL_MUSIC);
    g_speaker.standard_volume = volume_to_standard(volume);
    return &g_speaker.standard_volume;
}

// Method implementation
static void set_volume_method(IOT_ParameterList* parameters) {
    ValueType type;
    int *standard_vol = (int *)IOT_Thing_ParameterList_GetParameter(parameters, "volume", &type);
    if (standard_vol == NULL)
    {
        rt_kprintf("[Speaker] get volume failed\n");
        return;
    }
    uint8_t volume = standard_to_volume(*standard_vol);
    audio_server_set_private_volume(AUDIO_TYPE_LOCAL_MUSIC, volume);
}

// Speaker initialization
IOT_Thing* Speaker_creator(void) {
    rt_kprintf("%s in\n", __func__);
    if (g_speaker.thing == NULL)
    {
        g_speaker.thing = (IOT_Thing*)rt_malloc(sizeof(IOT_Thing));
        if (g_speaker.thing == NULL)
        {
            rt_kprintf("[Speaker] malloc failed\n");
            return NULL;
        }
    }
    IOT_Thing* thing = g_speaker.thing;
    thing->name = "Speaker";
    thing->description = "扬声器";

    // Define volume property
    rt_kprintf("%s add propertys\n", __func__);
    rt_list_init(&g_speaker.thing->properties.list);
    IOT_Thing_AddProperty(&g_speaker.thing->properties, "volume", "当前音量值", kValueTypeNumber, get_volume_property);

    // Define SetVolume method
    rt_kprintf("%s add methods\n", __func__);
    rt_list_init(&g_speaker.thing->methods.list);
    IOT_Method* method = IOT_Thing_AddMethod(&g_speaker.thing->methods, "SetVolume", "设置音量", set_volume_method);
    rt_kprintf("%s add parameters\n", __func__);
    IOT_Thing_AddParameter(&method->parameters, "volume", "音量值,0到100之间的整数", kValueTypeNumber, true);

    rt_kprintf("%s out\n", __func__);
    return thing;
}

// Thing declaration
//THING_REGISTER(Speaker, Speaker_Init);
