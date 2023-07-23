#include  "my_gui.h"
#define scr_act_width() lv_obj_get_width(lv_scr_act())
#define scr_act_height() lv_obj_get_height(lv_scr_act())
LV_IMG_DECLARE(fun_on);
LV_IMG_DECLARE(shuibeng);
LV_IMG_DECLARE(temp);
LV_IMG_DECLARE(LED);
LV_IMG_DECLARE(humi);
LV_IMG_DECLARE(light);
LV_IMG_DECLARE(weather);
lv_obj_t *fan_switch;
lv_obj_t *shuibeng_switch;
lv_obj_t *led_switch;
lv_obj_t *wendu_led;
lv_obj_t *shuibeng_led;
lv_obj_t *light_led;
lv_obj_t *gps_btn;
lv_obj_t *gps_info;
lv_obj_t *beep_btn;
static const lv_font_t* font;
 float tempval=0;
 float humival=0;
 float lightval=0;
 float weatherval=0;
 static float latitude=1;
 static float longtude=2;
 static char city_name[]="daye";
static void fan_cb(lv_event_t *e)
{
    lv_event_code_t code =lv_event_get_code(e);
    if(code==LV_EVENT_VALUE_CHANGED)
    {
        if(lv_obj_has_state(fan_switch,LV_STATE_CHECKED))
        {
            lv_led_on(wendu_led);
            fan=1;
        }
        else{
            lv_led_off(wendu_led);
						fan=0;
        }

    }
}
static void shui_cb(lv_event_t *e)
{
    lv_event_code_t code =lv_event_get_code(e);
    if(code==LV_EVENT_VALUE_CHANGED)
    {
        if(lv_obj_has_state(shuibeng_switch,LV_STATE_CHECKED))
        {
            lv_led_on(shuibeng_led);
            Water=1;
        }
        else{
            lv_led_off(shuibeng_led);
             Water=0;
        }

    }
}
static void led_cb(lv_event_t *e)
{
    lv_event_code_t code =lv_event_get_code(e);
    if(code==LV_EVENT_VALUE_CHANGED)
    {
        if(lv_obj_has_state(led_switch,LV_STATE_CHECKED))
        {
            lv_led_on(light_led);
            LED0=1;
        }
        else{
            lv_led_off(light_led);
            LED0=0;
        }

    }
}
static void gps_get(lv_event_t *e)
{
    lv_event_code_t code =lv_event_get_code(e);
    if(code==LV_EVENT_PRESSED)
    {
            longtude++;
            latitude++;
            printf("get gps\r\n");

    }
    lv_table_set_cell_value_fmt(gps_info,0,1,"%f",latitude);
    lv_table_set_cell_value_fmt(gps_info,1,1,"%f",longtude);
}
void my_gui(void)
{

    font = &lv_font_montserrat_20;
    //选项卡
    lv_obj_t *taball=lv_tabview_create(lv_scr_act(),LV_DIR_TOP,30);
    lv_obj_t *tab1=lv_tabview_add_tab(taball,"Info");
    lv_obj_t *tab2=lv_tabview_add_tab(taball,"Control");
    lv_obj_t *tab3=lv_tabview_add_tab(taball,"GPS");

    //首页选项卡信息
    //温度模块
    lv_obj_t *temp_lable=lv_obj_create(tab1);
    lv_obj_set_size(temp_lable,110,110);
    lv_obj_align(temp_lable,LV_ALIGN_OUT_TOP_LEFT,-5,-5);
    //img
    lv_obj_t *temp_image=lv_img_create(temp_lable);
    lv_img_set_src(temp_image,&temp);
    lv_img_set_zoom(temp_image,64);
    lv_obj_set_pos(temp_image,-50,-50);
    //lable1
    lv_obj_t *temp_info=lv_label_create(temp_lable);
    lv_obj_set_pos(temp_info,40,10);
    lv_label_set_text(temp_info,"Temp");
    //lable2
    lv_obj_t *temp_info2=lv_label_create(temp_lable);
    lv_obj_set_pos(temp_info2,10,50);
    lv_label_set_text_fmt(temp_info2,"%.1f C",tempval);
    lv_obj_set_style_text_font(temp_info2,font,LV_PART_MAIN);


    //湿度模块
    lv_obj_t *hum_lable=lv_obj_create(tab1);
    lv_obj_set_size(hum_lable,110,110);
    lv_obj_set_pos(hum_lable,110,-5);
    //img
    lv_obj_t *hum_image=lv_img_create(hum_lable);
    lv_img_set_src(hum_image,&humi);
    lv_img_set_zoom(hum_image,64);
    lv_obj_set_pos(hum_image,-50,-50);
    //lable1
    lv_obj_t *hum_info=lv_label_create(hum_lable);
    lv_obj_set_pos(hum_info,40,10);
    lv_label_set_text(hum_info,"Humi");
    //lable2
    lv_obj_t *hum_info2=lv_label_create(hum_lable);
    lv_obj_set_pos(hum_info2,10,50);
    lv_label_set_text_fmt(hum_info2,"%.1f %%",humival);
    lv_obj_set_style_text_font(hum_info2,font,LV_PART_MAIN);



    //亮度
    lv_obj_t *Light_lable=lv_obj_create(tab1);
    lv_obj_set_size(Light_lable,110,110);
    lv_obj_set_pos(Light_lable,-5,110);

        //img
    lv_obj_t *light_image=lv_img_create(Light_lable);
    lv_img_set_src(light_image,&light);
    lv_img_set_zoom(light_image,64);
    lv_obj_set_pos(light_image,-50,-50);
    //lable1
    lv_obj_t *light_info=lv_label_create(Light_lable);
    lv_obj_set_pos(light_info,40,10);
    lv_label_set_text(light_info,"Light");
    //lable2
    lv_obj_t *light_info2=lv_label_create(Light_lable);
    lv_obj_set_pos(light_info2,10,50);
    lv_label_set_text_fmt(light_info2,"%.1f %%",lightval);
    lv_obj_set_style_text_font(light_info2,font,LV_PART_MAIN);


   //天气模块
    lv_obj_t *weather_lable=lv_obj_create(tab1);
    lv_obj_set_size(weather_lable,110,110);
    lv_obj_set_pos(weather_lable,110,110);
    //img
    lv_obj_t *weather_image=lv_img_create(weather_lable);
    lv_img_set_src(weather_image,&weather);
    lv_img_set_zoom(weather_image,64);
    lv_obj_set_pos(weather_image,-50,-50);

    lv_obj_t *wea_info=lv_label_create(weather_lable);
    lv_obj_set_pos(wea_info,40,10);
    lv_label_set_text(wea_info,"Weath");

    lv_obj_t *wea_info2=lv_label_create(weather_lable);
    lv_obj_set_pos(wea_info2,10,50);
    lv_label_set_text_fmt(wea_info2,"%s",weatherval);
    lv_obj_set_style_text_font(wea_info2,font,LV_PART_MAIN);

    //控制页面选项卡信息

    lv_obj_t *control_info_lable=lv_obj_create(tab2);
    lv_obj_set_size(control_info_lable,scr_act_width()-20,scr_act_height()/2);
    lv_obj_set_pos(control_info_lable,0,-10);
    //led
    wendu_led=lv_led_create(control_info_lable);
    lv_led_set_color(wendu_led,lv_color_hex(0x1E90FF));
    lv_led_set_brightness(wendu_led,200);
    lv_obj_set_size(wendu_led,50,50);
    //image&labal
    lv_obj_t * fun_lable=lv_img_create(control_info_lable);
    lv_img_set_src(fun_lable,&fun_on);
    lv_img_set_zoom(fun_lable,128);
    lv_obj_update_layout(fun_lable);
    lv_obj_set_pos(fun_lable,-40,30);
    //led
    shuibeng_led=lv_led_create(control_info_lable);
    lv_led_set_color(shuibeng_led,lv_color_hex(0xDC143C));
    lv_led_set_brightness(shuibeng_led,200);
    lv_obj_set_size(shuibeng_led,50,50);
    lv_obj_set_pos(shuibeng_led,(scr_act_width()-20)/3,0);
    //image&lable
    lv_obj_t * shuibeng_lable=lv_img_create(control_info_lable);
    lv_img_set_src(shuibeng_lable,&shuibeng);
    lv_img_set_zoom(shuibeng_lable,128);
    lv_obj_update_layout(shuibeng_lable);
    lv_obj_set_pos(shuibeng_lable,35,30);

    //led
    light_led=lv_led_create(control_info_lable);
    lv_led_set_color(light_led,lv_color_hex(0xFFFF00));
    lv_led_set_brightness(light_led,200);
    lv_obj_set_size(light_led,50,50);
    lv_obj_set_pos(light_led,(scr_act_width()-20)*2/3,0);


    //image&lable
    lv_obj_t * light_lable=lv_img_create(control_info_lable);
    lv_img_set_zoom(light_lable,128);
    lv_img_set_src(light_lable,&LED);
    lv_obj_update_layout(light_lable);
    lv_obj_set_pos(light_lable,110,30);


    //Switch
    lv_obj_t *control_switch_lable2=lv_obj_create(tab2);
    lv_obj_set_size(control_switch_lable2,scr_act_width()-20,scr_act_height()/3);
    lv_obj_set_pos(control_switch_lable2,0,scr_act_height()/2-5);

    fan_switch=lv_switch_create(control_switch_lable2);
    lv_obj_set_align(fan_switch,LV_ALIGN_LEFT_MID);
    lv_obj_add_event_cb(fan_switch,fan_cb,LV_EVENT_VALUE_CHANGED,NULL);

    shuibeng_switch=lv_switch_create(control_switch_lable2);
    lv_obj_set_align(shuibeng_switch,LV_ALIGN_CENTER);
    lv_obj_add_event_cb(shuibeng_switch,shui_cb,LV_EVENT_VALUE_CHANGED,NULL);

    led_switch=lv_switch_create(control_switch_lable2);
    lv_obj_set_align(led_switch,LV_ALIGN_RIGHT_MID);
    lv_obj_add_event_cb(led_switch,led_cb,LV_EVENT_VALUE_CHANGED,NULL);

    //GPS页面
    gps_btn=lv_btn_create(tab3);
    lv_obj_set_size(gps_btn,120,60);
    lv_obj_set_align(gps_btn,LV_ALIGN_BOTTOM_MID);
    lv_obj_set_style_bg_color(gps_btn,lv_color_hex(0xff0000),LV_STATE_PRESSED);
    lv_obj_add_event_cb(gps_btn,gps_get,LV_EVENT_PRESSED,NULL);

    //按钮里面的标签
    lv_obj_t * gps_lable=lv_label_create(gps_btn);
    lv_label_set_text(gps_lable,"Get Pos");
    lv_obj_align(gps_lable,LV_ALIGN_CENTER,0,0);

    gps_info=lv_table_create(tab3);
    lv_table_set_row_cnt(gps_info,3);
    lv_table_set_col_cnt(gps_info,2);
    lv_table_set_col_width(gps_info,0,110);
    lv_table_set_col_width(gps_info,1,110);
    lv_table_set_cell_value(gps_info,0,0,"Latitude");
    lv_table_set_cell_value(gps_info,1,0,"Longitude");
    lv_table_set_cell_value(gps_info,2,0,"city");
    lv_table_set_cell_value_fmt(gps_info,0,1,"%f",latitude);
    lv_table_set_cell_value_fmt(gps_info,1,1,"%f",longtude);
    lv_table_set_cell_value_fmt(gps_info,2,1,"%s",city_name);
}
