#include "rtthread.h"
#include "bf0_hal.h"
#include "drv_io.h"
#include "littlevgl2rtt.h"
#if SOLUTION_WATCH
#include "global.h"
#define _MODULE_NAME_ "xiaozhi"
#include "app_module.h"
#else   //solution use lvgl v7
#include "lv_tiny_ttf.h"
#endif
#include "string.h"

#include "bf0_pm.h"
#include "gui_app_pm.h"
#include "drv_gpio.h"
#if !SOLUTION_WATCH
#include "lv_timer.h"
#include "lv_display.h"
#include "lv_obj_pos.h"
#endif
#include "ulog.h"
#if SOLUTION_WATCH
#undef BSP_USING_PM //disable pm apis
#endif

#define IDLE_TIME_LIMIT  (30000)
#define LCD_DEVICE_NAME  "lcd"

#if SOLUTION_WATCH
struct rt_semaphore update_ui_sema;
#else
static struct rt_semaphore update_ui_sema;
/*Create style with the new font*/
static lv_style_t style;
extern const unsigned char droid_sans_fallback_font[];
extern const int droid_sans_fallback_font_size;

extern const lv_image_dsc_t neutral;
extern const lv_image_dsc_t happy;
extern const lv_image_dsc_t laughing;
extern const lv_image_dsc_t funny;
extern const lv_image_dsc_t sad;
extern const lv_image_dsc_t angry;
extern const lv_image_dsc_t crying;
extern const lv_image_dsc_t loving;
extern const lv_image_dsc_t embarrassed;
extern const lv_image_dsc_t surprised;
extern const lv_image_dsc_t shocked;
extern const lv_image_dsc_t thinking;
extern const lv_image_dsc_t winking;
extern const lv_image_dsc_t cool;
extern const lv_image_dsc_t relaxed;
extern const lv_image_dsc_t delicious;
extern const lv_image_dsc_t kissy;
extern const lv_image_dsc_t confident;
extern const lv_image_dsc_t sleepy;
extern const lv_image_dsc_t silly;
extern const lv_image_dsc_t confused;


extern const lv_image_dsc_t ble;//ble
extern const lv_image_dsc_t ble_close;
#endif

static lv_obj_t *global_label1;
static lv_obj_t *global_label2;

static lv_obj_t *global_img;



static lv_obj_t *global_img_ble;


#if SOLUTION_WATCH
typedef struct
{
    lv_obj_t              *bg_page;
    rt_list_t             *list;
    lv_task_t             *refresh_task;
} app_xiaozhi_t;

static char chat_status[64];
static char chat_output[256];
static char emoji_set[32];
static char emoji_now[32];
#endif

void set_position_by_percentage(lv_obj_t * obj, int x_percent, int y_percent) {
    // Gets the width and height of the screen resolution
    int screen_width = lv_disp_get_hor_res(NULL);
    int screen_height = lv_disp_get_ver_res(NULL);

    // Calculate the sitting of the target position
    int target_x = (screen_width * x_percent) / 100;
    int target_y = (screen_height * y_percent) / 100;

    // Sets the location of the object
    lv_obj_set_pos(obj, target_x, target_y);
}

