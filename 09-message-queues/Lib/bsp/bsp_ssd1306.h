/**
 * @file bsp_ssd1306.h
 */
#ifndef __BSP_SSD1306_H__
#define __BSP_SSD1306_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>

#include "stm32l1xx_hal.h"
#include "bsp_ssd1306_cfg.h"
#include "bsp_ssd1306_fonts.h"

#define SSD1306_WIDTH 128u
#define SSD1306_HEIGHT 64u

#define SSD1306_MUX_RATIO 63u

#define SSD1306_DUMMY_BYTE_OFF 0x00
#define SSD1306_DUMMY_BYTE_ON 0xFF

#define SSD1306_SEG_SIZE 0x08

#define SSD1306_TITLE_HEIGHT 16u

#define SSD1306_BOARD_HEIGHT 40u
#define SSD1306_BOARD_WIDTH 120u
#define SSD1306_BOARD_X_OFFSET 3u
#define SSD1306_BOARD_Y_OFFSET 4u
#define SSD1306_BOARD_CELL_WIDTH 8u
#define SSD1306_BOARD_CELL_HEIGHT 10u

/** Original point to start print char to the board */
#define SSD1306_BOARD_X0 2u
/** Original point to start print char to the board */
#define SSd1306_BOARD_Y0 1u

#define SSD1306_FONT_HEIGHT 8u
#define SSD1306_FONT_WIDTH 6u

uint8_t SSD1306_Buffer[SSD1306_HEIGHT / 8][SSD1306_WIDTH];

typedef enum
{
    SSD1306_OK,
    SSD1306_WRITECMD_FAILED,
    SSD1306_WRITEDATA_FAILED
} SSD1306_ErrorCode_t;

typedef enum
{
    SSD1306_MEM_ADDR_HORIZONTAL_MODE,
    SSD1306_MEM_ADDR_VERTICAL_MODE,
    SSD1306_MEM_ADDR_PAGE_MODE
} SSD1306_MemAddrMode_t;

typedef enum
{
    SSD1306_PAGE_ADDR_0,
    SSD1306_PAGE_ADDR_1,
    SSD1306_PAGE_ADDR_2,
    SSD1306_PAGE_ADDR_3,
    SSD1306_PAGE_ADDR_4,
    SSD1306_PAGE_ADDR_5,
    SSD1306_PAGE_ADDR_6,
    SSD1306_PAGE_ADDR_7
} SSD1306_PageAddr_t;

typedef enum
{
    SSD1306_SCROLL_5_FRAME,
    SSD1306_SCROLL_64_FRAME,
    SSD1306_SCROLL_128_FRAME,
    SSD1306_SCROLL_256_FRAME,
    SSD1306_SCROLL_3_FRAME,
    SSD1306_SCROLL_4_FRAME,
    SSD1306_SCROLL_25_FRAME,
    SSD1306_SCROLL_2_FRAME
} SSD1306_ScrollFrame_t;

typedef enum
{
    SSD1306_VCOMH_065_VCC = 0x00,
    SSD1306_VCOMH_077_VCC = 0x20,
    SSD1306_VCOMH_083_VCC = 0x30
} SSD1306_VCOMHDeselectLevel_t;

typedef enum
{
    SSD1306_NORMAL_DISP,
    SSD1306_INVERSE_DISP
} SSD1306_Inverse_t;

typedef enum
{
    SSD1306_DISP_OFF,
    SSD1306_DISP_ON
} SSD1306_DispState_t;

typedef enum
{
    SSD1306_RIGHT_SCROLL = 0x01,
    SSD1306_LEFT_SCROLL
} SSD1306_HorizontalScrollDirection_t;

typedef enum
{
    SSD1306_SEG_REMAP_NORM,
    SSD1306_SEG_REMAP_INVERSE
} SSD1306_SegmentRemapDirection_t;

typedef enum
{
    SSD1306_FROM_COM0 = 0x00,
    SSD1306_TO_COM0 = 0x08
} SSD1306_COMOutputScanDirection_t;

