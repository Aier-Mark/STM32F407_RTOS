/**
 * @file lv_conf.h
 * Configuration file for v9.1.0
 */

/*
 * Copy this file as `lv_conf.h`
 * 1. simply next to the `lvgl` folder
 * 2. or any other places and
 *    - define `LV_CONF_INCLUDE_SIMPLE`
 *    - add the path as include path
 */

/* clang-format off */
#if 1 /*Set it to "1" to enable content*/

#ifndef LV_CONF_H
#define LV_CONF_H

/*If you need to include anything here, do it inside the `__ASSEMBLY__` guard */
#if  0 && defined(__ASSEMBLY__)
#include "my_include.h"
#endif

/*====================
        颜色设置
 *====================*/

/*颜色深度: 8 (A8), 16 (RGB565), 24 (RGB888), 32 (XRGB8888)*/
#define LV_COLOR_DEPTH 16

/*=========================
   STDLIB WRAPPER SETTINGS
 *=========================*/

/* Possible values
 * - LV_STDLIB_BUILTIN:     LVGL的内置实现
 * - LV_STDLIB_CLIB:        标准C函数，如: malloc, strlen, etc
 * - LV_STDLIB_MICROPYTHON: MicroPython implementation
 * - LV_STDLIB_RTTHREAD:    RT-Thread implementation
 * - LV_STDLIB_CUSTOM:      从外部实现这些功能
 */
#define LV_USE_STDLIB_MALLOC    LV_STDLIB_BUILTIN
#define LV_USE_STDLIB_STRING    LV_STDLIB_BUILTIN
#define LV_USE_STDLIB_SPRINTF   LV_STDLIB_BUILTIN

#if LV_USE_STDLIB_MALLOC == LV_STDLIB_BUILTIN
    /* `lv_mem_alloc()`可获得的内存大小 (以字节为单位)(>= 2kB) */
    #define LV_MEM_SIZE (20 * 1024U)          /*[字节]*/

    /* ' lv_malloc() '的内存扩展大小 (以字节为单位) */
    #define LV_MEM_POOL_EXPAND_SIZE 0

    /* 为内存池设置一个地址，而不是将其作为普通数组分配。也可以在外部SRAM中。 */
    #define LV_MEM_ADR 0     /*0: 未使用*/
    /* 给内存分配器而不是地址，它将被调用来获得LVGL的内存池。例如： my_malloc */
    #if LV_MEM_ADR == 0
        #undef LV_MEM_POOL_INCLUDE
        #undef LV_MEM_POOL_ALLOC
    #endif
#endif  /*LV_USE_STDLIB_MALLOC == LV_STDLIB_BUILTIN*/

/*====================
       HAL 设置
 *====================*/

/*Default display refresh, input device read and animation step period.*/
#define LV_DEF_REFR_PERIOD  33      /*[ms]*/

/*默认每英寸的点数量。用于初始化默认大小，例如小部件大小，样式填充。
 *(不是很重要，你可以调整它来修改默认大小和空格) */
#define LV_DPI_DEF 130     /*[px/inch]*/

/*=================
 *     操作系统
 *=================*/
/*选择要使用的操作系统。可能的选项:
 * - LV_OS_NONE
 * - LV_OS_PTHREAD
 * - LV_OS_FREERTOS
 * - LV_OS_CMSIS_RTOS2
 * - LV_OS_RTTHREAD
 * - LV_OS_WINDOWS
 * - LV_OS_CUSTOM */
#define LV_USE_OS   LV_OS_FREERTOS

#if LV_USE_OS == LV_OS_CUSTOM
    #define LV_OS_CUSTOM_INCLUDE <stdint.h>
#endif

/*========================
 *        显示配置
 *========================*/

/*Align the stride of all layers and images to this bytes*/
#define LV_DRAW_BUF_STRIDE_ALIGN                1

/*Align the start address of draw_buf addresses to this bytes*/
#define LV_DRAW_BUF_ALIGN                       4

/* If a widget has `style_opa < 255` (not `bg_opa`, `text_opa` etc) or not NORMAL blend mode
 * it is buffered into a "simple" layer before rendering. The widget can be buffered in smaller chunks.
 * "Transformed layers" (if `transform_angle/zoom` are set) use larger buffers
 * and can't be drawn in chunks. */

/*The target buffer size for simple layer chunks.*/
#define LV_DRAW_LAYER_SIMPLE_BUF_SIZE    (24 * 1024)   /*[bytes]*/

