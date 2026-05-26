#include "ui.h"
#include "chess_ai.h"  // for EMPTY, BLACK, WHITE, game result definitions

#include "game_types.h"  


extern AppState_t current_state;
extern GameCtrl_t chessgame;


/**
  * @brief  Draw the 3x3 game board with pieces and cursor
  * @note   Uses global OLED functions
  * @retval None
  */
void DrawGameBoard(void)
{
    OLED_Clear(); // 先清屏
    
    // 只在非 GAME_OVER 状态下显示顶部文字
    if (current_state == STATE_AI_THINK)
    {
        OLED_ShowString(0, 0, "AI Turn", OLED_8X16);
    }
    else if (current_state == STATE_PLAYER_MOVE)
    {
        OLED_ShowString(0, 0, "Hu Turn", OLED_8X16);
    }
    // STATE_GAME_OVER 时不绘制任何顶部文字，由外层处理
    
    // 绘制3x3棋盘线
    OLED_DrawLine(43, 16, 43, 60);
    OLED_DrawLine(82, 16, 82, 60);
    OLED_DrawLine(4, 30, 121, 30);
    OLED_DrawLine(4, 44, 121, 44);
    
    // 绘制光标 - 只在玩家移动时显示
    if (current_state == STATE_PLAYER_MOVE)
    {
        int row = chessgame.cursor_pos / 3, col = chessgame.cursor_pos % 3;
        int x0 = 4 + col * 39;
        int y0 = 16 + row * 14;
     
        // 画方框表示光标
        OLED_DrawLine(x0, y0, x0 + 38, y0);         // 上边
        OLED_DrawLine(x0, y0, x0, y0 + 13);         // 左边
        OLED_DrawLine(x0 + 38, y0, x0 + 38, y0 + 13); // 右边
        OLED_DrawLine(x0, y0 + 13, x0 + 38, y0 + 13); // 下边
    }
    
    // 绘制棋子
    for (int i = 0; i < 9; i++)
    {
        if (chessgame.board[i] == EMPTY) continue;
        
        int row = i / 3, col = i % 3;
        int cx = 4 + col * 39 + 20;  // 格子中心x
        int cy = 16 + row * 14 + 7;   // 格子中心y
        
        if (chessgame.board[i] == BLACK)
            OLED_DrawCircle(cx, cy, 5, 1);  // 填充圆
        else
            OLED_DrawCircle(cx, cy, 5, 0);  // 空心圆
    }
    OLED_Update();
}

/**
  * @brief  Draw the main menu screen
  * @param  main_option: Current selected menu option (0-2)
  * @retval None
  */
void DrawMainMenu(uint8_t main_option)
{
    OLED_Clear();
    OLED_ShowString(0, 0, "=== Chess Robot ===", OLED_8X16);
    OLED_ShowString(0, 20, main_option == 0 ? "-> Place Chess" : "   Place Chess", OLED_8X16);
    OLED_ShowString(0, 35, main_option == 1 ? "-> Play Game" : "   Play Game", OLED_8X16);
    OLED_ShowString(0, 50, main_option == 2 ? "-> Text" : "   Text", OLED_8X16);
    OLED_Update();
}

/**
  * @brief  Draw the select first player screen
  * @param  select_option: Current selected option (0-2)
  * @retval None
  */
void DrawSelectFirstMenu(uint8_t select_option)
{
    OLED_Clear();
    OLED_ShowString(0, 0, "Select First:", OLED_8X16);
    OLED_ShowString(0, 20, select_option == 0 ? "-> AI First" : "   AI First", OLED_8X16);
    OLED_ShowString(0, 35, select_option == 1 ? "-> Player First" : "   Player First", OLED_8X16);
    OLED_ShowString(0, 50, select_option == 2 ? "-> Back" : "   Back", OLED_8X16);
    OLED_Update();
}

/**
  * @brief  Draw the game over screen with result
  * @note   Uses DrawGameBoard() to show final board state
  * @retval None
  */
void DrawGameOver(void)
{
    // Draw the final board state first
    OLED_Clear();
    
    // 绘制3x3棋盘线
    OLED_DrawLine(43, 16, 43, 60);
    OLED_DrawLine(82, 16, 82, 60);
    OLED_DrawLine(4, 30, 121, 30);
    OLED_DrawLine(4, 44, 121, 44);
    
    // 绘制棋子（不含光标）
    for (int i = 0; i < 9; i++)
    {
        if (chessgame.board[i] == EMPTY) continue;
        
        int row = i / 3, col = i % 3;
        int cx = 4 + col * 39 + 20;  // 格子中心x
        int cy = 16 + row * 14 + 7;   // 格子中心y
        
        if (chessgame.board[i] == BLACK)
            OLED_DrawCircle(cx, cy, 5, 1);  // 填充圆
        else
            OLED_DrawCircle(cx, cy, 5, 0);  // 空心圆
    }
    
    // 显示结果文字
    if (chessgame.game_result == RESULT_DRAW)
        OLED_ShowString(0, 0, "Draw!", OLED_8X16);
    else if ((chessgame.game_result == RESULT_BLACK_WIN && chessgame.ai_color == BLACK) ||
             (chessgame.game_result == RESULT_WHITE_WIN && chessgame.ai_color == WHITE))
        OLED_ShowString(0, 0, "AI Win!", OLED_8X16);
    else if ((chessgame.game_result == RESULT_BLACK_WIN && chessgame.player_color == BLACK) ||
             (chessgame.game_result == RESULT_WHITE_WIN && chessgame.player_color == WHITE))
        OLED_ShowString(0, 0, "Human Win!", OLED_8X16);
    
    OLED_Update();
}
