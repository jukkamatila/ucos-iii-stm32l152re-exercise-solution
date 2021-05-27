/*!
 * @file bsp_ssd1306.c
 */

#include "bsp_ssd1306.h"

void SSD1306_SetConstrast(uint8_t constrast)
{
    SSD1306_WriteCommand(0x81);
    SSD1306_WriteCommand(constrast);
}

void SSD1306_EntireDispOnSoft(void) { SSD1306_WriteCommand(0xA4); }

void SSD1306_EntireDispOnHard(void) { SSD1306_WriteCommand(0xA5); }

void SSD1306_InverseDisp(SSD1306_Inverse_t state)
{
    SSD1306_WriteCommand(0xA6 | state);
}

void SSD1306_DispOnOff(SSD1306_DispState_t state)
{
    SSD1306_WriteCommand(0xAE | state);
    if (state == 0x01)      /* ON */
        SSD1306_Delay(100); /* SEG/COM will be ON after 100ms */
}

void SSD1306_SetupContHorizontalScroll(
    SSD1306_HorizontalScrollDirection_t direct, SSD1306_PageAddr_t start_page,
    SSD1306_ScrollFrame_t frame, SSD1306_PageAddr_t end_page)
{
    SSD1306_WriteCommand(0x26 | direct);
    SSD1306_WriteCommand(SSD1306_DUMMY_BYTE_OFF);
    SSD1306_WriteCommand(start_page);
    SSD1306_WriteCommand(frame);
    SSD1306_WriteCommand(end_page);
    SSD1306_WriteCommand(SSD1306_DUMMY_BYTE_OFF);
    SSD1306_WriteCommand(SSD1306_DUMMY_BYTE_ON);
}

void SSD1306_SetupContHorizontalVerticalScroll(
    SSD1306_HorizontalScrollDirection_t direct, SSD1306_PageAddr_t start_page,
    SSD1306_ScrollFrame_t frame, SSD1306_PageAddr_t end_page,
    SSD1306_Row_t offset)
{
    SSD1306_WriteCommand(0x29 | direct);
    SSD1306_WriteCommand(SSD1306_DUMMY_BYTE_OFF);
    SSD1306_WriteCommand(start_page);
    SSD1306_WriteCommand(frame);
    SSD1306_WriteCommand(end_page);
    SSD1306_WriteCommand(offset);
}

void SSD1306_DeactiveScroll(void) { SSD1306_WriteCommand(0x2E); }

void SSD1306_ActiveScroll(void) { SSD1306_WriteCommand(0x2F); }

void SSD1306_SetVerticalScrollArea(SSD1306_Row_t n_fixed_row,
                                   SSD1306_Row_t n_scroll_row)
{
    if (((n_fixed_row + n_scroll_row) <= SSD1306_MUX_RATIO) &&
        (n_scroll_row <= SSD1306_MUX_RATIO))
    {
        SSD1306_WriteCommand(0xA3);
        SSD1306_WriteCommand(n_fixed_row);
        SSD1306_WriteCommand(n_scroll_row);
    }
}

void SSD1306_SetMemAddrMode(SSD1306_MemAddrMode_t mode)
{
    SSD1306_WriteCommand(0x20);
    SSD1306_WriteCommand(mode);
}

void SSD1306_SetColAddr(SSD1306_Col_t col_start, SSD1306_Col_t col_end)
{
    SSD1306_WriteCommand(0x21);
    SSD1306_WriteCommand(col_start);
    SSD1306_WriteCommand(col_end);
}

void SSD1306_SetPageAddr(SSD1306_PageAddr_t start_page,
                         SSD1306_PageAddr_t end_page)
{
    SSD1306_WriteCommand(0x22);
    SSD1306_WriteCommand(start_page);
    SSD1306_WriteCommand(end_page);
}

void SSD1306_SetPageStartAddr(SSD1306_PageAddr_t start_page, SSD1306_Col_t seg)
{
    SSD1306_WriteCommand(0xB0 | start_page);
    SSD1306_WriteCommand(0x00 | (seg & 0x0F));
    SSD1306_WriteCommand(0x10 | ((seg & 0xF0) >> 4));
}

void SSD1306_SetDispStartLine(SSD1306_Row_t start_line)
{
    SSD1306_WriteCommand(0x40 | start_line);
}