typedef enum
{
    SSD1306_PIN_CFG_SEQUENT = 0x02,
    SSD1306_PIN_CFG_ALTER = 0x12
} SSD1306_PinConfig_t;

typedef enum
{
    SSD1306_COM_REMAP_DISABLE = 0x02,
    SSD1306_COM_REMAP_ENABLE = 0x22
} SSD1306_COMRemap_t;

typedef enum
{
    SSD1306_CHARGE_BUMP_DISABLE = 0x10,
    SSD1306_CHARGE_BUMP_ENABLE = 0x14
} SSD1306_ChargeBumpState_t;

typedef enum
{
    SSD1306_PIXEL_OFF,
    SSD1306_PIXEL_ON
} SSD1306_PixelState_t;

#define SSD1306_CONSTRAST

typedef enum
{
    SSD1306_COL_0,
    SSD1306_COL_1,
    SSD1306_COL_2,
    SSD1306_COL_3,
    SSD1306_COL_4,
    SSD1306_COL_5,
    SSD1306_COL_6,
    SSD1306_COL_7,
    SSD1306_COL_8,
    SSD1306_COL_9,
    SSD1306_COL_10,
    SSD1306_COL_11,
    SSD1306_COL_12,
    SSD1306_COL_13,
    SSD1306_COL_14,
    SSD1306_COL_15,
    SSD1306_COL_16,
    SSD1306_COL_17,
    SSD1306_COL_18,
    SSD1306_COL_19,
    SSD1306_COL_20,
    SSD1306_COL_21,
    SSD1306_COL_22,
    SSD1306_COL_23,
    SSD1306_COL_24,
    SSD1306_COL_25,
    SSD1306_COL_26,
    SSD1306_COL_27,
    SSD1306_COL_28,
    SSD1306_COL_29,
    SSD1306_COL_30,
    SSD1306_COL_31,
    SSD1306_COL_32,
    SSD1306_COL_33,
    SSD1306_COL_34,
    SSD1306_COL_35,
    SSD1306_COL_36,
    SSD1306_COL_37,
    SSD1306_COL_38,
    SSD1306_COL_39,
    SSD1306_COL_40,
    SSD1306_COL_41,
    SSD1306_COL_42,
    SSD1306_COL_43,
    SSD1306_COL_44,
    SSD1306_COL_45,
    SSD1306_COL_46,
    SSD1306_COL_47,
    SSD1306_COL_48,
    SSD1306_COL_49,
    SSD1306_COL_50,
    SSD1306_COL_51,
    SSD1306_COL_52,
    SSD1306_COL_53,
    SSD1306_COL_54,
    SSD1306_COL_55,
    SSD1306_COL_56,
    SSD1306_COL_57,
    SSD1306_COL_58,
    SSD1306_COL_59,
    SSD1306_COL_60,
    SSD1306_COL_61,
    SSD1306_COL_62,
    SSD1306_COL_63,
    SSD1306_COL_64,
    SSD1306_COL_65,
    SSD1306_COL_66,
    SSD1306_COL_67,
    SSD1306_COL_68,
    SSD1306_COL_69,
    SSD1306_COL_70,
    SSD1306_COL_71,
    SSD1306_COL_72,
    SSD1306_COL_73,
    SSD1306_COL_74,
    SSD1306_COL_75,
    SSD1306_COL_76,
    SSD1306_COL_77,
    SSD1306_COL_78,
    SSD1306_COL_79,
    SSD1306_COL_80,
    SSD1306_COL_81,
    SSD1306_COL_82,
    SSD1306_COL_83,
    SSD1306_COL_84,
    SSD1306_COL_85,
    SSD1306_COL_86,
    SSD1306_COL_87,
    SSD1306_COL_88,
    SSD1306_COL_89,
    SSD1306_COL_90,
    SSD1306_COL_91,
    SSD1306_COL_92,
    SSD1306_COL_93,
    SSD1306_COL_94,
    SSD1306_COL_95,
    SSD1306_COL_96,
    SSD1306_COL_97,
    SSD1306_COL_98,
    SSD1306_COL_99,
    SSD1306_COL_100,
    SSD1306_COL_101,
    SSD1306_COL_102,
    SSD1306_COL_103,
    SSD1306_COL_104,
    SSD1306_COL_105,
    SSD1306_COL_106,
    SSD1306_COL_107,
    SSD1306_COL_108,
    SSD1306_COL_109,
    SSD1306_COL_110,
    SSD1306_COL_111,
    SSD1306_COL_112,
    SSD1306_COL_113,
    SSD1306_COL_114,
    SSD1306_COL_115,
    SSD1306_COL_116,
    SSD1306_COL_117,
    SSD1306_COL_118,
    SSD1306_COL_119,
    SSD1306_COL_120,
    SSD1306_COL_121,
    SSD1306_COL_122,
    SSD1306_COL_123,
    SSD1306_COL_124,
    SSD1306_COL_125,
    SSD1306_COL_126,
    SSD1306_COL_127,
} SSD1306_Col_t;