#define LV_USE_DRAW_SW 1
#if LV_USE_DRAW_SW == 1
    /* Set the number of draw unit.
     * > 1 requires an operating system enabled in `LV_USE_OS`
     * > 1 means multiply threads will render the screen in parallel */
    #define LV_DRAW_SW_DRAW_UNIT_CNT    1

    /* Use Arm-2D to accelerate the sw render */
    #define LV_USE_DRAW_ARM2D_SYNC      0

    /* Enable native helium assembly to be compiled */
    #define LV_USE_NATIVE_HELIUM_ASM    0
    
    /* 0: use a simple renderer capable of drawing only simple rectangles with gradient, images, texts, and straight lines only
     * 1: use a complex renderer capable of drawing rounded corners, shadow, skew lines, and arcs too */
    #define LV_DRAW_SW_COMPLEX          1

    #if LV_DRAW_SW_COMPLEX == 1
        /*Allow buffering some shadow calculation.
        *LV_DRAW_SW_SHADOW_CACHE_SIZE is the max. shadow size to buffer, where shadow size is `shadow_width + radius`
        *Caching has LV_DRAW_SW_SHADOW_CACHE_SIZE^2 RAM cost*/
        #define LV_DRAW_SW_SHADOW_CACHE_SIZE 0

        /* Set number of maximally cached circle data.
        * The circumference of 1/4 circle are saved for anti-aliasing
        * radius * 4 bytes are used per circle (the most often used radiuses are saved)
        * 0: to disable caching */
        #define LV_DRAW_SW_CIRCLE_CACHE_SIZE 4
    #endif

    #define  LV_USE_DRAW_SW_ASM     LV_DRAW_SW_ASM_NONE

    #if LV_USE_DRAW_SW_ASM == LV_DRAW_SW_ASM_CUSTOM
        #define  LV_DRAW_SW_ASM_CUSTOM_INCLUDE ""
    #endif
#endif

/* Use NXP's VG-Lite GPU on iMX RTxxx platforms. */
#define LV_USE_DRAW_VGLITE 0

#if LV_USE_DRAW_VGLITE
    /* Enable blit quality degradation workaround recommended for screen's dimension > 352 pixels. */
    #define LV_USE_VGLITE_BLIT_SPLIT 0

    #if LV_USE_OS
        /* Enable VGLite draw async. Queue multiple tasks and flash them once to the GPU. */
        #define LV_USE_VGLITE_DRAW_ASYNC 1
    #endif

    /* Enable VGLite asserts. */
    #define LV_USE_VGLITE_ASSERT 0
#endif

/* Use NXP's PXP on iMX RTxxx platforms. */
#define LV_USE_DRAW_PXP 0

#if LV_USE_DRAW_PXP
    /* Enable PXP asserts. */
    #define LV_USE_PXP_ASSERT 0
#endif

/* Use Renesas Dave2D on RA  platforms. */
#define LV_USE_DRAW_DAVE2D 0

/* Draw using cached SDL textures*/
#define LV_USE_DRAW_SDL 0

/* Use VG-Lite GPU. */
#define LV_USE_DRAW_VG_LITE 0

#if LV_USE_DRAW_VG_LITE
/* Enable VG-Lite custom external 'gpu_init()' function */
#define LV_VG_LITE_USE_GPU_INIT 0

/* Enable VG-Lite assert. */
#define LV_VG_LITE_USE_ASSERT 0

/* VG-Lite flush commit trigger threshold. GPU will try to batch these many draw tasks. */
#define LV_VG_LITE_FLUSH_MAX_COUNT 8

/* Enable border to simulate shadow
 * NOTE: which usually improves performance,
 * but does not guarantee the same rendering quality as the software. */
#define LV_VG_LITE_USE_BOX_SHADOW 0

/* VG-Lite gradient image maximum cache number.
 * NOTE: The memory usage of a single gradient image is 4K bytes.
 */
#define LV_VG_LITE_GRAD_CACHE_SIZE 32

#endif

/*=======================
 *      功能配置
 *=======================*/

/*-------------
 *     日志
 *-----------*/

/*Enable the log module*/
#define LV_USE_LOG 0
#if LV_USE_LOG

    /*How important log should be added:
    *LV_LOG_LEVEL_TRACE       大量的日志给出了详细的信息
    *LV_LOG_LEVEL_INFO        记录重要事件
    *LV_LOG_LEVEL_WARN        如果发生了一些不想要的事情但没有引起问题，则记录下来
    *LV_LOG_LEVEL_ERROR       只有在系统可能出现故障时才会出现关键问题
    *LV_LOG_LEVEL_USER        仅用户自己添加的日志
    *LV_LOG_LEVEL_NONE        不要记录任何内容 */
    #define LV_LOG_LEVEL LV_LOG_LEVEL_WARN

    /*1: 使用'printf'打印日志;
     *0: 用户需要用' lv_log_register_print_cb() '注册回调函数 */
    #define LV_LOG_PRINTF 0

    /*1: Enable print timestamp;
     *0: Disable print timestamp*/
    #define LV_LOG_USE_TIMESTAMP 1

    /*1: Print file and line number of the log;
     *0: Do not print file and line number of the log*/
    #define LV_LOG_USE_FILE_LINE 1

    /* 在产生大量日志的模块中启用/禁用LV_LOG_TRACE */
    #define LV_LOG_TRACE_MEM        1
    #define LV_LOG_TRACE_TIMER      1
    #define LV_LOG_TRACE_INDEV      1
    #define LV_LOG_TRACE_DISP_REFR  1
    #define LV_LOG_TRACE_EVENT      1
    #define LV_LOG_TRACE_OBJ_CREATE 1
    #define LV_LOG_TRACE_LAYOUT     1
    #define LV_LOG_TRACE_ANIM       1
    #define LV_LOG_TRACE_CACHE      1

#endif  /*LV_USE_LOG*/

