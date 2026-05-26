#ifndef __GAME_CORE_H
#define __GAME_CORE_H

#include "game_types.h"

/* 游戏核心API */
int GameCore_PlayerMove(GameCtrl_t *game, uint8_t position);
int  GameCore_AIMove(GameCtrl_t *game, uint8_t ai_color);

/* 获取当前游戏模式 */
void GameCore_SetMode(GameMode_t mode);

#endif /* __GAME_CORE_H */