typedef enum
{
    SSD1306_ROW_0,
    SSD1306_ROW_1,
    SSD1306_ROW_2,
    SSD1306_ROW_3,
    SSD1306_ROW_4,
    SSD1306_ROW_5,
    SSD1306_ROW_6,
    SSD1306_ROW_7,
    SSD1306_ROW_8,
    SSD1306_ROW_9,
    SSD1306_ROW_10,
    SSD1306_ROW_11,
    SSD1306_ROW_12,
    SSD1306_ROW_13,
    SSD1306_ROW_14,
    SSD1306_ROW_15,
    SSD1306_ROW_16,
    SSD1306_ROW_17,
    SSD1306_ROW_18,
    SSD1306_ROW_19,
    SSD1306_ROW_20,
    SSD1306_ROW_21,
    SSD1306_ROW_22,
    SSD1306_ROW_23,
    SSD1306_ROW_24,
    SSD1306_ROW_25,
    SSD1306_ROW_26,
    SSD1306_ROW_27,
    SSD1306_ROW_28,
    SSD1306_ROW_29,
    SSD1306_ROW_30,
    SSD1306_ROW_31,
    SSD1306_ROW_32,
    SSD1306_ROW_33,
    SSD1306_ROW_34,
    SSD1306_ROW_35,
    SSD1306_ROW_36,
    SSD1306_ROW_37,
    SSD1306_ROW_38,
    SSD1306_ROW_39,
    SSD1306_ROW_40,
    SSD1306_ROW_41,
    SSD1306_ROW_42,
    SSD1306_ROW_43,
    SSD1306_ROW_44,
    SSD1306_ROW_45,
    SSD1306_ROW_46,
    SSD1306_ROW_47,
    SSD1306_ROW_48,
    SSD1306_ROW_49,
    SSD1306_ROW_50,
    SSD1306_ROW_51,
    SSD1306_ROW_52,
    SSD1306_ROW_53,
    SSD1306_ROW_54,
    SSD1306_ROW_55,
    SSD1306_ROW_56,
    SSD1306_ROW_57,
    SSD1306_ROW_58,
    SSD1306_ROW_59,
    SSD1306_ROW_60,
    SSD1306_ROW_61,
    SSD1306_ROW_62,
    SSD1306_ROW_63,
} SSD1306_Row_t;

