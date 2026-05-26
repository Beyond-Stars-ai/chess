#ifndef __GAME_CORE_H
#define __GAME_CORE_H

#include "game_types.h"

/* 游戏模式枚举 */
typedef enum {
    GAME_MODE_NONE = 0,
    GAME_MODE_PLAY,    // 实物下棋模式
    GAME_MODE_TEXT     // 调试模式（不下实物）
} GameMode_t;

/* 游戏核心API */
void GameCore_Init(GameCtrl_t *game, GameMode_t mode);
int GameCore_PlayerMove(GameCtrl_t *game, uint8_t position);
int  GameCore_AIMove(GameCtrl_t *game, uint8_t ai_color);
void GameCore_Reset(GameCtrl_t *game);

/* 获取当前游戏模式 */
GameMode_t GameCore_GetMode(void);
void GameCore_SetMode(GameMode_t mode);

#endif /* __GAME_CORE_H */