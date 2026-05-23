#include "chess_ai.h"
#include <stdbool.h>

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

/*---------------- 内部辅助：检测某方在某格能否立即获胜 ----------------*/
static bool IsWinningMove(const uint8_t board[9], int pos, uint8_t color)
{
    uint8_t temp[9];
    for (int i = 0; i < 9; i++) temp[i] = board[i];
    temp[pos] = color;
    int result = CheckGameResult(temp);
    return (result == (color == BLACK ? GAME_BLACK_WIN : GAME_WHITE_WIN));
}

/*---------------- AI 决策：轻量规则版 ----------------*/
int AI_GetBestMove(const uint8_t board[9], uint8_t ai_color)
{
    uint8_t opponent = (ai_color == BLACK) ? WHITE : BLACK;

    /* 1. 进攻：如果自己下某格能直接获胜，则下那里 */
    for (int i = 0; i < 9; i++)
    {
        if (board[i] == EMPTY && IsWinningMove(board, i, ai_color))
        {
            return i;
        }
    }

    /* 2. 防守：如果对手下某格会获胜，则抢占该格阻挡 */
    for (int i = 0; i < 9; i++)
    {
        if (board[i] == EMPTY && IsWinningMove(board, i, opponent))
        {
            return i;
        }
    }

    /* 3. 优先级落子：中心 > 角 > 边 */
    static const uint8_t priority[9] = {4, 0, 2, 6, 8, 1, 3, 5, 7};
    for (int i = 0; i < 9; i++)
    {
        uint8_t pos = priority[i];
        if (board[pos] == EMPTY) return pos;
    }

    return -1;   /* 棋盘已满 */
}

/*---------------- 初始化棋盘 ----------------*/
void BoardInit(uint8_t board[9])
{
    for (int i = 0; i < 9; i++) board[i] = EMPTY;
}