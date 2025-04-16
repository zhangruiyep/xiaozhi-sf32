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

#if SOLUTION_WATCH
struct rt_semaphore update_ui_sema;
#else
static struct rt_semaphore update_ui_sema;
/*Create style with the new font*/
static lv_style_t style;
extern const unsigned char droid_sans_fallback_font[];
extern const int droid_sans_fallback_font_size;

extern const lv_image_dsc_t color_neutral;
extern const lv_image_dsc_t color_happy;
extern const lv_image_dsc_t color_laughing;
extern const lv_image_dsc_t color_funny;
extern const lv_image_dsc_t color_sad;
extern const lv_image_dsc_t color_angry;
extern const lv_image_dsc_t color_crying;
extern const lv_image_dsc_t color_loving;
extern const lv_image_dsc_t color_embarrassed;
extern const lv_image_dsc_t color_surprised;
extern const lv_image_dsc_t color_shocked;
extern const lv_image_dsc_t color_thinking;
extern const lv_image_dsc_t color_winking;
extern const lv_image_dsc_t color_cool;
extern const lv_image_dsc_t color_relaxed;
extern const lv_image_dsc_t color_delicious;
extern const lv_image_dsc_t color_kissy;
extern const lv_image_dsc_t color_confident;
extern const lv_image_dsc_t color_sleepy;
extern const lv_image_dsc_t color_silly;
extern const lv_image_dsc_t color_confused;


extern const lv_image_dsc_t ble;//ble
extern const lv_image_dsc_t ble_close;
#endif

static lv_obj_t *global_label1;
static lv_obj_t *global_label2;

static lv_obj_t *global_img1;//emoji
static lv_obj_t *global_img2;
static lv_obj_t *global_img3;
static lv_obj_t *global_img4;
static lv_obj_t *global_img5;
static lv_obj_t *global_img6;
static lv_obj_t *global_img7;
static lv_obj_t *global_img8;
static lv_obj_t *global_img9;
static lv_obj_t *global_img10;
static lv_obj_t *global_img11;
static lv_obj_t *global_img12;
static lv_obj_t *global_img13;
static lv_obj_t *global_img14;
static lv_obj_t *global_img15;
static lv_obj_t *global_img16;
static lv_obj_t *global_img17;
static lv_obj_t *global_img18;
static lv_obj_t *global_img19;
static lv_obj_t *global_img20;
static lv_obj_t *global_img21;


static lv_obj_t *global_img_ble;
static lv_obj_t *global_img_ble_close;

