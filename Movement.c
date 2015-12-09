#ifndef MOVEMENT_C_INCLUDED
#define MOVEMENT_C_INCLUDED
#include <stdbool.h>
#include "Board.h"
#include "Game.h"
bool applyMove(void)
{
    if(currentPlayer == firstPlayer)
    {
        switch(board[command.currentX][command.currentY])
        {
        case 'P':
                if(command.currentY-command.nextY == 1 && isNotOccupied(command.nextX,command.nextY))
                {
                        board[command.nextX][command.nextY] = board[command.currentX][command.currentY];
                        if((command.currentX + command.currentY) % 2 == 0)
                            board[command.currentX][command.currentY] = '#';
                        else
                            board[command.currentX][command.currentY] = '-';
                }
        }

    }
}
bool isNotOccupied(int x, int y)
{
    if(board[x][y]=='#'||board[x][y]=='-')
    {
        return true;
    }
    return false ;
}

#endif // MOVEMENT_C_INCLUDED