typedef enum
{
    SSD1306_MUX_RATIO_15,
    SSD1306_MUX_RATIO_16,
    SSD1306_MUX_RATIO_17,
    SSD1306_MUX_RATIO_18,
    SSD1306_MUX_RATIO_19,
    SSD1306_MUX_RATIO_20,
    SSD1306_MUX_RATIO_21,
    SSD1306_MUX_RATIO_22,
    SSD1306_MUX_RATIO_23,
    SSD1306_MUX_RATIO_24,
    SSD1306_MUX_RATIO_25,
    SSD1306_MUX_RATIO_26,
    SSD1306_MUX_RATIO_27,
    SSD1306_MUX_RATIO_28,
    SSD1306_MUX_RATIO_29,
    SSD1306_MUX_RATIO_30,
    SSD1306_MUX_RATIO_31,
    SSD1306_MUX_RATIO_32,
    SSD1306_MUX_RATIO_33,
    SSD1306_MUX_RATIO_34,
    SSD1306_MUX_RATIO_35,
    SSD1306_MUX_RATIO_36,
    SSD1306_MUX_RATIO_37,
    SSD1306_MUX_RATIO_38,
    SSD1306_MUX_RATIO_39,
    SSD1306_MUX_RATIO_40,
    SSD1306_MUX_RATIO_41,
    SSD1306_MUX_RATIO_42,
    SSD1306_MUX_RATIO_43,
    SSD1306_MUX_RATIO_44,
    SSD1306_MUX_RATIO_45,
    SSD1306_MUX_RATIO_46,
    SSD1306_MUX_RATIO_47,
    SSD1306_MUX_RATIO_48,
    SSD1306_MUX_RATIO_49,
    SSD1306_MUX_RATIO_50,
    SSD1306_MUX_RATIO_51,
    SSD1306_MUX_RATIO_52,
    SSD1306_MUX_RATIO_53,
    SSD1306_MUX_RATIO_54,
    SSD1306_MUX_RATIO_55,
    SSD1306_MUX_RATIO_56,
    SSD1306_MUX_RATIO_57,
    SSD1306_MUX_RATIO_58,
    SSD1306_MUX_RATIO_59,
    SSD1306_MUX_RATIO_60,
    SSD1306_MUX_RATIO_61,
    SSD1306_MUX_RATIO_62,
    SSD1306_MUX_RATIO_63,
} SSD1306_MUXRatio_t;

typedef enum
{
    SSD1306_CLK_RATIO_0,
    SSD1306_CLK_RATIO_1,
    SSD1306_CLK_RATIO_2,
    SSD1306_CLK_RATIO_3,
    SSD1306_CLK_RATIO_4,
    SSD1306_CLK_RATIO_5,
    SSD1306_CLK_RATIO_6,
    SSD1306_CLK_RATIO_7,
    SSD1306_CLK_RATIO_8,
    SSD1306_CLK_RATIO_9,
    SSD1306_CLK_RATIO_10,
    SSD1306_CLK_RATIO_11,
    SSD1306_CLK_RATIO_12,
    SSD1306_CLK_RATIO_13,
    SSD1306_CLK_RATIO_14,
    SSD1306_CLK_RATIO_15,
} SSD1306_ClkRatio_t;

typedef enum
{
    SSD1306_CLK_FRQ_0,
    SSD1306_CLK_FRQ_1,
    SSD1306_CLK_FRQ_2,
    SSD1306_CLK_FRQ_3,
    SSD1306_CLK_FRQ_4,
    SSD1306_CLK_FRQ_5,
    SSD1306_CLK_FRQ_6,
    SSD1306_CLK_FRQ_7,
    SSD1306_CLK_FRQ_8,
    SSD1306_CLK_FRQ_9,
    SSD1306_CLK_FRQ_10,
    SSD1306_CLK_FRQ_11,
    SSD1306_CLK_FRQ_12,
    SSD1306_CLK_FRQ_13,
    SSD1306_CLK_FRQ_14,
    SSD1306_CLK_FRQ_15,
} SSD1306_ClkFrq_t;

typedef enum
{
    SSD1306_PHASE_1 = 1,
    SSD1306_PHASE_2,
    SSD1306_PHASE_3,
    SSD1306_PHASE_4,
    SSD1306_PHASE_5,
    SSD1306_PHASE_6,
    SSD1306_PHASE_7,
    SSD1306_PHASE_8,
    SSD1306_PHASE_9,
    SSD1306_PHASE_10,
    SSD1306_PHASE_11,
    SSD1306_PHASE_12,
    SSD1306_PHASE_13,
    SSD1306_PHASE_14,
    SSD1306_PHASE_15,
} SSD1306_Phase_t;