void SSD1306_SetSegRemap(SSD1306_SegmentRemapDirection_t direction)
{
    SSD1306_WriteCommand(0xA0 | direction);
}

void SSD1306_SetMultiplexRatio(SSD1306_MUXRatio_t ratio)
{
    SSD1306_WriteCommand(0xA8);
    SSD1306_WriteCommand(ratio);
}

void SSD1306_SetCOMOutPutScanDirection(SSD1306_COMOutputScanDirection_t direction)
{
    SSD1306_WriteCommand(0xC0 | direction);
}

void SSD1306_SetDispOffet(SSD1306_Row_t offset)
{
    SSD1306_WriteCommand(0xD3);
    SSD1306_WriteCommand(offset);
}

void SSD1306_SetHardwareCfg(SSD1306_PinConfig_t cfg, SSD1306_COMRemap_t remap)
{
    SSD1306_WriteCommand(0xDA);
    SSD1306_WriteCommand(cfg | remap);
}

void SSD1306_ClkCfg(SSD1306_ClkRatio_t ratio, SSD1306_ClkFrq_t frq)
{
    SSD1306_WriteCommand(0xD5);
    SSD1306_WriteCommand(ratio | (frq << 4));
}

void SSD1306_SetPrechargePeriod(SSD1306_Phase_t phase1_period,
                                SSD1306_Phase_t phase2_period)
{
    SSD1306_WriteCommand(0xD9);
    SSD1306_WriteCommand(phase1_period | (phase2_period << 4));
}

void SSD1306_SetVCOMHDeselectLevel(SSD1306_VCOMHDeselectLevel_t level)
{
    SSD1306_WriteCommand(0xDB);
    SSD1306_WriteCommand(level);
}

void SSD1306_SetChargeBumpSetting(SSD1306_ChargeBumpState_t state)
{
    SSD1306_WriteCommand(0x8D);
    SSD1306_WriteCommand(state);
}

void SSD1306_Reset(void)
{
    SSD1306_ResetPinOff();
    SSD1306_Delay(5); /* at least 3us */
    SSD1306_ResetPinOn();
}

void SSD1306_UpdateDisp(void)
{
    for (uint8_t i = 0; i < SSD1306_HEIGHT / SSD1306_SEG_SIZE; i++)
    {
        SSD1306_WriteCommand(0xB0 + i);
        SSD1306_WriteCommand(0x00);
        SSD1306_WriteCommand(0x10);
        for (uint8_t j = 0; j < SSD1306_WIDTH; j++)
        {
            SSD1306_WriteData(&SSD1306_Buffer[i][j], 1);
        }
    }
}

void SSD1306_FillBuffer(SSD1306_PixelState_t state)
{
    uint8_t byte = 0x00;
    if (state)
        byte = 0xFF;
    for (int8_t i = 0; i < SSD1306_HEIGHT / SSD1306_SEG_SIZE; i++)
    {
        for (int8_t j = 0; j < SSD1306_WIDTH; j++)
        {
            SSD1306_Buffer[i][j] = byte;
        }
    }
}

void SSD1306_DrawPixel(uint8_t x, uint8_t y, SSD1306_PixelState_t state)
{
    if (state)
        SSD1306_Buffer[y / SSD1306_SEG_SIZE][x] |= 1u << (y % SSD1306_SEG_SIZE);
    else
        SSD1306_Buffer[y / SSD1306_SEG_SIZE][x] &=
            ~(1u << (y % SSD1306_SEG_SIZE));
}

void SSD1306_DrawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2)
{
    int dx = x2 - x1;
    int dy = y2 - y1;

    int p = 2*dy-dx;

    while (x1 < x2 || y1 < y2)
    {
        if (p < 0)
        {
            x1++;
            p = p + 2*dy;
        }
        else
        {
            if(x1 < x2) x1++;
            y1++;
            p = p+2*dy-2*dx;
        }
        SSD1306_DrawPixel(x1,y1, SSD1306_PIXEL_ON);
    }
    SSD1306_UpdateDisp();
}

void SSD1306_DrawCol(uint8_t col, uint8_t height)
{
    for(int i = 0; i < SSD1306_HEIGHT; i ++)
    {
        if (i <= height)
        {
            SSD1306_DrawPixel(col,i, SSD1306_PIXEL_ON);
        }
        else
        {
            SSD1306_DrawPixel(col,i, SSD1306_PIXEL_OFF);
        }
    }
}

