#include "stm32f4xx_hal.h"
#include "OLED.h"
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

// 显存数组
uint8_t OLED_DisplayBuf[8][128];

// I2C 句柄（需外部定义，实际在 main.c 或这里定义）
extern I2C_HandleTypeDef hi2c1;
#define OLED_ADDR 0x78   // 7位地址0x3C左移一位

// 硬件I2C写命令
static void OLED_WriteCommand(uint8_t cmd) {
    uint8_t buf[2] = {0x00, cmd};
    HAL_I2C_Master_Transmit(&hi2c1, OLED_ADDR, buf, 2, 100);
}

// 硬件I2C写数据（批量）
static void OLED_WriteData(uint8_t *data, uint8_t count) {
    uint8_t *tx_buf = (uint8_t*)malloc(count + 1);
    if(tx_buf == NULL) return;
    tx_buf[0] = 0x40;   // 控制字节：数据模式
    memcpy(tx_buf + 1, data, count);
    HAL_I2C_Master_Transmit(&hi2c1, OLED_ADDR, tx_buf, count + 1, 100);
    free(tx_buf);
}

// 设置光标位置
static void OLED_SetCursor(uint8_t Page, uint8_t X) {
    // 若使用1.3寸OLED，需 X += 2;
    OLED_WriteCommand(0xB0 | Page);
    OLED_WriteCommand(0x10 | ((X & 0xF0) >> 4));
    OLED_WriteCommand(0x00 | (X & 0x0F));
}

// 初始化OLED
void OLED_Init(void) {
    // 注意：I2C1 和 GPIO 应该在 main.c 中提前初始化（MX_I2C1_Init）
    HAL_Delay(500);  // 等待OLED上电稳定

    OLED_WriteCommand(0xAE); // 关闭显示
    OLED_WriteCommand(0xD5); OLED_WriteCommand(0x80);
    OLED_WriteCommand(0xA8); OLED_WriteCommand(0x3F);
    OLED_WriteCommand(0xD3); OLED_WriteCommand(0x00);
    OLED_WriteCommand(0x40);
    OLED_WriteCommand(0xA1);
    OLED_WriteCommand(0xC8);
    OLED_WriteCommand(0xDA); OLED_WriteCommand(0x12);
    OLED_WriteCommand(0x81); OLED_WriteCommand(0xCF);
    OLED_WriteCommand(0xD9); OLED_WriteCommand(0xF1);
    OLED_WriteCommand(0xDB); OLED_WriteCommand(0x30);
    OLED_WriteCommand(0xA4);
    OLED_WriteCommand(0xA6);
    OLED_WriteCommand(0x8D); OLED_WriteCommand(0x14);
    OLED_WriteCommand(0xAF); // 开启显示

    OLED_Clear();
    OLED_Update();
}

// 更新整个屏幕
void OLED_Update(void) {
    for(uint8_t j = 0; j < 8; j++) {
        OLED_SetCursor(j, 0);
        OLED_WriteData(OLED_DisplayBuf[j], 128);
    }
}

// 部分更新
void OLED_UpdateArea(uint8_t X, uint8_t Y, uint8_t Width, uint8_t Height) {
    if(X > 127 || Y > 63) return;
    if(X + Width > 128) Width = 128 - X;
    if(Y + Height > 64) Height = 64 - Y;
    for(uint8_t j = Y / 8; j < (Y + Height - 1)/8 + 1; j++) {
        OLED_SetCursor(j, X);
        OLED_WriteData(&OLED_DisplayBuf[j][X], Width);
    }
}

// 清屏
void OLED_Clear(void) {
    for(uint8_t j = 0; j < 8; j++)
        for(uint8_t i = 0; i < 128; i++)
            OLED_DisplayBuf[j][i] = 0x00;
}

// 清除区域
void OLED_ClearArea(uint8_t X, uint8_t Y, uint8_t Width, uint8_t Height) {
    if(X > 127 || Y > 63) return;
    if(X + Width > 128) Width = 128 - X;
    if(Y + Height > 64) Height = 64 - Y;
    for(uint8_t j = Y; j < Y + Height; j++)
        for(uint8_t i = X; i < X + Width; i++)
            OLED_DisplayBuf[j / 8][i] &= ~(0x01 << (j % 8));
}

// 全局取反
void OLED_Reverse(void) {
    for(uint8_t j = 0; j < 8; j++)
        for(uint8_t i = 0; i < 128; i++)
            OLED_DisplayBuf[j][i] ^= 0xFF;
}