/**
 * @brief this function changes the constrast value of  the display
 *
 * @param[in] constrast should be in range [0x00 - 0xFF]
 */
void SSD1306_SetConstrast(uint8_t constrast);

/**
 * @brief this function turns entire display ON and resume to RAM content
 * display. Output follows RAM content.
 */
void SSD1306_EntireDispOnSoft(void);

/**
 * @brief this funtion turns entire display ON. Output ignores RAM content
 */
void SSD1306_EntireDispOnHard(void);

/**
 * @brief this function inverse display color
 *
 * @param[in] state \n
 * - SSD1306_NORMAL_DISP: 0 in RAM -> OFF in Display \n
 * - SSD1306_INVERSE_DISP: 0 in RAM -> ON in Display
 */
void SSD1306_InverseDisp(SSD1306_Inverse_t state);

/**
 * @brief turn display ON or OFF
 *
 * @param[in] state \n
 * - SSD1306_DISP_OFF: display OFF \n
 * - SSD1306_DISP_ON: display ON
 */
void SSD1306_DispOnOff(SSD1306_DispState_t state);

/**
 * @brief this function setups the horizontal scroll parameters and
 * determines the scrolling start page, end page and scrolling spped
 *
 * @note before calling this function the horizontal scroll must be
 * deactived. Otherwise, RAM content may be corrupted
 *
 * @param[in] direct \n
 * - SSD1306_RIGHT_SCROLL: Right Horizontal Scroll \n
 * - SSD1306_LEFT_SCROLL: Left Horizontal Scroll
 * @param[in] start_page
 * @param[in] frame set time interval between each scroll step in terms of
 * frame frequency
 * @param[in] end_page
 *
 */
void SSD1306_SetupContHorizontalScroll(
    SSD1306_HorizontalScrollDirection_t direct,
    SSD1306_PageAddr_t start_page, SSD1306_ScrollFrame_t frame,
    SSD1306_PageAddr_t end_page);

/**
 * @brief this function setups the continuous veritcal scroll parameters and
 * determines the scrolling start page, end page, scrolling speed and
 * vertical scrolling offset.
 *
 * @note before call this function the scroll must be deactivated.
 * Otherwise, RAM content may be corrupted.
 *
 * @param[in] direct \n
 * - SSD1306_RIGHT_SCROLL:	Vertical and Right Horizontal Scroll \n
 * - SSD1306_LEFT_SCROLL: Vertical and Left Horizontal Scroll
 * @param[in] start_page
 * @param[in] end_page
 * @param[in] offset
 */
void SSD1306_SetupContHorizontalVerticalScroll(
    SSD1306_HorizontalScrollDirection_t direct,
    SSD1306_PageAddr_t start_page, SSD1306_ScrollFrame_t frame,
    SSD1306_PageAddr_t end_page, SSD1306_Row_t offset);

/**
 * @brief this function deactivates scrolling
 *
 * @note should be called before calling SSD1306_SetupContHorizontalScroll()
 * and SSD1306_SetupContHorizontalVerticalScroll() function.
 */
void SSD1306_DeactiveScroll(void);

/**
 * @brief this function activate scrollling
 *
 * @note should be called after calling SSD1306_SetupContHorizontalScroll()
 * and SSD1306_SetupContHorizontalVerticalScroll() function.
 */
void SSD1306_ActiveScroll(void);

/**
 * @brief set up the vertical scroll area \n
 * - (n_fixed_row == 0 && n_scroll_row == mux ratio) -> whole area scrolls
 * \n
 * - (n_fixed_row == 0 && n_scroll_row < mux ratio) -> top area scrolls \n
 * - (n_fixed_row + n_scroll_row) < mux ratio -> central area scrolls \n
 * - (n_fixed_row + n_scroll_row) == mux ratio -> bottom area scrolls
 *
 * @param[in] n_fixed_row: number of rows in top fixed area
 * @param[in] n_scroll_row: number of rows in scroll area
 *
 * @note
 * - n_fixed_row + n_scroll_row <= multiplex ratio \n
 * - n_scroll_row < multiplex ratio
 */