#if SOLUTION_WATCH
typedef struct
{
    lv_obj_t              *bg_page;
    rt_list_t             *list;
} app_xiaozhi_t;
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
void xiaozhi_ui_init_img(lv_obj_t **img, const void *src_img, lv_obj_t *parent)
{
    *img = lv_img_create(parent, NULL);
    lv_img_set_src(*img, src_img);
    lv_obj_align(*img, parent, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_hidden(*img, true);
}
#endif

#if SOLUTION_WATCH
rt_err_t xiaozhi_ui_obj_init(lv_obj_t *bg_page)
#else
rt_err_t xiaozhi_ui_obj_init(void)
#endif
{

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
    set_position_by_percentage(global_label2, 0, 75);
#else
    global_label2 = lv_label_create(lv_screen_active());//output text
    lv_label_set_long_mode(global_label2, LV_LABEL_LONG_WRAP);  /*Break the long lines*/
    lv_obj_add_style(global_label2, &style, 0);
    lv_obj_set_width(global_label2, LV_HOR_RES_MAX);
    lv_obj_set_style_text_align(global_label2,LV_TEXT_ALIGN_CENTER, 0);
    set_position_by_percentage(global_label2, 0, 75);
#endif

#if SOLUTION_WATCH != 1
    LV_IMAGE_DECLARE(color_neutral);
    LV_IMAGE_DECLARE(color_happy);
    LV_IMAGE_DECLARE(color_laughing);
    LV_IMAGE_DECLARE(color_funny);
    LV_IMAGE_DECLARE(color_sad);
    LV_IMAGE_DECLARE(color_angry);
    LV_IMAGE_DECLARE(color_crying);
    LV_IMAGE_DECLARE(color_loving);
    LV_IMAGE_DECLARE(color_embarrassed);
    LV_IMAGE_DECLARE(color_surprised);
    LV_IMAGE_DECLARE(color_shocked);
    LV_IMAGE_DECLARE(color_thinking);
    LV_IMAGE_DECLARE(color_winking);
    LV_IMAGE_DECLARE(color_cool);
    LV_IMAGE_DECLARE(color_relaxed);
    LV_IMAGE_DECLARE(color_delicious);
    LV_IMAGE_DECLARE(color_kissy);
    LV_IMAGE_DECLARE(color_confident);
    LV_IMAGE_DECLARE(color_sleepy);
    LV_IMAGE_DECLARE(color_silly);
    LV_IMAGE_DECLARE(color_confused);

    LV_IMAGE_DECLARE(ble);
    LV_IMAGE_DECLARE(ble_close);
#endif

#if SOLUTION_WATCH
    xiaozhi_ui_init_img(&global_img1, APP_GET_IMG(color_neutral), bg_page);
    xiaozhi_ui_init_img(&global_img2, APP_GET_IMG(color_happy), bg_page);
    xiaozhi_ui_init_img(&global_img3, APP_GET_IMG(color_laughing), bg_page);
    xiaozhi_ui_init_img(&global_img4, APP_GET_IMG(color_funny), bg_page);
    xiaozhi_ui_init_img(&global_img5, APP_GET_IMG(color_sad), bg_page);
    xiaozhi_ui_init_img(&global_img6, APP_GET_IMG(color_angry), bg_page);
    xiaozhi_ui_init_img(&global_img7, APP_GET_IMG(color_crying), bg_page);
    xiaozhi_ui_init_img(&global_img8, APP_GET_IMG(color_loving), bg_page);
    xiaozhi_ui_init_img(&global_img9, APP_GET_IMG(color_embarrassed), bg_page);
    xiaozhi_ui_init_img(&global_img10, APP_GET_IMG(color_surprised), bg_page);
    xiaozhi_ui_init_img(&global_img11, APP_GET_IMG(color_shocked), bg_page);
    xiaozhi_ui_init_img(&global_img12, APP_GET_IMG(color_thinking), bg_page);
    xiaozhi_ui_init_img(&global_img13, APP_GET_IMG(color_winking), bg_page);
    xiaozhi_ui_init_img(&global_img14, APP_GET_IMG(color_cool), bg_page);
    xiaozhi_ui_init_img(&global_img15, APP_GET_IMG(color_relaxed), bg_page);
    xiaozhi_ui_init_img(&global_img16, APP_GET_IMG(color_delicious), bg_page);
    xiaozhi_ui_init_img(&global_img17, APP_GET_IMG(color_kissy), bg_page);
    xiaozhi_ui_init_img(&global_img18, APP_GET_IMG(color_confident), bg_page);
    xiaozhi_ui_init_img(&global_img19, APP_GET_IMG(color_sleepy), bg_page);
    xiaozhi_ui_init_img(&global_img20, APP_GET_IMG(color_silly), bg_page);
    xiaozhi_ui_init_img(&global_img21, APP_GET_IMG(color_confused), bg_page);
    xiaozhi_ui_init_img(&global_img_ble, APP_GET_IMG(ble), bg_page);
    lv_obj_align(global_img_ble, bg_page, LV_ALIGN_IN_TOP_LEFT, 30, 0);
    xiaozhi_ui_init_img(&global_img_ble_close, APP_GET_IMG(ble_close), bg_page);
    lv_obj_align(global_img_ble_close, bg_page, LV_ALIGN_IN_TOP_LEFT, 30, 0);
#else
    global_img1 = lv_img_create(lv_screen_active());
    lv_img_set_src(global_img1, &color_neutral);
    lv_obj_align(global_img1, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_flag(global_img1, LV_OBJ_FLAG_HIDDEN);//hidden

    global_img2 = lv_img_create(lv_screen_active());
    lv_img_set_src(global_img2, &color_happy);
    lv_obj_align(global_img2, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_flag(global_img2, LV_OBJ_FLAG_HIDDEN);//hidden

    global_img3 = lv_img_create(lv_screen_active());
    lv_img_set_src(global_img3, &color_laughing);
    lv_obj_align(global_img3, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_flag(global_img3, LV_OBJ_FLAG_HIDDEN);//hidden

    global_img4 = lv_img_create(lv_screen_active());
    lv_img_set_src(global_img4, &color_funny);
    lv_obj_align(global_img4, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_flag(global_img4, LV_OBJ_FLAG_HIDDEN);//hidden

    global_img5 = lv_img_create(lv_screen_active());
    lv_img_set_src(global_img5, &color_sad);
    lv_obj_align(global_img5, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_flag(global_img5, LV_OBJ_FLAG_HIDDEN);//hidden

    global_img6 = lv_img_create(lv_screen_active());
    lv_img_set_src(global_img6, &color_angry);
    lv_obj_align(global_img6, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_flag(global_img6, LV_OBJ_FLAG_HIDDEN);//hidden

    global_img7 = lv_img_create(lv_screen_active());
    lv_img_set_src(global_img7, &color_crying);
    lv_obj_align(global_img7, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_flag(global_img7, LV_OBJ_FLAG_HIDDEN);//hidden

    global_img8 = lv_img_create(lv_screen_active());
    lv_img_set_src(global_img8, &color_loving);
    lv_obj_align(global_img8, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_flag(global_img8, LV_OBJ_FLAG_HIDDEN);//hidden

    global_img9 = lv_img_create(lv_screen_active());
    lv_img_set_src(global_img9, &color_embarrassed);
    lv_obj_align(global_img9, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_flag(global_img9, LV_OBJ_FLAG_HIDDEN);//hidden

    global_img10 = lv_img_create(lv_screen_active());
    lv_img_set_src(global_img10, &color_surprised);
    lv_obj_align(global_img10, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_flag(global_img10, LV_OBJ_FLAG_HIDDEN);//hidden

    global_img11 = lv_img_create(lv_screen_active());
    lv_img_set_src(global_img11, &color_shocked);
    lv_obj_align(global_img11, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_flag(global_img11, LV_OBJ_FLAG_HIDDEN);//hidden

    global_img12 = lv_img_create(lv_screen_active());
    lv_img_set_src(global_img12, &color_thinking);
    lv_obj_align(global_img12, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_flag(global_img12, LV_OBJ_FLAG_HIDDEN);//hidden

    global_img13 = lv_img_create(lv_screen_active());
    lv_img_set_src(global_img13, &color_winking);
    lv_obj_align(global_img13, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_flag(global_img13, LV_OBJ_FLAG_HIDDEN);//hidden

    global_img14 = lv_img_create(lv_screen_active());
    lv_img_set_src(global_img14, &color_cool);
    lv_obj_align(global_img14, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_flag(global_img14, LV_OBJ_FLAG_HIDDEN);//hidden

    global_img15 = lv_img_create(lv_screen_active());
    lv_img_set_src(global_img15, &color_relaxed);
    lv_obj_align(global_img15, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_flag(global_img15, LV_OBJ_FLAG_HIDDEN);//hidden

    global_img16 = lv_img_create(lv_screen_active());
    lv_img_set_src(global_img16, &color_delicious);
    lv_obj_align(global_img16, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_flag(global_img16, LV_OBJ_FLAG_HIDDEN);//hidden

    global_img17 = lv_img_create(lv_screen_active());
    lv_img_set_src(global_img17, &color_kissy);
    lv_obj_align(global_img17, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_flag(global_img17, LV_OBJ_FLAG_HIDDEN);//hidden

    global_img18 = lv_img_create(lv_screen_active());
    lv_img_set_src(global_img18, &color_confident);
    lv_obj_align(global_img18, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_flag(global_img18, LV_OBJ_FLAG_HIDDEN);//hidden

    global_img19 = lv_img_create(lv_screen_active());
    lv_img_set_src(global_img19, &color_sleepy);
    lv_obj_align(global_img19, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_flag(global_img19, LV_OBJ_FLAG_HIDDEN);//hidden

    global_img20 = lv_img_create(lv_screen_active());
    lv_img_set_src(global_img20, &color_silly);
    lv_obj_align(global_img20, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_flag(global_img20, LV_OBJ_FLAG_HIDDEN);//hidden

    global_img21 = lv_img_create(lv_screen_active());
    lv_img_set_src(global_img21, &color_confused);
    lv_obj_align(global_img21, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_flag(global_img21, LV_OBJ_FLAG_HIDDEN);//hidden

    global_img_ble = lv_img_create(lv_screen_active());
    lv_img_set_src(global_img_ble, &ble);
    lv_obj_align(global_img_ble, LV_ALIGN_TOP_LEFT, 30, 0);
    lv_obj_add_flag(global_img_ble, LV_OBJ_FLAG_HIDDEN);//hidden

    global_img_ble_close = lv_img_create(lv_screen_active());
    lv_img_set_src(global_img_ble_close, &ble_close);
    lv_obj_align(global_img_ble_close, LV_ALIGN_TOP_LEFT, 30, 0);
    lv_obj_add_flag(global_img_ble_close, LV_OBJ_FLAG_HIDDEN);//hidden
#endif
    return RT_EOK;
}

void xiaozhi_ui_chat_status(char *string)//top text
{
    rt_sem_take(&update_ui_sema, RT_WAITING_FOREVER);

    if (string)
    {
        lv_label_set_text(global_label1, string);
    }

    rt_sem_release(&update_ui_sema);
}

void xiaozhi_ui_chat_output(char *string)
{
    rt_sem_take(&update_ui_sema, RT_WAITING_FOREVER);

    if (string)
    {
        lv_label_set_text(global_label2, string);
    }

    rt_sem_release(&update_ui_sema);
}


void xiaozhi_ui_update_emoji(char *string)//emoji
{
    rt_sem_take(&update_ui_sema, RT_WAITING_FOREVER);

#if SOLUTION_WATCH
    lv_obj_set_hidden(global_img1, true);
    lv_obj_set_hidden(global_img2, true);
    lv_obj_set_hidden(global_img3, true);
    lv_obj_set_hidden(global_img4, true);
    lv_obj_set_hidden(global_img5, true);
    lv_obj_set_hidden(global_img6, true);
    lv_obj_set_hidden(global_img7, true);
    lv_obj_set_hidden(global_img8, true);
    lv_obj_set_hidden(global_img9, true);
    lv_obj_set_hidden(global_img10, true);
    lv_obj_set_hidden(global_img11, true);
    lv_obj_set_hidden(global_img12, true);
    lv_obj_set_hidden(global_img13, true);
    lv_obj_set_hidden(global_img14, true);
    lv_obj_set_hidden(global_img15, true);
    lv_obj_set_hidden(global_img16, true);
    lv_obj_set_hidden(global_img17, true);
    lv_obj_set_hidden(global_img18, true);
    lv_obj_set_hidden(global_img19, true);
    lv_obj_set_hidden(global_img20, true);
    lv_obj_set_hidden(global_img21, true);
#else
    lv_obj_add_flag(global_img1, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(global_img2, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(global_img3, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(global_img4, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(global_img5, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(global_img6, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(global_img7, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(global_img8, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(global_img9, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(global_img10, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(global_img11, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(global_img12, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(global_img13, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(global_img14, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(global_img15, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(global_img16, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(global_img17, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(global_img18, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(global_img19, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(global_img20, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(global_img21, LV_OBJ_FLAG_HIDDEN);
#endif

    if(string)
    {
#if SOLUTION_WATCH
        if(strcmp(string,"neutral") == 0)
        {
            lv_obj_set_hidden(global_img1, false);
        }
        else if(strcmp(string,"happy") == 0)
        {
            lv_obj_set_hidden(global_img2, false);
        }
        else if(strcmp(string,"laughing") == 0)
        {
            lv_obj_set_hidden(global_img3, false);
        }
        else if(strcmp(string,"funny") == 0)
        {
            lv_obj_set_hidden(global_img4, false);
        }
        else if(strcmp(string,"sad") == 0)
        {
            lv_obj_set_hidden(global_img5, false);
        }
        else if(strcmp(string,"angry") == 0)
        {
            lv_obj_set_hidden(global_img6, false);
        }
        else if(strcmp(string,"crying") == 0)
        {
            lv_obj_set_hidden(global_img7, false);
        }
        else if(strcmp(string,"loving") == 0)
        {
            lv_obj_set_hidden(global_img8, false);
        }
        else if(strcmp(string,"embarrassed") == 0)
        {
            lv_obj_set_hidden(global_img9, false);
        }
        else if(strcmp(string,"surprised") == 0)
        {
            lv_obj_set_hidden(global_img10, false);
        }
        else if(strcmp(string,"shocked") == 0)
        {
            lv_obj_set_hidden(global_img11, false);
        }
        else if(strcmp(string,"thinking") == 0)
        {
            lv_obj_set_hidden(global_img12, false);
        }
        else if(strcmp(string,"winking") == 0)
        {
            lv_obj_set_hidden(global_img13, false);
        }
        else if(strcmp(string,"cool") == 0)
        {
            lv_obj_set_hidden(global_img14, false);
        }
        else if(strcmp(string,"relaxed") == 0)
        {
            lv_obj_set_hidden(global_img15, false);
        }
        else if(strcmp(string,"delicious") == 0)
        {
            lv_obj_set_hidden(global_img16, false);
        }
        else if(strcmp(string,"kissy") == 0)
        {
            lv_obj_set_hidden(global_img17, false);
        }
        else if(strcmp(string,"confident") == 0)
        {
            lv_obj_set_hidden(global_img18, false);
        }
        else if(strcmp(string,"sleepy") == 0)
        {
            lv_obj_set_hidden(global_img19, false);
        }
        else if(strcmp(string,"silly") == 0)
        {
            lv_obj_set_hidden(global_img20, false);
        }
        else if(strcmp(string,"confused") == 0)
        {
            lv_obj_set_hidden(global_img21, false);
        }
        else
        {
            lv_obj_set_hidden(global_img1, false);//common emoji is neutral
        }
#else
        if(strcmp(string,"neutral") == 0)
        {
            lv_obj_clear_flag(global_img1, LV_OBJ_FLAG_HIDDEN);
        }
        else if(strcmp(string,"happy") == 0)
        {
            lv_obj_clear_flag(global_img2, LV_OBJ_FLAG_HIDDEN);
        }
        else if(strcmp(string,"laughing") == 0)
        {
            lv_obj_clear_flag(global_img3, LV_OBJ_FLAG_HIDDEN);
        }
        else if(strcmp(string,"funny") == 0)
        {
            lv_obj_clear_flag(global_img4, LV_OBJ_FLAG_HIDDEN);
        }
        else if(strcmp(string,"sad") == 0)
        {
            lv_obj_clear_flag(global_img5, LV_OBJ_FLAG_HIDDEN);
        }
        else if(strcmp(string,"angry") == 0)
        {
            lv_obj_clear_flag(global_img6, LV_OBJ_FLAG_HIDDEN);
        }
        else if(strcmp(string,"crying") == 0)
        {
            lv_obj_clear_flag(global_img7, LV_OBJ_FLAG_HIDDEN);
        }
        else if(strcmp(string,"loving") == 0)
        {
            lv_obj_clear_flag(global_img8, LV_OBJ_FLAG_HIDDEN);
        }
        else if(strcmp(string,"embarrassed") == 0)
        {
            lv_obj_clear_flag(global_img9, LV_OBJ_FLAG_HIDDEN);
        }
        else if(strcmp(string,"surprised") == 0)
        {
            lv_obj_clear_flag(global_img10, LV_OBJ_FLAG_HIDDEN);
        }
        else if(strcmp(string,"shocked") == 0)
        {
            lv_obj_clear_flag(global_img11, LV_OBJ_FLAG_HIDDEN);
        }
        else if(strcmp(string,"thinking") == 0)
        {
            lv_obj_clear_flag(global_img12, LV_OBJ_FLAG_HIDDEN);
        }
        else if(strcmp(string,"winking") == 0)
        {
            lv_obj_clear_flag(global_img13, LV_OBJ_FLAG_HIDDEN);
        }
        else if(strcmp(string,"cool") == 0)
        {
            lv_obj_clear_flag(global_img14, LV_OBJ_FLAG_HIDDEN);
        }
        else if(strcmp(string,"relaxed") == 0)
        {
            lv_obj_clear_flag(global_img15, LV_OBJ_FLAG_HIDDEN);
        }
        else if(strcmp(string,"delicious") == 0)
        {
            lv_obj_clear_flag(global_img16, LV_OBJ_FLAG_HIDDEN);
        }
        else if(strcmp(string,"kissy") == 0)
        {
            lv_obj_clear_flag(global_img17, LV_OBJ_FLAG_HIDDEN);
        }
        else if(strcmp(string,"confident") == 0)
        {
            lv_obj_clear_flag(global_img18, LV_OBJ_FLAG_HIDDEN);
        }
        else if(strcmp(string,"sleepy") == 0)
        {
            lv_obj_clear_flag(global_img19, LV_OBJ_FLAG_HIDDEN);
        }
        else if(strcmp(string,"silly") == 0)
        {
            lv_obj_clear_flag(global_img20, LV_OBJ_FLAG_HIDDEN);
        }
        else if(strcmp(string,"confused") == 0)
        {
            lv_obj_clear_flag(global_img21, LV_OBJ_FLAG_HIDDEN);
        }
        else
        {
            lv_obj_clear_flag(global_img1, LV_OBJ_FLAG_HIDDEN);//common emoji is neutral
        }
#endif
    }

    rt_sem_release(&update_ui_sema);
}

void xiaozhi_ui_update_ble(char *string)//ble
{
    rt_sem_take(&update_ui_sema, RT_WAITING_FOREVER);

#if SOLUTION_WATCH
    lv_obj_set_hidden(global_img_ble, true);
    lv_obj_set_hidden(global_img_ble_close, true);

    if(string)
    {
        if(strcmp(string,"open") == 0)
        {
            lv_obj_set_hidden(global_img_ble, false);
        }
        else if(strcmp(string,"close") == 0)
        {
            lv_obj_set_hidden(global_img_ble_close, false);
        }
    }
#else
    lv_obj_add_flag(global_img_ble, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(global_img_ble_close, LV_OBJ_FLAG_HIDDEN);

    if(string)
    {
        if(strcmp(string,"open") == 0)
        {
            lv_obj_clear_flag(global_img_ble, LV_OBJ_FLAG_HIDDEN);
        }
        else if(strcmp(string,"close") == 0)
        {
            lv_obj_clear_flag(global_img_ble_close, LV_OBJ_FLAG_HIDDEN);
        }
    }
#endif

    rt_sem_release(&update_ui_sema);
}




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

    lv_style_init(&style);
    lv_font_t *font = lv_tiny_ttf_create_data(droid_sans_fallback_font, droid_sans_fallback_font_size, 30);
    lv_style_set_text_font(&style, font);
    lv_style_set_text_align(&style, LV_TEXT_ALIGN_CENTER);
    lv_obj_set_style_bg_color(lv_screen_active(), lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);//SET BG COLOR

    ret = xiaozhi_ui_obj_init();
    if (ret != RT_EOK)
    {
        return;
    }

    xiaozhi_ui_update_ble("close");
    xiaozhi_ui_chat_status("connecting...");
    xiaozhi_ui_chat_output("Ready...");
    xiaozhi_ui_update_emoji("neutral");

    while (1)
    {
        if (RT_EOK == rt_sem_trytake(&update_ui_sema))
        {
            ms = lv_task_handler();
            rt_thread_mdelay(ms);
            rt_sem_release(&update_ui_sema);
        }

    }
#endif
}

#if SOLUTION_WATCH
static app_xiaozhi_t *p_xiaozhi = NULL;
static void on_start(void)
{
    p_xiaozhi = (app_xiaozhi_t *)APP_GET_PAGE_MEM_PTR;
    lv_obj_t *cont = lv_basecont_parent_create(lv_scr_act());

    lv_obj_t *title = lvsf_title_create(cont, NULL);
    lvsf_title_set_text(title, app_get_str(key_xiaozhi, "Xiaozhi"));
    lvsf_title_set_visible_item(title, LVSF_TITLE_TITLE | LVSF_TITLE_TIME);

    lv_obj_t *bg_page = lvsf_page_create(cont, NULL);
    lvsf_page_set_scrollbar_mode(bg_page, LV_SCROLLBAR_MODE_OFF);
    lvsf_page_set_size(bg_page, lv_disp_get_hor_res(NULL), lv_disp_get_ver_res(NULL) - lv_obj_get_height(title) - lv_obj_get_y(title));
    lvsf_page_set_align(bg_page, title, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
    lvsf_page_set_status(bg_page, PAGE_STATE_STARTED);
    p_xiaozhi->bg_page = bg_page;
    xiaozhi_ui_obj_init(p_xiaozhi->bg_page);
    lvsf_page_set_defult_pos(p_xiaozhi->bg_page);

    xiaozhi_ui_update_ble("close");
    xiaozhi_ui_chat_status("connecting...");
    xiaozhi_ui_chat_output("Ready...");
    xiaozhi_ui_update_emoji("neutral");
}

static void on_resume(void)
{
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
}

static void on_pause(void)
{
    RT_ASSERT(p_xiaozhi);
    lvsf_page_set_focus_disable(p_xiaozhi->bg_page);
    lvsf_page_set_scrlbar_enable(p_xiaozhi->bg_page, false);
    lvsf_page_set_status(p_xiaozhi->bg_page, PAGE_STATE_PAUSED);
}

static void on_stop(void)
{
    RT_ASSERT(p_xiaozhi);
    p_xiaozhi = NULL;
}

APP_MSG_HANDLER_REGISTER(on_start, on_resume, on_pause, on_stop);
APPLICATION_REGISTER(app_get_strid(key_xiaozhi, "Xiaozhi"), img_siri,
                     "Xiaozhi", sizeof(app_xiaozhi_t));
#endif