// 区域取反
void OLED_ReverseArea(uint8_t X, uint8_t Y, uint8_t Width, uint8_t Height) {
    if(X > 127 || Y > 63) return;
    if(X + Width > 128) Width = 128 - X;
    if(Y + Height > 64) Height = 64 - Y;
    for(uint8_t j = Y; j < Y + Height; j++)
        for(uint8_t i = X; i < X + Width; i++)
            OLED_DisplayBuf[j / 8][i] ^= 0x01 << (j % 8);
}

// 显示字符
void OLED_ShowChar(uint8_t X, uint8_t Y, char Char, uint8_t FontSize) {
    if(FontSize == OLED_8X16)
        OLED_ShowImage(X, Y, 8, 16, OLED_F8x16[Char - ' ']);
    else if(FontSize == OLED_6X8)
        OLED_ShowImage(X, Y, 6, 8, OLED_F6x8[Char - ' ']);
}

// 显示字符串
void OLED_ShowString(uint8_t X, uint8_t Y, char *String, uint8_t FontSize) {
    for(uint8_t i = 0; String[i] != '\0'; i++)
        OLED_ShowChar(X + i * FontSize, Y, String[i], FontSize);
}

// 次方函数
static uint32_t OLED_Pow(uint32_t X, uint32_t Y) {
    uint32_t Result = 1;
    while(Y--) Result *= X;
    return Result;
}

// 显示数字
void OLED_ShowNum(uint8_t X, uint8_t Y, uint32_t Number, uint8_t Length, uint8_t FontSize) {
    for(uint8_t i = 0; i < Length; i++)
        OLED_ShowChar(X + i * FontSize, Y, Number / OLED_Pow(10, Length - i - 1) % 10 + '0', FontSize);
}

void OLED_ShowSignedNum(uint8_t X, uint8_t Y, int32_t Number, uint8_t Length, uint8_t FontSize) {
    uint32_t Number1;
    if(Number >= 0) {
        OLED_ShowChar(X, Y, '+', FontSize);
        Number1 = Number;
    } else {
        OLED_ShowChar(X, Y, '-', FontSize);
        Number1 = -Number;
    }
    for(uint8_t i = 0; i < Length; i++)
        OLED_ShowChar(X + (i+1)*FontSize, Y, Number1 / OLED_Pow(10, Length - i - 1) % 10 + '0', FontSize);
}

void OLED_ShowHexNum(uint8_t X, uint8_t Y, uint32_t Number, uint8_t Length, uint8_t FontSize) {
    uint8_t SingleNumber;
    for(uint8_t i = 0; i < Length; i++) {
        SingleNumber = Number / OLED_Pow(16, Length - i - 1) % 16;
        if(SingleNumber < 10)
            OLED_ShowChar(X + i * FontSize, Y, SingleNumber + '0', FontSize);
        else
            OLED_ShowChar(X + i * FontSize, Y, SingleNumber - 10 + 'A', FontSize);
    }
}

void OLED_ShowBinNum(uint8_t X, uint8_t Y, uint32_t Number, uint8_t Length, uint8_t FontSize) {
    for(uint8_t i = 0; i < Length; i++)
        OLED_ShowChar(X + i * FontSize, Y, Number / OLED_Pow(2, Length - i - 1) % 2 + '0', FontSize);
}

void OLED_ShowFloatNum(uint8_t X, uint8_t Y, double Number, uint8_t IntLength, uint8_t FraLength, uint8_t FontSize) {
    uint32_t Temp;
    if(Number >= 0)
        OLED_ShowChar(X, Y, '+', FontSize);
    else {
        OLED_ShowChar(X, Y, '-', FontSize);
        Number = -Number;
    }
    OLED_ShowNum(X + FontSize, Y, (uint32_t)Number, IntLength, FontSize);
    OLED_ShowChar(X + (IntLength + 1) * FontSize, Y, '.', FontSize);
    Number -= (uint32_t)Number;
    Temp = OLED_Pow(10, FraLength);
    OLED_ShowNum(X + (IntLength + 2) * FontSize, Y, ((uint32_t)(Number * Temp)) % Temp, FraLength, FontSize);
}