/*-------------
 *    断言
 *-----------*/

/* 如果操作失败或发现无效数据，则启用断言。
 * 如果启用了LV_USE_LOG，失败时会打印错误信息*/
#define LV_USE_ASSERT_NULL                  1   /* 检查参数是否为NULL。(非常快,推荐) */
#define LV_USE_ASSERT_MALLOC                1   /* 检查内存是否分配成功。(非常快,推荐) */
#define LV_USE_ASSERT_STYLE                 0   /* 检查样式是否正确初始化。(非常快,推荐) */
#define LV_USE_ASSERT_MEM_INTEGRITY         0   /* 关键操作完成后，请检查“lv_mem”的完整性。(慢)*/
#define LV_USE_ASSERT_OBJ                   0   /* 检查对象的类型和存在(例如，未删除)。(慢) */

/* 当assert发生时，添加一个自定义处理程序，例如重新启动MCU */
#define LV_ASSERT_HANDLER_INCLUDE           <stdint.h>
#define LV_ASSERT_HANDLER while(1);         /* 停止在默认情况下 */

/*-------------
 *    调试
 *-----------*/

/*1: Draw random colored rectangles over the redrawn areas*/
#define LV_USE_REFR_DEBUG 0

/*1: Draw a red overlay for ARGB layers and a green overlay for RGB layers*/
#define LV_USE_LAYER_DEBUG 0

/*1: Draw overlays with different colors for each draw_unit's tasks.
 *Also add the index number of the draw unit on white background.
 *For layers add the index number of the draw unit on black background.*/
#define LV_USE_PARALLEL_DRAW_DEBUG 0

/*-------------
 *    其他
 *-----------*/

#define LV_ENABLE_GLOBAL_CUSTOM 0
#if LV_ENABLE_GLOBAL_CUSTOM
    /*Header to include for the custom 'lv_global' function"*/
    #define LV_GLOBAL_CUSTOM_INCLUDE <stdint.h>
#endif

/*Default cache size in bytes.
 *Used by image decoders such as `lv_lodepng` to keep the decoded image in the memory.
 *If size is not set to 0, the decoder will fail to decode when the cache is full.
 *If size is 0, the cache function is not enabled and the decoded mem will be released immediately after use.*/
#define LV_CACHE_DEF_SIZE       0

/*Default number of image header cache entries. The cache is used to store the headers of images
 *The main logic is like `LV_CACHE_DEF_SIZE` but for image headers.*/
#define LV_IMAGE_HEADER_CACHE_DEF_CNT 0

/*Number of stops allowed per gradient. Increase this to allow more stops.
 *This adds (sizeof(lv_color_t) + 1) bytes per additional stop*/
#define LV_GRADIENT_MAX_STOPS   2

/* Adjust color mix functions rounding. GPUs might calculate color mix (blending) differently.
 * 0: round down, 64: round up from x.75, 128: round up from half, 192: round up from x.25, 254: round up */
#define LV_COLOR_MIX_ROUND_OFS  0

/* Add 2 x 32 bit variables to each lv_obj_t to speed up getting style properties */
#define LV_OBJ_STYLE_CACHE      0

/* Add `id` field to `lv_obj_t` */
#define LV_USE_OBJ_ID           0

/* Use lvgl builtin method for obj ID */
#define LV_USE_OBJ_ID_BUILTIN   0

/*Use obj property set/get API*/
#define LV_USE_OBJ_PROPERTY 0

/* VG-Lite Simulator */
/*Requires: LV_USE_THORVG_INTERNAL or LV_USE_THORVG_EXTERNAL */
#define LV_USE_VG_LITE_THORVG  0

#if LV_USE_VG_LITE_THORVG

    /*Enable LVGL's blend mode support*/
    #define LV_VG_LITE_THORVG_LVGL_BLEND_SUPPORT 0

    /*Enable YUV color format support*/
    #define LV_VG_LITE_THORVG_YUV_SUPPORT 0

    /*Enable 16 pixels alignment*/
    #define LV_VG_LITE_THORVG_16PIXELS_ALIGN 1

    /*Buffer address alignment*/
    #define LV_VG_LITE_THORVG_BUF_ADDR_ALIGN 64

    /*Enable multi-thread render*/
    #define LV_VG_LITE_THORVG_THREAD_RENDER 0

#endif

/*=====================
 *      编译器设置
 *====================*/

/* 对于设置为1的大端序系统 */
#define LV_BIG_ENDIAN_SYSTEM 0

/* 为' lv_tick_inc '函数定义一个自定义属性 */
#define LV_ATTRIBUTE_TICK_INC

/* 为' lv_timer_handler '函数定义一个自定义属性 */
#define LV_ATTRIBUTE_TIMER_HANDLER

/* 为' lv_disp_flush_ready '函数定义一个自定义属性 */
#define LV_ATTRIBUTE_FLUSH_READY

/* 缓冲区所需的对齐大小 */
#define LV_ATTRIBUTE_MEM_ALIGN_SIZE 1

/* 将被添加到需要对齐内存的地方(默认情况下-Os数据可能不会对齐到边界)。
 * E.g. __attribute__((aligned(4)))*/
