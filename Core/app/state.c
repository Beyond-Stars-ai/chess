#include "state.h"
#include "game_core.h"
#include "ui.h"
#include "cmsis_os.h"
#include "chess_ai.h"

// 外部全局变量声明（实际定义在 main.c）
extern AppState_t current_state;
extern GameCtrl_t chessgame;
extern UIOptions_t ui_options;
extern osThreadId_t Task_AILogicHandle;

// ==================== 颜色选择入口 ====================

void EnterColorSelect(void)
{
    ui_options.color_option = CELL_BLACK;
}

// ==================== 先后手选择入口 ====================

void EnterSelectFirst(void)
{
    chessgame.ai_color = BLACK;
    chessgame.player_color = WHITE;
    ui_options.select_option = SELECT_AI_FIRST;
}

// ==================== 游戏对战入口 ====================

void EnterGamePlay(void)
{
    BoardInit(chessgame.board);
    chessgame.cursor_pos = 4;
    chessgame.current_turn = (chessgame.ai_color == BLACK) ? 1 : 0;
    chessgame.game_result = RESULT_ONGOING;
    
    if (chessgame.current_turn == 1)  // AI先手
    {
        current_state = STATE_AI_THINK;
        osDelay(200);
        osThreadFlagsSet(Task_AILogicHandle, FLAG_AI_START);
    }
    else  // 玩家先手
    {
        current_state = STATE_PLAYER_MOVE;
    }
}

// ==================== 主菜单处理 ====================

void HandleMainMenu(KeyEvent_t key)
{
    switch (key)
    {
        case KEY_UP:
            if (ui_options.main_option > MAIN_PLACE)
                ui_options.main_option--;
            break;
            
        case KEY_DOWN:
            if (ui_options.main_option < MAIN_TEXT)
                ui_options.main_option++;
            break;
            
        case KEY_CONFIRM:
            if (ui_options.main_option == MAIN_GAME)
            {
                GameCore_SetMode(GAME_MODE_PLAY);
                current_state = STATE_SELECT_FIRST;
                EnterSelectFirst();
            }
            else if (ui_options.main_option == MAIN_PLACE)
            {
                current_state = STATE_COLOR_SELECT;
                EnterColorSelect();
            }
            else if (ui_options.main_option == MAIN_TEXT)
            {
                GameCore_SetMode(GAME_MODE_TEXT);
                current_state = STATE_SELECT_FIRST;
                EnterSelectFirst();
            }
            break;
            
        default:
            break;
    }
}

// ==================== 先后手选择处理 ====================

void HandleSelectFirst(KeyEvent_t key)
{
    switch (key)
    {
        case KEY_UP:
            if (ui_options.select_option > SELECT_AI_FIRST)
                ui_options.select_option--;
            break;
            
        case KEY_DOWN:
            if (ui_options.select_option < SELECT_BACK)
                ui_options.select_option++;
            break;
            
        case KEY_CONFIRM:
            if (ui_options.select_option == SELECT_AI_FIRST)
            {
                chessgame.ai_color = BLACK;
                chessgame.player_color = WHITE;
                EnterGamePlay();
            }
            else if (ui_options.select_option == SELECT_PLAYER_FIRST)
            {
                chessgame.ai_color = WHITE;
                chessgame.player_color = BLACK;
                EnterGamePlay();
            }
            else if (ui_options.select_option == SELECT_BACK)
            {
                current_state = STATE_MAIN_MENU;
                ui_options.main_option = MAIN_GAME;
                ui_options.select_option = SELECT_AI_FIRST;
            }
            break;
            
        default:
            break;
    }
}

// ==================== 玩家移动/落子处理（对战模式） ====================

void HandlePlayerMove(KeyEvent_t key)
{
    switch (key)
    {
        case KEY_UP:
            if (chessgame.cursor_pos >= 3) chessgame.cursor_pos -= 3;
            break;
            
        case KEY_DOWN:
            if (chessgame.cursor_pos <= 5) chessgame.cursor_pos += 3;
            break;
            
        case KEY_LEFT:
            if (chessgame.cursor_pos % 3 != 0) chessgame.cursor_pos--;
            break;
            
        case KEY_RIGHT:
            if (chessgame.cursor_pos % 3 != 2) chessgame.cursor_pos++;
            break;
            
        case KEY_CONFIRM:
            if (chessgame.board[chessgame.cursor_pos] == EMPTY)
            {
            chessgame.board[chessgame.cursor_pos] = chessgame.player_color;
            chessgame.game_result = CheckGameResult(chessgame.board);
                
            if (chessgame.game_result != RESULT_ONGOING)
            {
            current_state = STATE_GAME_OVER;
            }
            else
            {
            current_state = STATE_AI_THINK;
            osDelay(200);
            osThreadFlagsSet(Task_AILogicHandle, FLAG_AI_START);
            }
                
            }
            break;
            
        default:
            break;
    }
}

// ==================== 颜色选择处理（放置模式） ====================

void HandleSelectColor(KeyEvent_t key)
{
    switch (key)
    {
        case KEY_UP:
            if (ui_options.color_option > CELL_BLACK)
                ui_options.color_option--;
            break;
            
        case KEY_DOWN:
            if (ui_options.color_option < COLOR_BACK)
                ui_options.color_option++;
            break;
            
        case KEY_CONFIRM:
            if (ui_options.color_option == COLOR_BACK)
            {
                current_state = STATE_MAIN_MENU;
                ui_options.main_option = MAIN_PLACE;
            }
            else
            {
                current_state = STATE_PLACE_SELECT;
                chessgame.cursor_pos = 4;
            }
            break;
            
        default:
            break;
    }
}

// ==================== 位置选择处理（放置模式） ====================

void HandlePlaceMove(KeyEvent_t key)
{
    switch (key)
    {
        case KEY_UP:
            if (chessgame.cursor_pos >= 3) chessgame.cursor_pos -= 3;
            break;
            
        case KEY_DOWN:
            if (chessgame.cursor_pos <= 5) chessgame.cursor_pos += 3;
            break;
            
        case KEY_LEFT:
            if (chessgame.cursor_pos % 3 != 0) chessgame.cursor_pos--;
            break;
            
        case KEY_RIGHT:
            if (chessgame.cursor_pos % 3 != 2) chessgame.cursor_pos++;
            break;
            
        case KEY_CONFIRM:
        {
            CellState_t color = (ui_options.color_option == CELL_BLACK) ? CELL_BLACK : CELL_WHITE;
            chessgame.board[chessgame.cursor_pos] = color;
            
            current_state = STATE_COLOR_SELECT;
            ui_options.color_option = CELL_BLACK;
            break;
        }
            
        default:
            break;
    }
}

// ==================== 游戏结束处理 ====================

void HandleGameOver(KeyEvent_t key)
{
    switch (key)
    {
        case KEY_CONFIRM:
            BoardInit(chessgame.board);
            chessgame.cursor_pos = 4;
            chessgame.ai_color = BLACK;
            chessgame.player_color = WHITE;
            chessgame.current_turn = 0;
            chessgame.game_result = RESULT_ONGOING;
            
            current_state = STATE_MAIN_MENU;
            ui_options.main_option = MAIN_GAME;
            break;
            
        default:
            break;
    }
}