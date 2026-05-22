#include "chess_ai.h"
// #include <limits.h>

/*---------------- 胜负判定 ----------------*/
int CheckGameResult(const uint8_t board[9])
{
    static const int winLines[8][3] = {
        {0,1,2}, {3,4,5}, {6,7,8},
        {0,3,6}, {1,4,7}, {2,5,8},
        {0,4,8}, {2,4,6}
    };

    for (int i = 0; i < 8; i++)
    {
        int a = board[winLines[i][0]];
        int b = board[winLines[i][1]];
        int c = board[winLines[i][2]];
        if (a != EMPTY && a == b && b == c)
        {
            return (a == BLACK) ? GAME_BLACK_WIN : GAME_WHITE_WIN;
        }
    }

    for (int i = 0; i < 9; i++)
    {
        if (board[i] == EMPTY) return GAME_ONGOING;
    }
    return GAME_DRAW;
}

/*---------------- Minimax AI 决策 ----------------*/
static int16_t Evaluate(int result, int depth, uint8_t ai_color)
{
    if (result == GAME_BLACK_WIN && ai_color == BLACK) return 10 - depth;
    if (result == GAME_WHITE_WIN && ai_color == WHITE) return 10 - depth;
    if (result == GAME_BLACK_WIN && ai_color != BLACK) return -10 + depth;
    if (result == GAME_WHITE_WIN && ai_color != WHITE) return -10 + depth;
    return 0;
}

static int16_t Minimax(uint8_t board[9], int depth, int isMaximizing, uint8_t ai_color)
{
    int result = CheckGameResult(board);
    if (result != GAME_ONGOING)
    {
        return Evaluate(result, depth, ai_color);
    }

    int16_t best = isMaximizing ? INT16_MIN : INT16_MAX;
    uint8_t currentPlayer = isMaximizing ? ai_color : (ai_color == BLACK ? WHITE : BLACK);

    for (int i = 0; i < 9; i++)
    {
        if (board[i] == EMPTY)
        {
            board[i] = currentPlayer;
            int16_t score = Minimax(board, depth + 1, !isMaximizing, ai_color);
            board[i] = EMPTY;

            if (isMaximizing)
            {
                if (score > best) best = score;
            }
            else
            {
                if (score < best) best = score;
            }
        }
    }
    return best;
}

/*---------------- 接口1：AI决策最佳落子位置 ----------------*/
int AI_GetBestMove(const uint8_t board[9], uint8_t ai_color)
{
    int16_t bestScore = INT16_MIN;
    int bestMove = -1;

    for (int i = 0; i < 9; i++)
    {
        if (board[i] == EMPTY)
        {
            uint8_t tempBoard[9];
            for (int j = 0; j < 9; j++) tempBoard[j] = board[j];
            tempBoard[i] = ai_color;

            int16_t score = Minimax(tempBoard, 0, 0, ai_color);
            if (score > bestScore)
            {
                bestScore = score;
                bestMove = i;
            }
        }
    }
    return bestMove;
}

/*---------------- 初始化棋盘 ----------------*/
void BoardInit(uint8_t board[9])
{
    for (int i = 0; i < 9; i++) board[i] = EMPTY;
}