#define LV_ATTRIBUTE_MEM_ALIGN

/* 属性来标记大型常量数组，例如字体的位图 */
#define LV_ATTRIBUTE_LARGE_CONST

/* RAM中大数组声明的编译器前缀 */
#define LV_ATTRIBUTE_LARGE_RAM_ARRAY

/* 将性能关键功能放入更快的内存中(例如RAM) */
#define LV_ATTRIBUTE_FAST_MEM

/* 导出整型常量到绑定。该宏与LV_<CONST> that形式的常量一起使用
 * 也应该出现在LVGL绑定API，如Micropython。*/
#define LV_EXPORT_CONST_INT(int_value) struct _silence_gcc_warning /* 默认值只是防止GCC警告 */

/*Prefix all global extern data with this*/
#define LV_ATTRIBUTE_EXTERN_DATA

/* Use `float` as `lv_value_precise_t` */
#define LV_USE_FLOAT            0

/*==================
 *      字库设置
 *===================*/

/*Montserrat fonts with ASCII range and some symbols using bpp = 4
 *https://fonts.google.com/specimen/Montserrat*/
#define LV_FONT_MONTSERRAT_8  0
#define LV_FONT_MONTSERRAT_10 0
#define LV_FONT_MONTSERRAT_12 0
#define LV_FONT_MONTSERRAT_14 1
#define LV_FONT_MONTSERRAT_16 0
#define LV_FONT_MONTSERRAT_18 0
#define LV_FONT_MONTSERRAT_20 0
#define LV_FONT_MONTSERRAT_22 0
#define LV_FONT_MONTSERRAT_24 0
#define LV_FONT_MONTSERRAT_26 0
#define LV_FONT_MONTSERRAT_28 0
#define LV_FONT_MONTSERRAT_30 0
#define LV_FONT_MONTSERRAT_32 0
#define LV_FONT_MONTSERRAT_34 0
#define LV_FONT_MONTSERRAT_36 0
#define LV_FONT_MONTSERRAT_38 0
#define LV_FONT_MONTSERRAT_40 0
#define LV_FONT_MONTSERRAT_42 0
#define LV_FONT_MONTSERRAT_44 0
#define LV_FONT_MONTSERRAT_46 0
#define LV_FONT_MONTSERRAT_48 0

/* 展示特色 */
#define LV_FONT_MONTSERRAT_28_COMPRESSED 0  /*bpp = 3*/
#define LV_FONT_DEJAVU_16_PERSIAN_HEBREW 0  /* 希伯来语，阿拉伯语，波斯语以及它们的各种形式 */
#define LV_FONT_SIMSUN_16_CJK            0  /*1000 most common CJK radicals*/

/* 像素完美的等宽字体 */
#define LV_FONT_UNSCII_8  0
#define LV_FONT_UNSCII_16 0

/* 可选声明自定义字体在这里。
 * 你也可以使用这些字体作为默认字体，它们将是全球可用的。
 *E.g. #define LV_FONT_CUSTOM_DECLARE   LV_FONT_DECLARE(my_font_1) LV_FONT_DECLARE(my_font_2)*/
#define LV_FONT_CUSTOM_DECLARE

/* 始终设置默认字体 */
#define LV_FONT_DEFAULT &lv_font_montserrat_14

/* 启用处理大字体和/或带有大量字符的字体。
 * 限制取决于字体大小，字体面和bpp。
 * 编译器错误将被触发，如果字体需要它 */
#define LV_FONT_FMT_TXT_LARGE 0

/* 启用/禁用对压缩字体的支持。 */
#define LV_USE_FONT_COMPRESSED 0

/* 当找不到字形dsc时启用绘图占位符 */
#define LV_USE_FONT_PLACEHOLDER 1

/*=================
 *     文本设置
 *=================*/

/**
 * 为字符串选择字符编码
 * IDE或编辑器应该具有相同的字符编码
 * - LV_TXT_ENC_UTF8
 * - LV_TXT_ENC_ASCII
 */
#define LV_TXT_ENC LV_TXT_ENC_UTF8

/* 可以在这些字符上中断(换行)文本 */
#define LV_TXT_BREAK_CHARS " ,.;:-_)]}"

/* 如果一个单词至少有这么长，就会在“最美”的地方断裂
 * 要禁用，设置一个值<= 0 */
#define LV_TXT_LINE_BREAK_LONG_LEN 0

/* 在一个长单词中，在停顿之前放置一行的最小字符数。
 * 取决于LV_TXT_LINE_BREAK_LONG_LEN。 */
#define LV_TXT_LINE_BREAK_LONG_PRE_MIN_LEN 3

/* 在一个长单词中，在停顿后插入一行的最小字符数。
 * 取决于LV_TXT_LINE_BREAK_LONG_LEN。*/
#define LV_TXT_LINE_BREAK_LONG_POST_MIN_LEN 3

/* 支持双向文本。允许混合从左到右和从右到左的文本。
 * 方向会根据Unicode双向算法进行处理:
 *https://www.w3.org/International/articles/inline-bidi-markup/uba-basics*/