void OLED_ShowChinese(uint8_t X, uint8_t Y, char *Chinese) {
    uint8_t pChinese = 0, pIndex;
    char SingleChinese[OLED_CHN_CHAR_WIDTH + 1] = {0};
    for(uint8_t i = 0; Chinese[i] != '\0'; i++) {
        SingleChinese[pChinese++] = Chinese[i];
        if(pChinese >= OLED_CHN_CHAR_WIDTH) {
            pChinese = 0;
            for(pIndex = 0; strcmp(OLED_CF16x16[pIndex].Index, "") != 0; pIndex++) {
                if(strcmp(OLED_CF16x16[pIndex].Index, SingleChinese) == 0)
                    break;
            }
            OLED_ShowImage(X + ((i+1)/OLED_CHN_CHAR_WIDTH - 1)*16, Y, 16, 16, OLED_CF16x16[pIndex].Data);
        }
    }
}

// 显示图像（核心绘图函数）
void OLED_ShowImage(uint8_t X, uint8_t Y, uint8_t Width, uint8_t Height, const uint8_t *Image) {
    if(X > 127 || Y > 63) return;
    if(X + Width > 128) Width = 128 - X;
    if(Y + Height > 64) Height = 64 - Y;
    OLED_ClearArea(X, Y, Width, Height);
    for(uint8_t j = 0; j < (Height - 1)/8 + 1; j++) {
        for(uint8_t i = 0; i < Width; i++) {
            if(X + i > 127) break;
            if(Y/8 + j > 7) return;
            OLED_DisplayBuf[Y/8 + j][X + i] |= Image[j * Width + i] << (Y % 8);
            if(Y/8 + j + 1 > 7) continue;
            OLED_DisplayBuf[Y/8 + j + 1][X + i] |= Image[j * Width + i] >> (8 - Y % 8);
        }
    }
}

void OLED_Printf(uint8_t X, uint8_t Y, uint8_t FontSize, char *format, ...) {
    char String[30];
    va_list arg;
    va_start(arg, format);
    vsprintf(String, format, arg);
    va_end(arg);
    OLED_ShowString(X, Y, String, FontSize);
}

void OLED_DrawPoint(uint8_t X, uint8_t Y) {
    if(X > 127 || Y > 63) return;
    OLED_DisplayBuf[Y / 8][X] |= 0x01 << (Y % 8);
}

uint8_t OLED_GetPoint(uint8_t X, uint8_t Y) {
    if(X > 127 || Y > 63) return 0;
    return (OLED_DisplayBuf[Y / 8][X] & (0x01 << (Y % 8))) ? 1 : 0;
}

void OLED_DrawLine(uint8_t X0, uint8_t Y0, uint8_t X1, uint8_t Y1) {
    int16_t x, y, dx, dy, d, incrE, incrNE, temp;
    int16_t x0 = X0, y0 = Y0, x1 = X1, y1 = Y1;
    uint8_t yflag = 0, xyflag = 0;
    if(y0 == y1) {
        if(x0 > x1) { temp = x0; x0 = x1; x1 = temp; }
        for(x = x0; x <= x1; x++) OLED_DrawPoint(x, y0);
    } else if(x0 == x1) {
        if(y0 > y1) { temp = y0; y0 = y1; y1 = temp; }
        for(y = y0; y <= y1; y++) OLED_DrawPoint(x0, y);
    } else {
        if(x0 > x1) { temp = x0; x0 = x1; x1 = temp; temp = y0; y0 = y1; y1 = temp; }
        if(y0 > y1) { y0 = -y0; y1 = -y1; yflag = 1; }
        if(y1 - y0 > x1 - x0) { temp = x0; x0 = y0; y0 = temp; temp = x1; x1 = y1; y1 = temp; xyflag = 1; }
        dx = x1 - x0;
        dy = y1 - y0;
        incrE = 2 * dy;
        incrNE = 2 * (dy - dx);
        d = 2 * dy - dx;
        x = x0; y = y0;
        if(yflag && xyflag) OLED_DrawPoint(y, -x);
        else if(yflag)      OLED_DrawPoint(x, -y);
        else if(xyflag)     OLED_DrawPoint(y, x);
        else                OLED_DrawPoint(x, y);
        while(x < x1) {
            x++;
            if(d < 0) d += incrE;
            else { y++; d += incrNE; }
            if(yflag && xyflag) OLED_DrawPoint(y, -x);
            else if(yflag)      OLED_DrawPoint(x, -y);
            else if(xyflag)     OLED_DrawPoint(y, x);
            else                OLED_DrawPoint(x, y);
        }
    }
}

