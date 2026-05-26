#ifndef __CHESS_AI_H
#define __CHESS_AI_H

#include <stdint.h>

//最早的史山
/* ---------- 棋盘格子状态 ---------- */
#define EMPTY 0
#define BLACK 1
#define WHITE 2

/* ---------- 游戏状态返回值 ---------- */
#define GAME_ONGOING   0
#define GAME_BLACK_WIN 1
#define GAME_WHITE_WIN 2
#define GAME_DRAW      3

/* ---------- 外部接口 ---------- */
void BoardInit(uint8_t board[9]);
int  CheckGameResult(const uint8_t board[9]);
int  AI_GetBestMove(const uint8_t board[9], uint8_t ai_color);

#endif /* __CHESS_AI_H */