#define LV_USE_BIDI 0
#if LV_USE_BIDI
    /* 设置默认方向。支持的值:
    *`LV_BASE_DIR_LTR` 从左到右
    *`LV_BASE_DIR_RTL` 从右到左
    *`LV_BASE_DIR_AUTO` 检测文本基本方向 */
    #define LV_BIDI_BASE_DIR_DEF LV_BASE_DIR_AUTO
#endif

/* 支持阿拉伯语/波斯处理
 * 在这些语言中，字符应该根据其在文本中的位置被替换为其他形式 */
#define LV_USE_ARABIC_PERSIAN_CHARS 0

/*==================
 *    控件设置
 *================*/

/*Documentation of the widgets: https://docs.lvgl.io/latest/en/html/widgets/index.html*/

#define LV_WIDGETS_HAS_DEFAULT_VALUE  1

#define LV_USE_ANIMIMG    1

#define LV_USE_ARC        1

#define LV_USE_BAR        1

#define LV_USE_BUTTON        1

#define LV_USE_BUTTONMATRIX  1

#define LV_USE_CALENDAR   1
#if LV_USE_CALENDAR
    #define LV_CALENDAR_WEEK_STARTS_MONDAY 0
    #if LV_CALENDAR_WEEK_STARTS_MONDAY
        #define LV_CALENDAR_DEFAULT_DAY_NAMES {"Mo", "Tu", "We", "Th", "Fr", "Sa", "Su"}
    #else
        #define LV_CALENDAR_DEFAULT_DAY_NAMES {"Su", "Mo", "Tu", "We", "Th", "Fr", "Sa"}
    #endif

    #define LV_CALENDAR_DEFAULT_MONTH_NAMES {"January", "February", "March",  "April", "May",  "June", "July", "August", "September", "October", "November", "December"}
    #define LV_USE_CALENDAR_HEADER_ARROW 1
    #define LV_USE_CALENDAR_HEADER_DROPDOWN 1
#endif  /*LV_USE_CALENDAR*/

#define LV_USE_CANVAS     1

#define LV_USE_CHART      1

#define LV_USE_CHECKBOX   1

#define LV_USE_DROPDOWN   1   /* 依赖: lv_label*/

#define LV_USE_IMAGE      1   /* 依赖: lv_label*/

#define LV_USE_IMAGEBUTTON     1

#define LV_USE_KEYBOARD   1

#define LV_USE_LABEL      1
#if LV_USE_LABEL
    #define LV_LABEL_TEXT_SELECTION 1 /* 启用标签的选择文本*/
    #define LV_LABEL_LONG_TXT_HINT 1  /* 在标签中存储一些额外的信息，以加快绘制非常长的文本 */
    #define LV_LABEL_WAIT_CHAR_COUNT 3  /*The count of wait chart*/
#endif

#define LV_USE_LED        1

#define LV_USE_LINE       1

#define LV_USE_LIST       1

#define LV_USE_MENU       1

#define LV_USE_MSGBOX     1

#define LV_USE_ROLLER     1   /* 依赖: lv_label*/

#define LV_USE_SCALE      1

#define LV_USE_SLIDER     1   /* 依赖: lv_bar*/

#define LV_USE_SPAN       1
#if LV_USE_SPAN
    /* 一个行文本可以包含最大数量的span描述符 */
    #define LV_SPAN_SNIPPET_STACK_SIZE 64
#endif

#define LV_USE_SPINBOX    1

#define LV_USE_SPINNER    1

#define LV_USE_SWITCH     1

#define LV_USE_TEXTAREA   1   /* 依赖: lv_label*/
#if LV_USE_TEXTAREA != 0
    #define LV_TEXTAREA_DEF_PWD_SHOW_TIME 1500    /*ms*/
#endif

#define LV_USE_TABLE      1

#define LV_USE_TABVIEW    1

#define LV_USE_TILEVIEW   1

#define LV_USE_WIN        1

/*==================
 *       主题
 *==================*/

/* 一个简单，令人印象深刻，非常完整的主题 */
#define LV_USE_THEME_DEFAULT 1
#if LV_USE_THEME_DEFAULT

    /* 0:光模式;1:黑暗的模式 */
    #define LV_THEME_DEFAULT_DARK 0

    /* 1:按下使能生长 */
    #define LV_THEME_DEFAULT_GROW 1

    /* 默认转换时间[ms] */
    #define LV_THEME_DEFAULT_TRANSITION_TIME 80
#endif /*LV_USE_THEME_DEFAULT*/

/* 一个非常简单的主题，这是一个自定义主题的良好起点 */
#define LV_USE_THEME_SIMPLE 1

/* 专为单色显示器设计的主题 */
#define LV_USE_THEME_MONO 1

/*==================
 *       布局
 *==================*/

/* 类似于CSS中的Flexbox布局 */
#define LV_USE_FLEX 1

/* 类似于CSS中的网格布局 */
#define LV_USE_GRID 1

/*====================
 *      第三方库
 *====================*/

/* 通用api的文件系统接口 */