void OLED_DrawRectangle(uint8_t X, uint8_t Y, uint8_t Width, uint8_t Height, uint8_t IsFilled) {
    if(IsFilled) {
        for(uint8_t i = X; i < X + Width; i++)
            for(uint8_t j = Y; j < Y + Height; j++)
                OLED_DrawPoint(i, j);
    } else {
        for(uint8_t i = X; i < X + Width; i++) {
            OLED_DrawPoint(i, Y);
            OLED_DrawPoint(i, Y + Height - 1);
        }
        for(uint8_t i = Y; i < Y + Height; i++) {
            OLED_DrawPoint(X, i);
            OLED_DrawPoint(X + Width - 1, i);
        }
    }
}

static uint8_t OLED_pnpoly(uint8_t nvert, int16_t *vertx, int16_t *verty, int16_t testx, int16_t testy) {
    int16_t i, j, c = 0;
    for(i = 0, j = nvert - 1; i < nvert; j = i++) {
        if(((verty[i] > testy) != (verty[j] > testy)) &&
           (testx < (vertx[j] - vertx[i]) * (testy - verty[i]) / (verty[j] - verty[i]) + vertx[i]))
            c = !c;
    }
    return c;
}

void OLED_DrawTriangle(uint8_t X0, uint8_t Y0, uint8_t X1, uint8_t Y1, uint8_t X2, uint8_t Y2, uint8_t IsFilled) {
    if(!IsFilled) {
        OLED_DrawLine(X0, Y0, X1, Y1);
        OLED_DrawLine(X0, Y0, X2, Y2);
        OLED_DrawLine(X1, Y1, X2, Y2);
    } else {
        int16_t vx[] = {X0, X1, X2};
        int16_t vy[] = {Y0, Y1, Y2};
        uint8_t minx = X0, miny = Y0, maxx = X0, maxy = Y0;
        if(X1 < minx) minx = X1; if(X2 < minx) minx = X2;
        if(Y1 < miny) miny = Y1; if(Y2 < miny) miny = Y2;
        if(X1 > maxx) maxx = X1; if(X2 > maxx) maxx = X2;
        if(Y1 > maxy) maxy = Y1; if(Y2 > maxy) maxy = Y2;
        for(uint8_t i = minx; i <= maxx; i++)
            for(uint8_t j = miny; j <= maxy; j++)
                if(OLED_pnpoly(3, vx, vy, i, j))
                    OLED_DrawPoint(i, j);
    }
}

void OLED_DrawCircle(uint8_t X, uint8_t Y, uint8_t Radius, uint8_t IsFilled) {
    int16_t x = 0, y = Radius, d = 1 - Radius;
    OLED_DrawPoint(X + x, Y + y);
    OLED_DrawPoint(X - x, Y - y);
    OLED_DrawPoint(X + y, Y + x);
    OLED_DrawPoint(X - y, Y - x);
    if(IsFilled)
        for(int16_t j = -y; j < y; j++)
            OLED_DrawPoint(X, Y + j);
    while(x < y) {
        x++;
        if(d < 0) d += 2*x + 1;
        else { y--; d += 2*(x - y) + 1; }
        OLED_DrawPoint(X + x, Y + y);
        OLED_DrawPoint(X + y, Y + x);
        OLED_DrawPoint(X - x, Y - y);
        OLED_DrawPoint(X - y, Y - x);
        OLED_DrawPoint(X + x, Y - y);
        OLED_DrawPoint(X + y, Y - x);
        OLED_DrawPoint(X - x, Y + y);
        OLED_DrawPoint(X - y, Y + x);
        if(IsFilled) {
            for(int16_t j = -y; j < y; j++) { OLED_DrawPoint(X + x, Y + j); OLED_DrawPoint(X - x, Y + j); }
            for(int16_t j = -x; j < x; j++) { OLED_DrawPoint(X - y, Y + j); OLED_DrawPoint(X + y, Y + j); }
        }
    }
}

