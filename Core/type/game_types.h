#ifndef __GAME_TYPES_H
#define __GAME_TYPES_H

#include <stdint.h>

// 棋子颜色定义
#define EMPTY   0
#define BLACK   1
#define WHITE   2

// 任务通知标志位定义
#define FLAG_AI_START       (1 << 0)
#define FLAG_AI_DONE        (1 << 1)

#define FLAG_START_SCAN   (1 << 3)  // 通知A
#define FLAG_STOP_SCAN    (1 << 4)  // 通知B

/* 游戏状态枚举 */
typedef enum
{
    STATE_MAIN_MENU,        // 主菜单

    // STATE_PLACE_CHESS,   // 放置棋子
    STATE_COLOR_SELECT,     // 选择颜色
    STATE_PLACE_SELECT,     // 选择位置
    
    //  STATE_PLAY_GAME     //开始游戏
    STATE_SELECT_FIRST,     // 选先后手
    STATE_GAME_OVER,        // 游戏结束
    STATE_AI_THINK,         // AI思考
    STATE_PLAYER_MOVE,      // 玩家移动光标/落子
} AppState_t;

/* UI 选项结构体 */
typedef enum
{
    MAIN_PLACE = 0,
    MAIN_GAME = 1,
    MAIN_TEXT = 2, 
} MainOption_t;

typedef enum
{
    SELECT_AI_FIRST = 0,    // AI先手
    SELECT_PLAYER_FIRST = 1, // 玩家先手
    SELECT_BACK = 2,       // 返回主菜单
} SelectOption_t;

typedef enum {
    CELL_EMPTY = 0, 
    CELL_BLACK = 1,
    CELL_WHITE = 2,
    COLOR_BACK = 3,       // 返回主菜单
} CellState_t;             //由于神奇的原因，CELL_BLACK的值不能为0

typedef struct
{
    // 主菜单选项
    MainOption_t main_option;          // 0=PLACE, 1=GAME, 2=TEXT
    
    // 先后手选项
    SelectOption_t select_option;         // 0=AI_FIRST, 1=PLAYER_FIRST, 2=BACK

    // 颜色选择选项
    CellState_t color_option;         // 0=EMPTY, 1=BLACK, 2=WHITE
} UIOptions_t;


/* 游戏结果枚举 */
typedef enum {
    RESULT_ONGOING   = 0,
    RESULT_BLACK_WIN = 1,
    RESULT_WHITE_WIN = 2,
    RESULT_DRAW      = 3
} GameResult_t;

/* 游戏控制块 */
typedef struct
{
    uint8_t board[9];           // 棋盘
    uint8_t cursor_pos;         // 光标位置 0-8
    uint8_t ai_color;           // AI颜色
    uint8_t player_color;       // 玩家颜色
    uint8_t current_turn;       // 0=玩家, 1=AI
    GameResult_t game_result;            // 游戏结果
} GameCtrl_t;

/* 游戏模式枚举 */
typedef enum {
    GAME_MODE_NONE = 0,
    GAME_MODE_PLAY,    // 实物下棋模式
    GAME_MODE_TEXT     // 调试模式（不下实物）
} GameMode_t;

#endif /* __GAME_TYPES_H */