/*API for fopen, fread, etc*/
#define LV_USE_FS_STDIO 0
#if LV_USE_FS_STDIO
    #define LV_FS_STDIO_LETTER '\0'     /* 设置一个可访问驱动器的大写字母 (e.g. 'A') */
    #define LV_FS_STDIO_PATH ""         /* 设置工作目录。文件/目录路径将被追加到它 */
    #define LV_FS_STDIO_CACHE_SIZE 0    /*>0 在lv_fs_read()中缓存这个字节数 */
#endif

/*API for open, read, etc*/
#define LV_USE_FS_POSIX 0
#if LV_USE_FS_POSIX
    #define LV_FS_POSIX_LETTER '\0'     /* 设置一个可访问驱动器的大写字母 (e.g. 'A') */
    #define LV_FS_POSIX_PATH ""         /* 设置工作目录。文件/目录路径将被追加到它 */
    #define LV_FS_POSIX_CACHE_SIZE 0    /*>0 在lv_fs_read()中缓存这个字节数 */
#endif

/*API for CreateFile, ReadFile, etc*/
#define LV_USE_FS_WIN32 0
#if LV_USE_FS_WIN32
    #define LV_FS_WIN32_LETTER '\0'     /* 设置一个可访问驱动器的大写字母 (e.g. 'A') */
    #define LV_FS_WIN32_PATH ""         /* 设置工作目录。文件/目录路径将被追加到它 */
    #define LV_FS_WIN32_CACHE_SIZE 0    /*>0 在lv_fs_read()中缓存这个字节数 */
#endif

/*API for FATFS (needs to be added separately). Uses f_open, f_read, etc*/
#define LV_USE_FS_FATFS 0
#if LV_USE_FS_FATFS
    #define LV_FS_FATFS_LETTER '\0'     /* 设置一个可访问驱动器的大写字母 (e.g. 'A')*/
    #define LV_FS_FATFS_CACHE_SIZE 0    /*>0 在lv_fs_read()中缓存这个字节数 */
#endif

/*API for memory-mapped file access. */
#define LV_USE_FS_MEMFS 0
#if LV_USE_FS_MEMFS
    #define LV_FS_MEMFS_LETTER '\0'     /* 设置一个可访问驱动器的大写字母 (e.g. 'A')*/
#endif

/*API for LittleFs. */
#define LV_USE_FS_LITTLEFS 0
#if LV_USE_FS_LITTLEFS
    #define LV_FS_LITTLEFS_LETTER '\0'     /* 设置一个可访问驱动器的大写字母 (e.g. 'A')*/
#endif

/*LODEPNG decoder library*/
#define LV_USE_LODEPNG 0

/*PNG decoder(libpng) library*/
#define LV_USE_LIBPNG 0

/*BMP decoder library*/
#define LV_USE_BMP 0

/* JPG + split JPG decoder library.
 * Split JPG is a custom format optimized for embedded systems. */
#define LV_USE_TJPGD 0

/* libjpeg-turbo decoder library.
 * Supports complete JPEG specifications and high-performance JPEG decoding. */
#define LV_USE_LIBJPEG_TURBO 0

/*GIF decoder library*/
#define LV_USE_GIF 0
#if LV_USE_GIF
/*GIF decoder accelerate*/
#define LV_GIF_CACHE_DECODE_DATA 0
#endif


/*Decode bin images to RAM*/
#define LV_BIN_DECODER_RAM_LOAD 0

/*RLE decompress library*/
#define LV_USE_RLE 0

/*QR code library*/
#define LV_USE_QRCODE 0

/*Barcode code library*/
#define LV_USE_BARCODE 0

/*FreeType library*/
#define LV_USE_FREETYPE 0
#if LV_USE_FREETYPE
    /*Let FreeType to use LVGL memory and file porting*/
    #define LV_FREETYPE_USE_LVGL_PORT 0

    /*FreeType中符号的缓存计数。它表示可以缓存的字形数量。
     *该值越高，使用的内存就越多。*/
    #define LV_FREETYPE_CACHE_FT_GLYPH_CNT 256
#endif

/* Built-in TTF decoder */
#define LV_USE_TINY_TTF 0
#if LV_USE_TINY_TTF
    /* Enable loading TTF data from files */
    #define LV_TINY_TTF_FILE_SUPPORT 0
#endif

/*Rlottie library*/
#define LV_USE_RLOTTIE 0

/*Enable Vector Graphic APIs*/
#define LV_USE_VECTOR_GRAPHIC  0

/* Enable ThorVG (vector graphics library) from the src/libs folder */
#define LV_USE_THORVG_INTERNAL 0

/* Enable ThorVG by assuming that its installed and linked to the project */
#define LV_USE_THORVG_EXTERNAL 0

/*Use lvgl built-in LZ4 lib*/
#define LV_USE_LZ4_INTERNAL  0

/*Use external LZ4 library*/
#define LV_USE_LZ4_EXTERNAL  0

/*FFmpeg库的图像解码和播放视频
 *支持所有主要的图像格式，所以不启用其他图像解码器给它*/
#define LV_USE_FFMPEG 0
#if LV_USE_FFMPEG
    /* 将输入信息转储到stderr */
    #define LV_FFMPEG_DUMP_FORMAT 0
#endif

/*==================
 *       其他
 *==================*/

/* 1:启用API对对象进行快照 */
#define LV_USE_SNAPSHOT 0

