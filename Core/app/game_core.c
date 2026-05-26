#include "game_core.h"
#include "chess_ai.h"

static GameMode_t current_mode = GAME_MODE_NONE;

void GameCore_SetMode(GameMode_t mode)
{
    current_mode = mode;
}

int GameCore_PlayerMove(GameCtrl_t *game, uint8_t position)
{
    if (game->board[position] != EMPTY) return 0;
    
    game->board[position] = game->player_color;
    // game->cursor_pos = position;
    game->game_result = CheckGameResult(game->board);
    return 1;  // 落子成功
    // chessgame.board[chessgame.cursor_pos] = chessgame.player_color;
    // chessgame.game_result = CheckGameResult(chessgame.board);
}

int GameCore_AIMove(GameCtrl_t *game, uint8_t ai_color)
{
    int move = AI_GetBestMove(game->board, ai_color);
    
    if (move >= 0)
    {
        game->board[move] = ai_color;
        game->cursor_pos = move;
        game->game_result = CheckGameResult(game->board);
        
        // 如果是实物模式，通知机械臂执行
        if (current_mode == GAME_MODE_PLAY)
        {
            // 触发机械臂动作（通过消息队列或事件标志）
            // ArmControl_ExecuteMove(move, ai_color);
        }
        // TEXT模式不下发机械臂指令，只更新逻辑
    }
    
    return move;
}
