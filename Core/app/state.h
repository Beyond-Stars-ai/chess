// Core/Inc/state_handlers.h
#ifndef __STATE_HANDLERS_H
#define __STATE_HANDLERS_H

#include "game_types.h"
#include "key.h"

// ==================== 状态处理函数 ====================

// 主菜单状态处理
void HandleMainMenu(KeyEvent_t key);

// 选先后手状态处理
void HandleSelectFirst(KeyEvent_t key);

// 颜色选择状态处理（放置模式）
void HandleSelectColor(KeyEvent_t key);

// 位置选择状态处理（放置模式）
void HandlePlaceMove(KeyEvent_t key);

// 玩家移动/落子状态处理（对战模式）
void HandlePlayerMove(KeyEvent_t key);

// 游戏结束状态处理
void HandleGameOver(KeyEvent_t key);

// ==================== 状态入口函数 ====================

// 进入选择先后手界面时的初始化
void EnterSelectFirst(void);

// 进入游戏对战的初始化
void EnterGamePlay(void);

// 进入颜色选择界面的初始化
void EnterColorSelect(void);

#endif /* __STATE_HANDLERS_H */