/*1: 启用系统监控组件 */
#define LV_USE_SYSMON   0
#if LV_USE_SYSMON
    /*Get the idle percentage. E.g. uint32_t my_get_idle(void);*/
    #define LV_SYSMON_GET_IDLE lv_timer_get_idle

    /*1: Show CPU usage and FPS count
     * Requires `LV_USE_SYSMON = 1`*/
    #define LV_USE_PERF_MONITOR 0
    #if LV_USE_PERF_MONITOR
        #define LV_USE_PERF_MONITOR_POS LV_ALIGN_BOTTOM_RIGHT

        /*0: Displays performance data on the screen, 1: Prints performance data using log.*/
        #define LV_USE_PERF_MONITOR_LOG_MODE 0
    #endif

    /*1: Show the used memory and the memory fragmentation
     * Requires `LV_USE_STDLIB_MALLOC = LV_STDLIB_BUILTIN`
     * Requires `LV_USE_SYSMON = 1`*/
    #define LV_USE_MEM_MONITOR 0
    #if LV_USE_MEM_MONITOR
        #define LV_USE_MEM_MONITOR_POS LV_ALIGN_BOTTOM_LEFT
    #endif

#endif /*LV_USE_SYSMON*/

/*1: Enable the runtime performance profiler*/
#define LV_USE_PROFILER 0
#if LV_USE_PROFILER
    /*1: Enable the built-in profiler*/
    #define LV_USE_PROFILER_BUILTIN 1
    #if LV_USE_PROFILER_BUILTIN
        /*Default profiler trace buffer size*/
        #define LV_PROFILER_BUILTIN_BUF_SIZE (16 * 1024)     /*[bytes]*/
    #endif

    /*Header to include for the profiler*/
    #define LV_PROFILER_INCLUDE "lvgl/src/misc/lv_profiler_builtin.h"

    /*Profiler start point function*/
    #define LV_PROFILER_BEGIN    LV_PROFILER_BUILTIN_BEGIN

    /*Profiler end point function*/
    #define LV_PROFILER_END      LV_PROFILER_BUILTIN_END

    /*Profiler start point function with custom tag*/
    #define LV_PROFILER_BEGIN_TAG LV_PROFILER_BUILTIN_BEGIN_TAG

    /*Profiler end point function with custom tag*/
    #define LV_PROFILER_END_TAG   LV_PROFILER_BUILTIN_END_TAG
#endif

/*1: Enable Monkey test*/
#define LV_USE_MONKEY 0

/*1: Enable grid navigation*/
#define LV_USE_GRIDNAV 0

/*1: Enable lv_obj fragment*/
#define LV_USE_FRAGMENT 0

/*1: Support using images as font in label or span widgets */
#define LV_USE_IMGFONT 0

/*1: Enable an observer pattern implementation*/
#define LV_USE_OBSERVER 1

/*1: Enable Pinyin input method*/
/*Requires: lv_keyboard*/
#define LV_USE_IME_PINYIN 0
#if LV_USE_IME_PINYIN
    /*1: Use default thesaurus*/
    /*If you do not use the default thesaurus, be sure to use `lv_ime_pinyin` after setting the thesauruss*/
    #define LV_IME_PINYIN_USE_DEFAULT_DICT 1
    /*Set the maximum number of candidate panels that can be displayed*/
    /*This needs to be adjusted according to the size of the screen*/
    #define LV_IME_PINYIN_CAND_TEXT_NUM 6

    /*Use 9 key input(k9)*/
    #define LV_IME_PINYIN_USE_K9_MODE      1
    #if LV_IME_PINYIN_USE_K9_MODE == 1
        #define LV_IME_PINYIN_K9_CAND_TEXT_NUM 3
    #endif /*LV_IME_PINYIN_USE_K9_MODE*/
#endif

/*1: Enable file explorer*/
/*Requires: lv_table*/
#define LV_USE_FILE_EXPLORER                     0
#if LV_USE_FILE_EXPLORER
    /*Maximum length of path*/
    #define LV_FILE_EXPLORER_PATH_MAX_LEN        (128)
    /*Quick access bar, 1:use, 0:not use*/
    /*Requires: lv_list*/
    #define LV_FILE_EXPLORER_QUICK_ACCESS        1
#endif

/*==================
 *       设备
 *==================*/

/*Use SDL to open window on PC and handle mouse and keyboard*/
#define LV_USE_SDL              0
#if LV_USE_SDL
    #define LV_SDL_INCLUDE_PATH     <SDL2/SDL.h>
    #define LV_SDL_RENDER_MODE      LV_DISPLAY_RENDER_MODE_DIRECT   /*LV_DISPLAY_RENDER_MODE_DIRECT is recommended for best performance*/
    #define LV_SDL_BUF_COUNT        1    /*1 or 2*/
    #define LV_SDL_FULLSCREEN       0    /*1: Make the window full screen by default*/
    #define LV_SDL_DIRECT_EXIT      1    /*1: Exit the application when all SDL windows are closed*/
    #define LV_SDL_MOUSEWHEEL_MODE  LV_SDL_MOUSEWHEEL_MODE_ENCODER  /*LV_SDL_MOUSEWHEEL_MODE_ENCODER/CROWN*/