void OLED_DrawEllipse(uint8_t X, uint8_t Y, uint8_t A, uint8_t B, uint8_t IsFilled) {
    int16_t x = 0, y = B;
    int16_t a = A, b = B;
    float d1 = b*b + a*a*(-b + 0.5);
    if(IsFilled)
        for(int16_t j = -y; j < y; j++)
            OLED_DrawPoint(X, Y + j);
    OLED_DrawPoint(X + x, Y + y);
    OLED_DrawPoint(X - x, Y - y);
    OLED_DrawPoint(X - x, Y + y);
    OLED_DrawPoint(X + x, Y - y);
    while(b*b*(x+1) < a*a*(y-0.5)) {
        if(d1 <= 0) d1 += b*b*(2*x+3);
        else { d1 += b*b*(2*x+3) + a*a*(-2*y+2); y--; }
        x++;
        if(IsFilled)
            for(int16_t j = -y; j < y; j++) { OLED_DrawPoint(X + x, Y + j); OLED_DrawPoint(X - x, Y + j); }
        OLED_DrawPoint(X + x, Y + y);
        OLED_DrawPoint(X - x, Y - y);
        OLED_DrawPoint(X - x, Y + y);
        OLED_DrawPoint(X + x, Y - y);
    }
    float d2 = b*b*(x+0.5)*(x+0.5) + a*a*(y-1)*(y-1) - a*a*b*b;
    while(y > 0) {
        if(d2 <= 0) { d2 += b*b*(2*x+2) + a*a*(-2*y+3); x++; }
        else d2 += a*a*(-2*y+3);
        y--;
        if(IsFilled)
            for(int16_t j = -y; j < y; j++) { OLED_DrawPoint(X + x, Y + j); OLED_DrawPoint(X - x, Y + j); }
        OLED_DrawPoint(X + x, Y + y);
        OLED_DrawPoint(X - x, Y - y);
        OLED_DrawPoint(X - x, Y + y);
        OLED_DrawPoint(X + x, Y - y);
    }
}

static uint8_t OLED_IsInAngle(int16_t X, int16_t Y, int16_t StartAngle, int16_t EndAngle) {
    int16_t PointAngle = atan2(Y, X) / 3.14 * 180;
    if(StartAngle < EndAngle)
        return (PointAngle >= StartAngle && PointAngle <= EndAngle);
    else
        return (PointAngle >= StartAngle || PointAngle <= EndAngle);
}

void OLED_DrawArc(uint8_t X, uint8_t Y, uint8_t Radius, int16_t StartAngle, int16_t EndAngle, uint8_t IsFilled) {
    int16_t x = 0, y = Radius, d = 1 - Radius;
    if(OLED_IsInAngle(x, y, StartAngle, EndAngle)) OLED_DrawPoint(X + x, Y + y);
    if(OLED_IsInAngle(-x, -y, StartAngle, EndAngle)) OLED_DrawPoint(X - x, Y - y);
    if(OLED_IsInAngle(y, x, StartAngle, EndAngle)) OLED_DrawPoint(X + y, Y + x);
    if(OLED_IsInAngle(-y, -x, StartAngle, EndAngle)) OLED_DrawPoint(X - y, Y - x);
    if(IsFilled)
        for(int16_t j = -y; j < y; j++)
            if(OLED_IsInAngle(0, j, StartAngle, EndAngle)) OLED_DrawPoint(X, Y + j);
    while(x < y) {
        x++;
        if(d < 0) d += 2*x + 1;
        else { y--; d += 2*(x - y) + 1; }
        if(OLED_IsInAngle(x, y, StartAngle, EndAngle)) OLED_DrawPoint(X + x, Y + y);
        if(OLED_IsInAngle(y, x, StartAngle, EndAngle)) OLED_DrawPoint(X + y, Y + x);
        if(OLED_IsInAngle(-x, -y, StartAngle, EndAngle)) OLED_DrawPoint(X - x, Y - y);
        if(OLED_IsInAngle(-y, -x, StartAngle, EndAngle)) OLED_DrawPoint(X - y, Y - x);
        if(OLED_IsInAngle(x, -y, StartAngle, EndAngle)) OLED_DrawPoint(X + x, Y - y);
        if(OLED_IsInAngle(y, -x, StartAngle, EndAngle)) OLED_DrawPoint(X + y, Y - x);
        if(OLED_IsInAngle(-x, y, StartAngle, EndAngle)) OLED_DrawPoint(X - x, Y + y);
        if(OLED_IsInAngle(-y, x, StartAngle, EndAngle)) OLED_DrawPoint(X - y, Y + x);
        if(IsFilled) {
            for(int16_t j = -y; j < y; j++) {
                if(OLED_IsInAngle(x, j, StartAngle, EndAngle)) OLED_DrawPoint(X + x, Y + j);
                if(OLED_IsInAngle(-x, j, StartAngle, EndAngle)) OLED_DrawPoint(X - x, Y + j);
            }
            for(int16_t j = -x; j < x; j++) {
                if(OLED_IsInAngle(-y, j, StartAngle, EndAngle)) OLED_DrawPoint(X - y, Y + j);
                if(OLED_IsInAngle(y, j, StartAngle, EndAngle)) OLED_DrawPoint(X + y, Y + j);
            }
        }
    }
}