void SSD1306_SoundVisualize(uint8_t* p_data, uint8_t size, uint8_t max)
{
    for(int i = 0; i < size; i ++)
    {
        for(int j = 0; j < SSD1306_BOARD_HEIGHT; j++)
        {
            if(j < *(p_data+i)/(max/SSD1306_BOARD_HEIGHT))
            {
                SSD1306_DrawPixel(i,j, SSD1306_PIXEL_ON);
            }
            else
            {
                SSD1306_DrawPixel(i,j, SSD1306_PIXEL_OFF);

            }
        }
    }
    SSD1306_UpdateDisp();
}


void SSD1306_PrintCharXY(uint8_t x, uint8_t y, uint8_t c)
{
    if (x < SSD1306_WIDTH && y < SSD1306_HEIGHT)
    {
        for (uint8_t j = 0; j < SSD1306_FONT_WIDTH; j++)
        {
            for (uint8_t i = 0; i < SSD1306_FONT_HEIGHT; i++)
            {
                SSD1306_DrawPixel(x + j, y + i,
                                  Font[(c - 32) * SSD1306_FONT_WIDTH + j] &
                                      (1 << i));
            }
        }
    }
    SSD1306_UpdateDisp();
}

void SSD1306_PrintStrXY(uint8_t x, uint8_t y, uint8_t *str, uint8_t length)
{
    for (uint8_t i = 0; i < length - 1; i++)
    {
        SSD1306_PrintCharXY(x + (i * SSD1306_FONT_WIDTH), y, *(str + i));
    }
}

void SSD1306_PrintCharColRow(uint8_t col, uint8_t row, uint8_t c)
{
    SSD1306_PrintCharXY(col * SSD1306_FONT_WIDTH, row * SSD1306_FONT_HEIGHT, c);
}

void SSD1306_PrintStrColRow(uint8_t col, uint8_t row, uint8_t *str,
                            uint8_t length)
{
    for (uint8_t i = 0; i < length - 1; i++)
    {
        SSD1306_PrintCharColRow(col + i, row, *(str + i));
    }
}

void SSD1306_Board_Initialize(void)
{
    for (uint8_t y = 0; y <= SSD1306_BOARD_HEIGHT; y++)
    {
        for (uint8_t x = 0; x <= SSD1306_BOARD_WIDTH; x++)
        {
            if (y == 0 || y % 10 == 0)
            {
                SSD1306_DrawPixel(x + SSD1306_BOARD_X_OFFSET,
                                  y + SSD1306_BOARD_Y_OFFSET, SSD1306_PIXEL_ON);
            }
            if (x == 0 || x % 8 == 0)
            {
                SSD1306_DrawPixel(x + SSD1306_BOARD_X_OFFSET,
                                  y + SSD1306_BOARD_Y_OFFSET, SSD1306_PIXEL_ON);
            }
        }
    }
    SSD1306_UpdateDisp();
}

void SSD1306_Board_PrintChar(uint8_t board_col, uint8_t board_row, char c)
{
    if (board_col <= SSD1306_BOARD_WIDTH / SSD1306_BOARD_CELL_WIDTH &&
        board_row <= SSD1306_BOARD_HEIGHT / SSD1306_BOARD_CELL_HEIGHT)
    {
        for (uint8_t j = 0; j < SSD1306_FONT_WIDTH; j++)
        {
            for (uint8_t i = 0; i < SSD1306_FONT_HEIGHT; i++)
            {
                SSD1306_DrawPixel(
                    board_col * SSD1306_BOARD_CELL_WIDTH + SSD1306_BOARD_X0 +
                        SSD1306_BOARD_X_OFFSET + j,
                    board_row * SSD1306_BOARD_CELL_HEIGHT + SSd1306_BOARD_Y0 +
                        SSD1306_BOARD_Y_OFFSET + i,
                    Font[(c - 32) * SSD1306_FONT_WIDTH + j] & (1 << i));
            }
        }
    }
    SSD1306_UpdateDisp();
}

void SSD1306_Board_PrintTitle(uint8_t x_offset, uint8_t *title, uint8_t length)
{
    SSD1306_PrintStrXY(x_offset, SSD1306_HEIGHT - SSD1306_TITLE_HEIGHT, title,
                       length);
}
