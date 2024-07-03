/**
 * @file lv_port_disp_templ.c
 *
 */

/*Copy this file as "lv_port_disp.c" and set this value to "1" to enable content*/
#if 1

/*********************
 *      INCLUDES
 *********************/
#include "lv_port_disp.h"
#include <stdbool.h>
#include "./BSP/LCD/lcd.h" /* 导入 lcd 驱动头文件 */

/*********************
 *      DEFINES
 *********************/
#define USE_SRAM 0 /* 使用外部 sram 为 1，否则为 0 */
#ifdef USE_SRAM
#include "./MALLOC/malloc.h"
#endif

#ifndef MY_DISP_HOR_RES
// #warning Please define or replace the macro MY_DISP_HOR_RES with the actual screen width, default value 320 is used for now.
#define MY_DISP_HOR_RES (800) /* 屏幕宽度 */
#endif

#ifndef MY_DISP_VER_RES
// #warning Please define or replace the macro MY_DISP_VER_RES with the actual screen height, default value 240 is used for now.
#define MY_DISP_VER_RES (480) /* 屏幕高度 */
#endif

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
/* 显示设备初始化函数 */
static void disp_init(void);
/* 显示设备刷新函数 */
static void disp_flush(lv_display_t *disp, const lv_area_t *area, uint8_t *px_map);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

/**
 * @brief       LCD加速绘制函数
 * @param       (sx,sy),(ex,ey):填充矩形对角坐标,区域大小为:(ex - sx + 1) * (ey - sy + 1)
 * @param       color:要填充的颜色
 * @retval      无
 */
void lcd_draw_fast_rgb_color(int16_t sx, int16_t sy,int16_t ex, int16_t ey, uint8_t *color)
{
    uint16_t w = ex-sx+1;
    uint16_t h = ey-sy+1;

    lcd_set_window(sx, sy, w, h);
    uint32_t draw_size = w * h;
    lcd_write_ram_prepare();

    for(uint32_t i = 0; i < draw_size; i++)
    {
        lcd_wr_data(color[i]);
    }
}

/**
 * @brief 初始化并注册显示设备
 * @param 无
 * @retval 无
 */
void lv_port_disp_init(void)
{
    /*-------------------------
     * Initialize your display
     * -----------------------*/
    disp_init();

    /*------------------------------------
     * Create a display and set a flush_cb
     * -----------------------------------*/
    lv_display_t *disp = lv_display_create(MY_DISP_HOR_RES, MY_DISP_VER_RES);
    lv_display_set_flush_cb(disp, disp_flush);

    /* Example 1
     * One buffer for partial rendering*/
    static lv_color_t buf_1_1[MY_DISP_HOR_RES * 10]; /*A buffer for 10 rows*/
    lv_display_set_buffers(disp, buf_1_1, NULL, sizeof(buf_1_1), LV_DISPLAY_RENDER_MODE_PARTIAL);

    /* Example 2
     * Two buffers for partial rendering
     * In flush_cb DMA or similar hardware should be used to update the display in the background.*/
    // static lv_color_t buf_2_1[MY_DISP_HOR_RES * 10];
    // static lv_color_t buf_2_2[MY_DISP_HOR_RES * 10];
    // lv_display_set_buffers(disp, buf_2_1, buf_2_2, sizeof(buf_2_1), LV_DISPLAY_RENDER_MODE_PARTIAL);

    /* Example 3
     * Two buffers screen sized buffer for double buffering.
     * Both LV_DISPLAY_RENDER_MODE_DIRECT and LV_DISPLAY_RENDER_MODE_FULL works, see their comments*/
    // static lv_color_t buf_3_1[MY_DISP_HOR_RES * MY_DISP_VER_RES];
    // static lv_color_t buf_3_2[MY_DISP_HOR_RES * MY_DISP_VER_RES];
    // lv_display_set_buffers(disp, buf_3_1, buf_3_2, sizeof(buf_3_1), LV_DISPLAY_RENDER_MODE_DIRECT);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/**
 * @brief 初始化显示设备和必要的外围设备
 * @param 无
 * @retval 无
 */
static void disp_init(void)
{
    /*You code here*/
    lcd_init();         /* 初始化 LCD */
    lcd_display_dir(1); /* 设置横屏 */
}

volatile bool disp_flush_enabled = true;

/* Enable updating the screen (the flushing process) when disp_flush() is called by LVGL
 */
void disp_enable_update(void)
{
    disp_flush_enabled = true;
}

/* Disable updating the screen (the flushing process) when disp_flush() is called by LVGL
 */
void disp_disable_update(void)
{
    disp_flush_enabled = false;
}

/* 将内部缓冲区的内容刷新到显示屏上的特定区域
 *`px_map` contains the rendered image as raw pixel map and it should be copied to `area` on the display.
 *You can use DMA or any hardware acceleration to do this operation in the background but
 *'lv_display_flush_ready()' has to be called when it's finished.*/
/**
 * @brief 将内部缓冲区的内容刷新到显示屏上的特定区域
 * @note ' px_map '包含渲染图像作为原始像素图，它应该复制到显示器上的' area '
 * @note 可以使用 DMA 或者任何硬件在后台加速执行这个操作
 * 但是，需要在刷新完成后调用函数 'lv_disp_flush_ready()'
 * @param disp_drv : 显示设备
 * @arg area : 要刷新的区域，包含了填充矩形的对角坐标
 * @arg px_map : 颜色数组
 *
 * @retval 无
 */
static void disp_flush(lv_display_t *disp_drv, const lv_area_t *area, uint8_t *px_map)
{
    // if (disp_flush_enabled)
    // {
    //     /* 最简单的情况(但也是最慢的)是将所有像素一个接一个地放到屏幕上 */

    //     int32_t x;
    //     int32_t y;
    //     for (y = area->y1; y <= area->y2; y++)
    //     {
    //         for (x = area->x1; x <= area->x2; x++)
    //         {
    //             /*Put a pixel to the display. For example:*/
    //             /*put_px(x, y, *px_map)*/
    //             px_map++;
    //         }
    //     }
    // }

    /* 在指定区域内填充指定颜色块 */
    // lcd_color_fill(area->x1, area->y1, area->x2, area->y2, px_map);
    lcd_draw_fast_rgb_color(area->x1, area->y1, area->x2, area->y2, px_map);

    /* 重要!!!
     * 通知图形库，已经刷新完毕了 */
    lv_display_flush_ready(disp_drv);
}

#else /*Enable this file at the top*/

/*This dummy typedef exists purely to silence -Wpedantic.*/
typedef int keep_pedantic_happy;
#endif
