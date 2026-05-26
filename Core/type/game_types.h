#ifndef __GAME_TYPES_H
#define __GAME_TYPES_H

#include <stdint.h>

/* 游戏状态枚举 */
typedef enum
{
    STATE_MAIN_MENU,        // 主菜单
    STATE_SELECT_FIRST,     // 选先后手
    STATE_AI_THINK,         // AI思考
    STATE_PLAYER_MOVE,      // 玩家移动光标/落子
    STATE_GAME_OVER,        // 游戏结束
} AppState_t;

/* 游戏控制块 */
typedef struct
{
    uint8_t board[9];           // 棋盘
    uint8_t cursor_pos;         // 光标位置 0-8
    uint8_t ai_color;           // AI颜色
    uint8_t player_color;       // 玩家颜色
    uint8_t current_turn;       // 0=玩家, 1=AI
    int game_result;            // GAME_ONGOING/BLACK_WIN/WHITE_WIN/DRAW
} GameCtrl_t;

#endif /* __GAME_TYPES_H */