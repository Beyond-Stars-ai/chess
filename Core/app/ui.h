#ifndef __UI_H
#define __UI_H

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "oled.h"

// UI绘制函数
void DrawGameBoard(void);
void DrawMainMenu(uint8_t main_option);
void DrawSelectFirstMenu(uint8_t select_option);
void DrawGameOver(void);

#endif /* __UI_H */