void SSD1306_SetVerticalScrollArea(SSD1306_Row_t n_fixed_row,
                                   SSD1306_Row_t n_scroll_row);

/**
 * @brief this function sets memory addressing mode
 *
 * @param[in] mode \n
 * - SSD1306_MEM_ADDR_HORIZONTAL_MODE: Horizontal addressing mode \n
 * - SSD1306_MEM_ADDR_VERTICAL_MODE: Vertical addressing mode \n
 * - SSD1306_MEM_ADDR_PAGE_MODE: Page addressing mode
 */
void SSD1306_SetMemAddrMode(SSD1306_MemAddrMode_t mode);

/**
 * @brief this function set column start address and end adress of the
 * display data RAM.
 *
 * @note ONLY use for Horizontal or Vertical Addressing Mode
 *
 * @param[in] col_start should be in range [0-127]
 * @param[in] col_end should be in range [0-127]
 */
void SSD1306_SetColAddr(SSD1306_Col_t col_start, SSD1306_Col_t col_end);

/**
 * @brief this function set start page address and end page address of the
 * display data RAM.
 *
 * @note ONLY use for Horizontal or Vertical Addressing Mode
 *
 * @param[in] start_page should be in range [0-7]
 * @param[in] end_page should be in range [0-7]
 */
void SSD1306_SetPageAddr(SSD1306_PageAddr_t start_page,
                         SSD1306_PageAddr_t end_page);

/**
 * @brief this function set start page address and column(segment) start
 * address
 *
 * @note ONLY use for Page Addressing Mode
 *
 * @param[in] start_page should be in range [0-7]
 * @param[in] seg should be in range [0-127]
 */
void SSD1306_SetPageStartAddr(SSD1306_PageAddr_t start_page,
                              SSD1306_Col_t seg);

/**
 * @brief this function sets the Display Start Line register to
 * determine starting address of display RAM, by selecting a value from
 * 0 to 63. With value = 0, RAM row 0 is mapped to COM0. With value = 1,
 * RAM row 1 is mapped to COM0 and so on.
 *
 * @param[in] start_line should be in range [0-63]
 */
void SSD1306_SetDispStartLine(SSD1306_Row_t start_line);

/**
 * @brief this function changes the mapping between the display data column
 * address and the segment driver.
 *
 * @param[in] direction \n
 * - SSD1306_SEG_REMAP_NORM: column address 0 is mapped to SEG0\n
 * - SSD1306_SEG_REMAP_INVERSE: column address 127 is mapped to SEG0
 */
void SSD1306_SetSegRemap(SSD1306_SegmentRemapDirection_t direction);

/**
 * @brief this function switches the default 63 multiplex mode to any
 * multiplex mode, ranging from 16 to 63. The output pads COM0~COM63 will be
 * switched to the coressponding COM signal
 *
 * @param[in] ratio should be in range [16-63]
 */
void SSD1306_SetMultiplexRatio(SSD1306_MUXRatio_t ratio);

/**
 * @brief this function sets the scan direction of the COM output.
 *
 * @param[in] direction \n
 * - SSD1306_FROM_COM0: Normal mode. Scan from COM0 to COM[N-1]\n
 * - SSD1306_TO_COM0: Remapped mode. Scan from COM[N-1] to COM0
 */
void SSD1306_SetCOMOutPutScanDirection(
    SSD1306_COMOutputScanDirection_t direction);

/**
 * @brief this function sets vertical shift by COM from 0 to 63
 *
 * @paramp[in] offset should be in range [0-63]
 */
void SSD1306_SetDispOffet(SSD1306_Row_t offset);