#if SOLUTION_WATCH
rt_err_t xiaozhi_ui_obj_init(lv_obj_t *bg_page)
#else
rt_err_t xiaozhi_ui_obj_init(void)
#endif
{
#if SOLUTION_WATCH
    global_img_ble = lv_img_create(bg_page, NULL);//ble
    lv_img_set_src(global_img_ble, APP_GET_IMG(ble));
    lv_obj_align(global_img_ble, bg_page, LV_ALIGN_IN_TOP_LEFT, 40, 0);


    global_img = lv_img_create(bg_page, NULL);//emoji
    lv_img_set_src(global_img, APP_GET_IMG(neutral));
    lv_obj_align(global_img, bg_page, LV_ALIGN_CENTER, 0, -40);
#else
    LV_IMAGE_DECLARE(neutral);
    LV_IMAGE_DECLARE(happy);
    LV_IMAGE_DECLARE(laughing);
    LV_IMAGE_DECLARE(funny);
    LV_IMAGE_DECLARE(sad);
    LV_IMAGE_DECLARE(angry);
    LV_IMAGE_DECLARE(crying);
    LV_IMAGE_DECLARE(loving);
    LV_IMAGE_DECLARE(embarrassed);
    LV_IMAGE_DECLARE(surprised);
    LV_IMAGE_DECLARE(shocked);
    LV_IMAGE_DECLARE(thinking);
    LV_IMAGE_DECLARE(winking);
    LV_IMAGE_DECLARE(cool);
    LV_IMAGE_DECLARE(relaxed);
    LV_IMAGE_DECLARE(delicious);
    LV_IMAGE_DECLARE(kissy);
    LV_IMAGE_DECLARE(confident);
    LV_IMAGE_DECLARE(sleepy);
    LV_IMAGE_DECLARE(silly);
    LV_IMAGE_DECLARE(confused);

    LV_IMAGE_DECLARE(ble);
    LV_IMAGE_DECLARE(ble_close);

    global_img_ble = lv_img_create(lv_screen_active());//ble
    lv_img_set_src(global_img_ble, &ble);
    lv_obj_align(global_img_ble, LV_ALIGN_TOP_LEFT, 40, 0);
    

    global_img = lv_img_create(lv_screen_active());//emoji
    lv_img_set_src(global_img, &neutral);
    lv_obj_align(global_img, LV_ALIGN_CENTER, 0, -40);
#endif
    
    
#if SOLUTION_WATCH
    global_label1 = lv_label_create(bg_page, NULL);//top text
    lv_label_set_long_mode(global_label1, LV_LABEL_LONG_SROLL_CIRC);
    lv_obj_set_width(global_label1, 150);
    lv_label_set_align(global_label1, LV_LABEL_ALIGN_CENTER);
    lv_obj_align(global_label1, bg_page, LV_ALIGN_IN_TOP_MID, 0, 0);
#else
    global_label1 = lv_label_create(lv_screen_active());//top text

    lv_label_set_long_mode(global_label1, LV_LABEL_LONG_SCROLL_CIRCULAR); 
    lv_obj_add_style(global_label1, &style, 0);
    lv_obj_set_width(global_label1, 150);
    lv_obj_set_style_text_align(global_label1,LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(global_label1, LV_ALIGN_TOP_MID, 0, 0);
#endif
    

#if SOLUTION_WATCH
    global_label2 = lv_label_create(bg_page, NULL);
    lv_label_set_long_mode(global_label2, LV_LABEL_LONG_BREAK);
    lv_obj_set_width(global_label2, LV_HOR_RES_MAX);
    lv_label_set_align(global_label2, LV_LABEL_ALIGN_CENTER);
    lv_obj_align(global_label2, global_img, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
#else
    global_label2 = lv_label_create(lv_screen_active());//output text

    lv_label_set_long_mode(global_label2, LV_LABEL_LONG_WRAP);  /*Break the long lines*/
    lv_obj_add_style(global_label2, &style, 0);
    lv_obj_set_width(global_label2, LV_HOR_RES_MAX);
    lv_obj_set_style_text_align(global_label2,LV_TEXT_ALIGN_CENTER, 0);
    
    lv_obj_align_to(global_label2, global_img, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
#endif
   
    return RT_EOK;
}

void xiaozhi_ui_chat_status(char *string)//top text
{
    rt_sem_take(&update_ui_sema, RT_WAITING_FOREVER);

    if (string)
    {
#if SOLUTION_WATCH
        strncpy(chat_status, string, sizeof(chat_status));
#else
        lv_label_set_text(global_label1, string);
#endif
    }

    rt_sem_release(&update_ui_sema);
}

void xiaozhi_ui_chat_output(char *string)
{
    rt_sem_take(&update_ui_sema, RT_WAITING_FOREVER);

    if (string)
    {
#if SOLUTION_WATCH
        strncpy(chat_output, string, sizeof(chat_output));
#else
        lv_label_set_text(global_label2, string);
#ifdef BSP_USING_PM
        lv_display_trigger_activity(NULL);
#endif // BSP_USING_PM
#endif
    }

    rt_sem_release(&update_ui_sema);
}

#if SOLUTION_WATCH
typedef struct
{
    const char *img_string;
    const void *src_img;
} emoji_t;

#define XZ_EMOJI_ITEM(x) {#x, APP_GET_IMG(x)}

static emoji_t emoji_list[] =
{
    XZ_EMOJI_ITEM(neutral),
    XZ_EMOJI_ITEM(happy),
    XZ_EMOJI_ITEM(laughing),
    XZ_EMOJI_ITEM(funny),
    XZ_EMOJI_ITEM(sad),
    XZ_EMOJI_ITEM(angry),
    XZ_EMOJI_ITEM(crying),
    XZ_EMOJI_ITEM(loving),
    XZ_EMOJI_ITEM(embarrassed),
    XZ_EMOJI_ITEM(surprised),
    XZ_EMOJI_ITEM(shocked),
    XZ_EMOJI_ITEM(thinking),
    XZ_EMOJI_ITEM(winking),
    XZ_EMOJI_ITEM(cool),
    XZ_EMOJI_ITEM(relaxed),
    XZ_EMOJI_ITEM(delicious),
    XZ_EMOJI_ITEM(kissy),
    XZ_EMOJI_ITEM(confident),
    XZ_EMOJI_ITEM(sleepy),
    XZ_EMOJI_ITEM(silly),
    XZ_EMOJI_ITEM(confused),
};
#endif

void xiaozhi_ui_update_emoji(char *string)//emoji
{
    
    rt_sem_take(&update_ui_sema, RT_WAITING_FOREVER);

#if SOLUTION_WATCH
    strncpy(emoji_set, string, sizeof(emoji_set));
#else
    if (string) {
        if (strcmp(string, "neutral") == 0) {
            lv_img_set_src(global_img, &neutral);
        } else if (strcmp(string, "happy") == 0) {
            lv_img_set_src(global_img, &happy);
        } else if (strcmp(string, "laughing") == 0) {
            lv_img_set_src(global_img, &laughing);
        } else if (strcmp(string, "funny") == 0) {
            lv_img_set_src(global_img, &funny);
        } else if (strcmp(string, "sad") == 0) {
            lv_img_set_src(global_img, &sad);
        } else if (strcmp(string, "angry") == 0) {
            lv_img_set_src(global_img, &angry);
        } else if (strcmp(string, "crying") == 0) {
            lv_img_set_src(global_img, &crying);
        } else if (strcmp(string, "loving") == 0) {
            lv_img_set_src(global_img, &loving);
        } else if (strcmp(string, "embarrassed") == 0) {
            lv_img_set_src(global_img, &embarrassed);
        } else if (strcmp(string, "surprised") == 0) {
            lv_img_set_src(global_img, &surprised);
        } else if (strcmp(string, "shocked") == 0) {
            lv_img_set_src(global_img, &shocked);
        } else if (strcmp(string, "thinking") == 0) {
            lv_img_set_src(global_img, &thinking);
        } else if (strcmp(string, "winking") == 0) {
            lv_img_set_src(global_img, &winking);
        } else if (strcmp(string, "cool") == 0) {
            lv_img_set_src(global_img, &cool);
        } else if (strcmp(string, "relaxed") == 0) {
            lv_img_set_src(global_img, &relaxed);
        } else if (strcmp(string, "delicious") == 0) {
            lv_img_set_src(global_img, &delicious);
        } else if (strcmp(string, "kissy") == 0) {
            lv_img_set_src(global_img, &kissy);
        } else if (strcmp(string, "confident") == 0) {
            lv_img_set_src(global_img, &confident);
        } else if (strcmp(string, "sleepy") == 0) {
            lv_img_set_src(global_img, &sleepy);
        } else if (strcmp(string, "silly") == 0) {
            lv_img_set_src(global_img, &silly);
        } else if (strcmp(string, "confused") == 0) {
            lv_img_set_src(global_img, &confused);
        } else {
            lv_img_set_src(global_img, &neutral); // common emoji is neutral
        }
    }
#endif

    rt_sem_release(&update_ui_sema);
}

void xiaozhi_ui_update_ble(char *string)//ble
{
    rt_sem_take(&update_ui_sema, RT_WAITING_FOREVER);

    if(string)
    {
        if(strcmp(string,"open") == 0)
        {
#if SOLUTION_WATCH
            lv_img_set_src(global_img_ble, APP_GET_IMG(ble));
#else
            lv_img_set_src(global_img_ble, &ble);
#endif
        }
        else if(strcmp(string,"close") == 0)
        {
#if SOLUTION_WATCH
            lv_img_set_src(global_img_ble, APP_GET_IMG(ble_close));
#else
            lv_img_set_src(global_img_ble, &ble_close);
#endif
        }
    }

    rt_sem_release(&update_ui_sema);
}

#ifdef BSP_USING_PM
static rt_device_t lcd_device;
static void pm_event_handler(gui_pm_event_type_t event)
{
    LOG_I("in pm_event_handle"); 
    switch (event)
     {
     case GUI_PM_EVT_SUSPEND:
     {
        LOG_I("in GUI_PM_EVT_SUSPEND"); 
        lv_timer_enable(false);
        break;
     }
     case GUI_PM_EVT_RESUME:
     {
         lv_timer_enable(true);
         break;
     }
     default:
     {
         RT_ASSERT(0);
     }
     }
} 
void pm_ui_init()
{
    
    int8_t wakeup_pin;
    uint16_t gpio_pin;
    GPIO_TypeDef *gpio;

    gpio = GET_GPIO_INSTANCE(34);
    gpio_pin = GET_GPIOx_PIN(34);

    wakeup_pin = HAL_HPAON_QueryWakeupPin(gpio, gpio_pin);
    RT_ASSERT(wakeup_pin >= 0);
    
    lcd_device = rt_device_find(LCD_DEVICE_NAME);
    if(lcd_device==RT_NULL)
    {
        LOG_I("lcd_device!=NULL!");
        RT_ASSERT(0);
    }
    pm_enable_pin_wakeup(wakeup_pin, AON_PIN_MODE_DOUBLE_EDGE);
    gui_ctx_init();
    gui_pm_init(lcd_device, pm_event_handler);

}
#endif

void xiaozhi_ui_task(void *args)
{
    rt_err_t ret = RT_EOK;
    rt_uint32_t ms;


    rt_sem_init(&update_ui_sema, "update_ui", 1, RT_IPC_FLAG_FIFO);

#if !SOLUTION_WATCH
    /* init littlevGL */
    ret = littlevgl2rtt_init("lcd");
    if (ret != RT_EOK)
    {
        return;
    }
    
#ifdef BSP_USING_PM
    pm_ui_init();
#endif
    

    lv_style_init(&style);
    lv_font_t *font = lv_tiny_ttf_create_data(droid_sans_fallback_font, droid_sans_fallback_font_size, 30);
    lv_style_set_text_font(&style, font);
    lv_style_set_text_align(&style, LV_TEXT_ALIGN_CENTER);
    lv_style_set_text_color(&style, lv_color_hex(0xFFFFFF));
    lv_obj_set_style_bg_color(lv_screen_active(), lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);//SET BG COLOR

    ret = xiaozhi_ui_obj_init();
    if (ret != RT_EOK)
    {
        return;
    }

    xiaozhi_ui_update_ble("close");
    xiaozhi_ui_chat_status("连接...");
    xiaozhi_ui_chat_output("ui准备中...");
    xiaozhi_ui_update_emoji("neutral");

    while (1)
    {
        if (RT_EOK == rt_sem_trytake(&update_ui_sema))
        {
            ms = lv_task_handler();

            char * current_text=lv_label_get_text(global_label1);
    /*
            if (current_text) 
            {
                rt_kprintf("Label text: %s\n", current_text);
            }
    */      
#ifdef BSP_USING_PM
            if(strcmp(current_text, "聆听中...") == 0)
            {
                lv_display_trigger_activity(NULL);
            }
            if (lv_display_get_inactive_time(NULL) > IDLE_TIME_LIMIT)
            {
                LOG_I("10s no action \n");
                LOG_I("jzl0:  %x",hwp_rtc->BKP0R);
                LOG_I("jzl1:  %x",hwp_rtc->BKP1R);
                LOG_I("jzl2:  %x",hwp_rtc->BKP2R);
                LOG_I("jzl3:  %x",hwp_rtc->BKP3R);
                LOG_I("jzl4:  %x",hwp_rtc->BKP4R);
                LOG_I("jzl9:  %x",hwp_rtc->BKP9R);
                gui_pm_fsm(GUI_PM_ACTION_SLEEP);
            
            }
        
            if (gui_is_force_close())
            {
                LOG_I("in force_close");
                bool lcd_drawing;
                rt_device_control(lcd_device, RTGRAPHIC_CTRL_GET_BUSY, &lcd_drawing);
                if (!lcd_drawing)
                {
                    LOG_I("no input:%d", lv_display_get_inactive_time(NULL));
                    gui_suspend();
                    LOG_I("ui resume");
                    /* force screen to redraw */
                    lv_obj_invalidate(lv_screen_active());
                    /* reset activity timer */
                    lv_display_trigger_activity(NULL);
    
                }
            
            }
#endif // BSP_USING_PM

            rt_thread_mdelay(ms);
            rt_sem_release(&update_ui_sema);
        }

    }
#endif

}

#if SOLUTION_WATCH
static app_xiaozhi_t *p_xiaozhi = NULL;

extern void xz_button_event_handler(bool is_press);
extern void xz_button_event_handler2(void);
static int32_t xiaozhi_keypad_handler_cb(lv_key_t key, lv_indev_state_t event)
{
    if ((LV_KEY_HOME == key) && (LV_INDEV_STATE_PR == event))
    {
        LOG_D("%s home key pressed", __func__);
        xz_button_event_handler2();
    }
    else if (LV_KEY_NEXT == key)
    {
        //ingore long press time
        if (event != LV_INDEV_STATE_REL)
        {
            event = LV_INDEV_STATE_PR;
        }

        static lv_indev_state_t last_state = LV_INDEV_STATE_REL;
        if (last_state == event)
        {
            return LV_BLOCK_EVENT;
        }
        last_state = event;
        LOG_D("%s last=%d", __func__, last_state);

        if (LV_INDEV_STATE_REL == event)
        {
            LOG_D("%s next key released", __func__);
            xz_button_event_handler(false);
        }
        else
        {
            LOG_D("%s next key pressed", __func__);
            xz_button_event_handler(true);
        }
    }
    return LV_BLOCK_EVENT;
}

void xiaozhi_page_refresh(lv_task_t *task)
{
    if (strcmp(lv_label_get_text(global_label1), chat_status) != 0)
    {
        lv_label_set_text(global_label1, chat_status);
    }

    if (strcmp(lv_label_get_text(global_label2), chat_output) != 0)
    {
        lv_label_set_text(global_label2, chat_output);
    }

    if (strcmp(emoji_set, emoji_now) != 0)
    {
        int i = 0;
        for (i = 0; i < sizeof(emoji_list)/sizeof(emoji_list[0]); i++)
        {
            if (strcmp(emoji_set, emoji_list[i].img_string) == 0) {
                lv_img_set_src(global_img, emoji_list[i].src_img);
                break;
            }
        }
        /* default */
        if (i >= sizeof(emoji_list)/sizeof(emoji_list[0]))
        {
            lv_img_set_src(global_img, emoji_list[0].src_img);
        }
        strcpy(emoji_now, emoji_set);
    }
}

static void on_start(void)
{
    p_xiaozhi = (app_xiaozhi_t *)APP_GET_PAGE_MEM_PTR;
    lv_obj_t *cont = lv_basecont_parent_create(lv_scr_act());

    lv_obj_t *title = lvsf_title_create(cont, NULL);
    lvsf_title_set_text(title, app_get_str(key_xiaozhi, "Xiaozhi"));
    lvsf_title_bottom_set_icons(title, APP_GET_IMG_FROM_APP(symbol, img_title_backspace));
    lvsf_title_set_visible_item(title, LVSF_TITLE_TITLE | LVSF_TITLE_TIME | LVSF_TITLE_BACK_BTN | LVSF_TITLE_BACK_BTN_ICONS);

    lv_obj_t *bg_page = lvsf_page_create(cont, NULL);
    lvsf_page_set_scrollbar_mode(bg_page, LV_SCROLLBAR_MODE_OFF);
    lvsf_page_set_size(bg_page, lv_disp_get_hor_res(NULL), lv_disp_get_ver_res(NULL) - lv_obj_get_height(title) - lv_obj_get_y(title));
    lvsf_page_set_align(bg_page, title, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
    lv_ext_set_local_bg(bg_page, LV_COLOR_WHITE, LV_OPA_COVER);
    lvsf_page_set_status(bg_page, PAGE_STATE_STARTED);
    p_xiaozhi->bg_page = bg_page;
    xiaozhi_ui_obj_init(p_xiaozhi->bg_page);
    lvsf_page_set_defult_pos(p_xiaozhi->bg_page);

    keypad_handler_register(xiaozhi_keypad_handler_cb);

    xiaozhi_ui_update_ble("close");
    xiaozhi_ui_chat_status("connecting...");
    xiaozhi_ui_chat_output("Ready...");
    xiaozhi_ui_update_emoji("neutral");
}

static void on_resume(void)
{
    app_disable_screen_lock_time();
    RT_ASSERT(p_xiaozhi);
    if (PAGE_STATE_STARTED != lvsf_page_get_status(p_xiaozhi->bg_page))
    {
        lv_page_clean(p_xiaozhi->bg_page);
        xiaozhi_ui_obj_init(p_xiaozhi->bg_page);
        lvsf_page_set_defult_pos(p_xiaozhi->bg_page);
        xiaozhi_ui_update_ble("close");
        xiaozhi_ui_chat_status("connecting...");
        xiaozhi_ui_chat_output("Ready...");
        xiaozhi_ui_update_emoji("neutral");
    }
    lvsf_page_set_drag_dir(p_xiaozhi->bg_page, DRAG_DIR_VER);
    lvsf_page_set_focus_enable(p_xiaozhi->bg_page, true);
    lvsf_page_focus_entry(p_xiaozhi->bg_page);
    lvsf_page_set_scrlbar_enable(p_xiaozhi->bg_page, true);
    lvsf_page_set_status(p_xiaozhi->bg_page, PAGE_STATE_RESUMED);
    /* refresh task */
    if (NULL == p_xiaozhi->refresh_task)
    {
        p_xiaozhi->refresh_task = lv_task_create(xiaozhi_page_refresh, APP_REFRESH_PERIOD, LV_TASK_PRIO_LOW, (void *)0);
    }
    xiaozhi_page_refresh(NULL);
}

static void on_pause(void)
{
    RT_ASSERT(p_xiaozhi);
    lvsf_page_set_focus_disable(p_xiaozhi->bg_page);
    lvsf_page_set_scrlbar_enable(p_xiaozhi->bg_page, false);
    lvsf_page_set_status(p_xiaozhi->bg_page, PAGE_STATE_PAUSED);
    app_enable_screen_lock_time();
    if (p_xiaozhi->refresh_task)
    {
        lv_task_del(p_xiaozhi->refresh_task);
        p_xiaozhi->refresh_task = NULL;
    }
}

static void on_stop(void)
{
    keypad_handler_register(NULL);
    RT_ASSERT(p_xiaozhi);
    p_xiaozhi = NULL;
}

APP_MSG_HANDLER_REGISTER(on_start, on_resume, on_pause, on_stop);
APPLICATION_REGISTER(app_get_strid(key_xiaozhi, "Xiaozhi"), img_siri,
                     "Xiaozhi", sizeof(app_xiaozhi_t));
#endif