#endif

/*Use X11 to open window on Linux desktop and handle mouse and keyboard*/
#define LV_USE_X11              0
#if LV_USE_X11
    #define LV_X11_DIRECT_EXIT         1  /*Exit the application when all X11 windows have been closed*/
    #define LV_X11_DOUBLE_BUFFER       1  /*Use double buffers for endering*/
    /*select only 1 of the following render modes (LV_X11_RENDER_MODE_PARTIAL preferred!)*/
    #define LV_X11_RENDER_MODE_PARTIAL 1  /*Partial render mode (preferred)*/
    #define LV_X11_RENDER_MODE_DIRECT  0  /*direct render mode*/
    #define LV_X11_RENDER_MODE_FULL    0  /*Full render mode*/
#endif

/*Driver for /dev/fb*/
#define LV_USE_LINUX_FBDEV      0
#if LV_USE_LINUX_FBDEV
    #define LV_LINUX_FBDEV_BSD           0
    #define LV_LINUX_FBDEV_RENDER_MODE   LV_DISPLAY_RENDER_MODE_PARTIAL
    #define LV_LINUX_FBDEV_BUFFER_COUNT  0
    #define LV_LINUX_FBDEV_BUFFER_SIZE   60
#endif

/*Use Nuttx to open window and handle touchscreen*/
#define LV_USE_NUTTX    0

#if LV_USE_NUTTX
    #define LV_USE_NUTTX_LIBUV    0

    /*Use Nuttx custom init API to open window and handle touchscreen*/
    #define LV_USE_NUTTX_CUSTOM_INIT    0

    /*Driver for /dev/lcd*/
    #define LV_USE_NUTTX_LCD      0
    #if LV_USE_NUTTX_LCD
        #define LV_NUTTX_LCD_BUFFER_COUNT    0
        #define LV_NUTTX_LCD_BUFFER_SIZE     60
    #endif

    /*Driver for /dev/input*/
    #define LV_USE_NUTTX_TOUCHSCREEN    0

#endif

/*Driver for /dev/dri/card*/
#define LV_USE_LINUX_DRM        0

/*Interface for TFT_eSPI*/
#define LV_USE_TFT_ESPI         0

/*Driver for evdev input devices*/
#define LV_USE_EVDEV    0

/*Driver for libinput input devices*/
#define LV_USE_LIBINPUT    0

#if LV_USE_LIBINPUT
    #define LV_LIBINPUT_BSD    0

    /*Full keyboard support*/
    #define LV_LIBINPUT_XKB             0
    #if LV_LIBINPUT_XKB
        /*"setxkbmap -query" can help find the right values for your keyboard*/
        #define LV_LIBINPUT_XKB_KEY_MAP { .rules = NULL, .model = "pc101", .layout = "us", .variant = NULL, .options = NULL }
    #endif
#endif

/*Drivers for LCD devices connected via SPI/parallel port*/
#define LV_USE_ST7735		0
#define LV_USE_ST7789		0
#define LV_USE_ST7796		0
#define LV_USE_ILI9341		0

#define LV_USE_GENERIC_MIPI (LV_USE_ST7735 | LV_USE_ST7789 | LV_USE_ST7796 | LV_USE_ILI9341)

/* LVGL Windows backend */
#define LV_USE_WINDOWS    0

/*==================
*       实例
*==================*/

/* 允许使用库构建示例 */
#define LV_BUILD_EXAMPLES 1

/*===================
 *     演示使用
 ====================*/

/* 显示了一些部件。可能需要增加“LV_MEM_SIZE” */
#define LV_USE_DEMO_WIDGETS 0

/* 演示编码器和键盘的用法 */
#define LV_USE_DEMO_KEYPAD_AND_ENCODER 0

/* 基准系统 */
#define LV_USE_DEMO_BENCHMARK 0

/*Render test for each primitives. Requires at least 480x272 display*/
#define LV_USE_DEMO_RENDER 0

/* LVGL压力测试 */
#define LV_USE_DEMO_STRESS 1

/* 音乐播放器的演示 */
#define LV_USE_DEMO_MUSIC 0
#if LV_USE_DEMO_MUSIC
    #define LV_DEMO_MUSIC_SQUARE    0
    #define LV_DEMO_MUSIC_LANDSCAPE 0
    #define LV_DEMO_MUSIC_ROUND     0
    #define LV_DEMO_MUSIC_LARGE     0
    #define LV_DEMO_MUSIC_AUTO_PLAY 0
#endif

/*Flex layout demo*/
#define LV_USE_DEMO_FLEX_LAYOUT     0

/*Smart-phone like multi-language demo*/
#define LV_USE_DEMO_MULTILANG       0

/*Widget transformation demo*/
#define LV_USE_DEMO_TRANSFORM       0

/*Demonstrate scroll settings*/
#define LV_USE_DEMO_SCROLL          0

/*Vector graphic demo*/
#define LV_USE_DEMO_VECTOR_GRAPHIC  0
/*--END OF LV_CONF_H--*/

#endif /*LV_CONF_H*/

#endif /*End of "Content enable"*/