/**
 * @brief this function sets COM Pins Hardware Configuration
 *
 * @param[in] cfg \n
 * - SSD1306_PIN_CFG_SEQUENT: sequential COM pin configuration \n
 * - SSD1306_PIN_CFG_ALTER: alternative COM pin configuration
 * @param[in] remap \n
 * - SSD1306_COM_REMAP_DISABLE: disable COM Left/Right remap \n
 * - SSD1306_COM_REMAP_ENABLE: enable COM Left/Right remap
 */
void SSD1306_SetHardwareCfg(SSD1306_PinConfig_t cfg,
                            SSD1306_COMRemap_t remap);
/**
 * @brief this function sets display clock divide ratio and oscillator
 * frequency
 *
 * @param[in] ratio should be in range [0-15]
 * @param[in] frq should be in range [0-15]
 */
void SSD1306_ClkCfg(SSD1306_ClkRatio_t ratio, SSD1306_ClkFrq_t frq);

/**
 * @brief this function sets the duration of the pre-charge period. This
 * interval is counted in number of display clock.
 *
 * @param[in] phase1_period should be in range [0-15]
 * @param[in] phase2_period should be in range [0-15]
 */
void SSD1306_SetPrechargePeriod(SSD1306_Phase_t phase1_period,
                                SSD1306_Phase_t phase2_period);

/**
 * @brief this function adjust the VCOMH regulator output
 *
 * @param[in] level \n
 * - SSD1306_VCOMH_065_VCC \n
 * - SSD1306_VCOMH_077_VCC \n
 * - SSD1306_VCOMH_083_VCC
 */
void SSD1306_SetVCOMHDeselectLevel(SSD1306_VCOMHDeselectLevel_t level);

/**
 * @brief this function turns the internal regulator circuit on/off.
 *
 * @note The internal regulator circuit in SSD1306 accompanying only 2
 * exernal capacitors can generate a 7.5V voltage supply (VCC) from a low
 * voltage supply input (VBAT). The VCC is the voltage supply to the OLED
 * driver block.
 *
 * @param[in] state \n
 * - SSD1306_CHARGE_BUMP_DISABLE: disable charge pump \n
 * - SSD1306_CHARGE_BUMP_ENABLE: enable charge pump during display on
 */
void SSD1306_SetChargeBumpSetting(SSD1306_ChargeBumpState_t state);

/**
 * @brief this function initializes SSD1306
 *
 * @note this function should be called first before using any SSD1306's
 * functions. SSD1306 is intialized by execute following commands
 * 1. RESET (active LOW)
 * 2. Set multiplex ratio (0xA8)
 * 3. Set display offset (0xD3)
 * 4. Set display start line (0x40)
 * 5. Set segment re-map (0xA0/0xA1)
 * 6. Set COM Output Direction (0xC0/0xC8)
 * 7. Set COM pins hardware configuration (0xDA)
 * 8. Set contrast control (0x81)
 * 9. Disable entire display on (0xA4)
 * 10. Set normal display (0xA6)
 * 11. Set osc frequency (0xD5)
 * 12. Enable charge pump regulator (0x8D)
 * 12. Display on (0xAF)
 */
void SSD1306_Init(void);

/**
 * @brief this function resets the OLED
 *
 * @note when RES# input is LOW, the chip is initialized with following
 * status \n
 * 1. Display is OFF
 * 2. 126 x 64 Display Mode
 * 3. Normal segment and display data column address and row address mapping
 * (SEG0 mapped to address 00h and COM0 mapped to address 00h
 * 4. Shift register data clear in serial interface
 * 5. Display start line is set at display RAM address 0
 * 6. Column address counter is set at 0
 * 7. Normal scan direction of the COM outputs
 * 8. Constrast control register is set at 7Fh
 * 9. Normal display mode
 */
void SSD1306_Reset(void);

/**
 * @brief this function updates the changes to the display
 */
void SSD1306_UpdateDisp(void);

/**
 * @brief this function fills the SSD1306_Buffer with 0x00 or 0xFF
 */
void SSD1306_FillBuffer(SSD1306_PixelState_t state);

/**
 * @brief this function draws a pixel at (x,y) location.
 *
 * @note (0,0) locate at the bottom left of the OLED and (127,63) locate at
 * the top right of the OLED.
 *
 * @param[in] x should be in range [0-127]
 * @param[in] y should be in range [0-63]
 * @param[in] state on/off
 */
void SSD1306_DrawPixel(uint8_t x, uint8_t y, SSD1306_PixelState_t state);

/**
 * @brief this function draws a line between 2 dots (x1, y1) and (x2, y2)
 *
 * @note (0,0) locate at the bottom left of the OLED and (127,63) locate at
 * the top right of the OLED.
 *
 * @param[in] x1 should be in range [0-127]
 * @param[in] y1 should be in range [0-63]
 * @param[in] x2 should be in range [0-127]
 * @param[in] y2 should be in range [0-63]
 */
void SSD1306_DrawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);

/**
 * @brief this function draws a column with a given height
 * 
 * @param[in] col which column to draw
 * @param[in] height height of the column
 */
void SSD1306_DrawCol(uint8_t col, uint8_t height);

/**
 * @brief this function visualize sound
 * 
 * @param[in] p_data pointer to data 
 * @param[in] size equals width of the display
 * @param[in] max max of the data input, it is used to scale the data 
 * to apporiate height on the display
 */
void SSD1306_SoundVisualize(uint8_t* p_data, uint8_t size, uint8_t max);


/**
 * @brief this function draws a char at (x,y) location.
 *
 * @note this function will draw the character from (x,y) up and to the
 * right of (x,y)
 * @param[in] x should be in range [0-127]
 * @param[in] y should be in range [0-63]
 * @param[in] c
 */
void SSD1306_PrintCharXY(uint8_t x, uint8_t y, uint8_t c);

/**
 * @brief this function draws a string start at (x,y) location (up and
 * right)
 *
 * @param[in] x should be in range [0-127]
 * @param[in] y should be in range [0-63]
 * @param[in] *str pointer to the string
 * @param[in] length length/size of the string
 */
void SSD1306_PrintStrXY(uint8_t x, uint8_t y, uint8_t *str, uint8_t length);

/**
 * @brief this function draws a character at (col,row).
 *
 * @note for 128x64 display with 6x8 font, there are 21 cols and 8 rows
 *
 * @param[in] col should be in range [0-20]
 * @param[in] row should be in range [0-7]
 */
void SSD1306_PrintCharColRow(uint8_t col, uint8_t row, uint8_t c);

/**
 * @brief this function draws a string at (col,row)
 *
 * @note for 128x64 display with 6x8 font, there are 21 cols and 8 rows
 *
 * @param[in] col should be in range [0-20]
 * @param[in] row should be in range [0-7]
 * @param[in] *str pointer to the string
 * @param[in] length length/size of the string
 */
void SSD1306_PrintStrColRow(uint8_t col, uint8_t row, uint8_t *str,
                            uint8_t length);

/**
 * @brief this function draws a table (15 cols x 4 rows) on the OLED display
 */
void SSD1306_Board_Initialize(void);

/**
 * @brief this function draws a character to the initilized table
 * (SSD1306_Board_Initialize()) at (board_col, board_row)
 *
 * @param[in] board_col should be in range [0-14]
 * @param[in] board_row should be in range [0-3]
 */
void SSD1306_Board_PrintChar(uint8_t board_col, uint8_t board_row, char c);

/**
 * @brief this functions draw a title (string) of the table/board on the top
 * of the OLED
 *
 * @param[in] x_offset offset on x-axis
 * @param[in] *title pointer to title
 * @param[in] length length/size of title
 */
void SSD1306_Board_PrintTitle(uint8_t x_offset, uint8_t *title,
                              uint8_t length);

#ifdef __cplusplus
}
#endif /* End of CPP guard */

#endif /* _BSP_SSD1306_H_ */
