#include <stdbool.h>
#include <ctype.h>
#include <math.h>
#include "Board.h"
#include "Game.h"
#include "Movement.h"
#include <stdio.h>

char pieces[] = { 'p', 'r','n', 'b', 'q', 'k'};
int pieceSize = sizeof(pieces) / sizeof(pieces[0]);

bool applyMove(void)
{
    bool movedSuccessfully = false ;
    do
    {
        getMove();
        if( commandStart)
            return false;
        int x1 = command.currentX ;
        int y1 = command.currentY ;
        int x2 = command.nextX;
        int y2 = command.nextY;
        bool flag = checkRightPiece( x1, y1);
        bool kingCheck = !checkIfAvailable( command);
        if( ! flag )
        {
            continue;
        }
        if( kingCheck)
        {
            clearScreen();
            printBoard();
            printError("king check");
            continue;
        }
        if( flag )
            switch(board[command.currentY][command.currentX])
            {
            case 'P':
            case 'p':
                movedSuccessfully = checkSoldier( x1, y1, x2, y2, board[command.currentY][command.currentX]);
                break;
            case 'R':
            case 'r':
                movedSuccessfully = checkRook(  x1, y1, x2, y2, board[command.currentY][command.currentX]);
                break;
            case 'N':
            case 'n':
                movedSuccessfully = checkHorse( x1, y1, x2, y2, board[command.currentY][command.currentX]);
                break;
            case 'B':
            case 'b':
                movedSuccessfully = checkBishop( x1, y1, x2, y2, board[command.currentY][command.currentX]);
                break;
            case 'Q':
            case 'q':
                movedSuccessfully = checkQueen( x1, y1, x2, y2, board[command.currentY][command.currentX]);
                break;
            case 'K':
            case 'k':
                movedSuccessfully = checkKing( x1, y1, x2, y2, board[command.currentY][command.currentX]);
                break;
            }
        if( !(movedSuccessfully)  )
        {
            clearScreen();
            printBoard();
            printError("not valid move");
        }
    }
    while( !(movedSuccessfully) );
    turn++ ;
    maxTurn = turn;
    doPromotion();
    saveMove();
    return true;
}

bool isNotOccupied(void)
{
    int x = command.nextX,y = command.nextY;
    if(board[y][x]=='#' || board[y][x]=='-')
    {
        return true;
    }
    return false ;
}
bool isFirstMove(void)
{
    int x = command.currentX,y = command.currentY;
    if(board[y][x] == 'P')
    {
        if(y == 6)
            return true;
        else
            return false;
    }
    else if(board[y][x] == 'p')
    {
        if(y == 1)
            return true;
        else
            return false;
    }
    return false;
}
bool isValidEat(int x1,int y1,int x2,int y2,char piece)
{
    int deltaY = y2 - y1 , deltaX = x2 - x1;
    switch(piece)
    {
    case 'P':
        if(!isNotOccupied())
        {
            if( ( deltaX == 1 ) && ( deltaY == -1 ) && islower(board[y2][x2] ))
                return true;
            else if( ( deltaX == -1 ) && ( deltaY == -1 ) && islower(board[y2][x2]))
                return true;
            else
                return false;
        }
        break;
    case 'p':
        if(!(isNotOccupied() ) )
        {
            if( ( deltaX == 1 ) && ( deltaY == 1 ) && isupper(board[y2][x2]))
                return true;
            else if( ( deltaX == -1 ) && ( deltaY == 1 ) && isupper(board[y2][x2]))
                return true;
            else
                return false;
        }
        break;
    case 'R':
    case 'Q':
    case 'B':
    case 'N':
        if( islower( board[y2][x2] ) )
            return true;
        else
            return false;
        break;
    case 'K':
        if( islower( board[y2][x2]) )
            return true;
        else
            return false;
        break;
    case 'r':
    case 'q':
    case 'b':
    case 'n':
        if( isupper( board[y2][x2] ) )
            return true;
        else
            return false;
        break;
    case 'k':
        if( isupper( board[y2][x2]) )
            return true;
        else
            return false;
        break;
    }
    return false;
}
void movePiece(void)
{
    board[command.nextY][command.nextX] = board[command.currentY][command.currentX];
    if((command.currentX + command.currentY) % 2 == 0)
        board[command.currentY][command.currentX] = '#';
    else
        board[command.currentY][command.currentX] = '-';
}
bool obstaclesExist(int x1 ,int y1 , int x2 , int y2,char piece)
{
    int deltaX = x2 - x1;
    int deltaY = y2 - y1;
    switch(piece)
    {
    case 'P':
        if(isFirstMove())
        {
            if(board[y1-1][x1] != '#' && board[y1-1][x1] != '-')
                return true;
        }
        break;
    case 'p':
        if(isFirstMove())
        {
            if(board[y1+1][x1] != '#' && board[y1+1][x1] != '-')
                return true;
        }
        break;
    case 'R':
    case 'r':
        if(deltaX == 0)
        {
            if(y2 > y1)
            {
                for(int y = y1+1 ; y < y2 ; y++)
                {
                    if(board[y][x1] != '#' && board[y][x1] != '-')
                    {
                        return true;
                    }
                }
            }
            else if(y2 < y1)
            {
                for(int y = y1-1 ; y > y2 ; y--)
                {
                    if(board[y][x1] != '#' && board[y][x1] != '-')
                    {
                        return true;
                    }
                }
            }
        }
        else if(deltaY == 0)
        {
            if(x2 > x1)
            {
                for(int x = x1+1 ; x < x2 ; x++)
                {
                    if(board[y1][x] != '#' && board[y1][x] != '-')
                    {
                        return true;
                    }
                }
            }
            else if(x2 < x1)
            {
                for(int x = x1-1 ; x > x2 ; x--)
                {
                    if(board[y1][x] != '#' && board[y1][x] != '-')
                    {
                        return true;
                    }
                }
            }
        }
        break;
    case 'B':
    case 'b':
        if(deltaX == deltaY && x2 > x1)
        {
            for(int x = x1+1, y = y1+1 ; x < x2 ; x++)
            {
                if(board[y][x] != '#' && board[y][x] != '-')
                    return true;
                y++;
            }
        }
        else if(deltaX == deltaY && x2 < x1)
        {
            for(int x = x1-1, y = y1-1 ; x > x2 ; x--)
            {
                if(board[y][x] != '#' && board[y][x] != '-')
                    return true;
                y--;
            }
        }
        else if(deltaX == -deltaY && x2 > x1)
        {
            for(int x = x1+1, y = y1-1 ; x < x2 ; x++)
            {
                if(board[y][x] != '#' && board[y][x] != '-')
                    return true;
                y--;
            }
        }
        else if(deltaX == -deltaY && x2 < x1)
        {
            for(int x = x1-1, y = y1+1 ; x > x2 ; x--)
            {
                if(board[y][x] != '#' && board[y][x] != '-')
                    return true;
                y++;
            }
        }
        break;
    case 'Q':
    case 'q':
        if(deltaX == 0)
        {
            if(y2 > y1)
            {
                for(int y = y1+1 ; y < y2 ; y++)
                {
                    if(board[y][x1] != '#' && board[y][x1] != '-')
                    {
                        return true;
                    }
                }
            }
            else if(y2 < y1)
            {
                for(int y = y1-1 ; y > y2 ; y--)
                {
                    if(board[y][x1] != '#' && board[y][x1] != '-')
                    {
                        return true;
                    }
                }
            }
        }
        else if(deltaY == 0)
        {
            if(x2 > x1)
            {
                for(int x = x1+1 ; x < x2 ; x++)
                {
                    if(board[y1][x] != '#' && board[y1][x] != '-')
                    {
                        return true;
                    }
                }
            }
            else if(x2 < x1)
            {
                for(int x = x1-1 ; x > x2 ; x--)
                {
                    if(board[y1][x] != '#' && board[y1][x] != '-')
                    {
                        return true;
                    }
                }
            }
        }
        else if(deltaX == deltaY && x2 > x1)
        {
            for(int x = x1+1, y = y1+1 ; x < x2 ; x++)
            {
                if(board[y][x] != '#' && board[y][x] != '-')
                    return true;
                y++;
            }
        }
        else if(deltaX == deltaY && x2 < x1)
        {
            for(int x = x1-1, y = y1-1 ; x > x2 ; x--)
            {
                if(board[y][x] != '#' && board[y][x] != '-')
                    return true;
                y--;
            }
        }
        else if(deltaX == -deltaY && x2 > x1)
        {
            for(int x = x1+1, y = y1-1 ; x < x2 ; x++)
            {
                if(board[y][x] != '#' && board[y][x] != '-')
                    return true;
                y--;
            }
        }
        else if(deltaX == -deltaY && x2 < x1)
        {
            for(int x = x1-1, y = y1+1 ; x > x2 ; x--)
            {
                if(board[y][x] != '#' && board[y][x] != '-')
                    return true;
                y++;
            }
        }
        break;
    }
    return false;
}

bool checkRightPiece(int x1, int y1)
{

    switch ( board[command.currentY][command.currentX])
    {
    case '#':
    case '-':
        if(!simulation)//checks if it's an actual move or checkmate simulation
        {
            clearScreen();
            printBoard();
            printError("empty block");
        }
        return false;
    }

    if( currentPlayer == firstPlayer && board[y1][x1] == toupper( board[y1][x1] ) )
        return true;
    else if( currentPlayer == secondPlayer && board[y1][x1] ==  tolower(board[y1][x1]) )
        return true;
    else
        if(!simulation)//checks if it's an actual move or checkmate simulation
        {
            clearScreen();
            printBoard();
            printError("move one piece");
        }
    return false;
}

bool checkSoldier(int x1, int y1, int x2, int y2, char type)//checks if the given move is a valid move for pawn
{
    int deltaX = x2 - x1;
    int deltaY = y2 - y1;
    if( type == 'p' )
        deltaY = -(deltaY);
    if(deltaY == -1 && deltaX == 0 && isNotOccupied())
    {
        movePiece();//moves the piece
        if(!simulation)//checks if it's an actual move or checkmate simulation
        {
            deadPieces[turn] = 0;
        }
        return true;
    }
    else if(deltaY == -2 && deltaX == 0 && isNotOccupied() && isFirstMove() && !obstaclesExist(x1,y1,x2,y2,type))
    {
        movePiece();//moves the piece
        if(!simulation)//checks if it's an actual move or checkmate simulation
        {
            deadPieces[turn] = 0;
        }
        return true;
    }
    else if(isValidEat(x1,y1,x2,y2,type))
    {
        if(!simulation)//checks if it's an actual move or checkmate simulation
        {
            addToGraveyard();
            addToDeadPieces();
        }
        movePiece();//moves the piece
        return true;
    }
    else
    {
        return false;
    }
}

bool checkKing(int x1, int y1, int x2, int y2, char type)//checks if the given move is a valid move for king
{
    int deltaX = x2 - x1;
    int deltaY = y2 - y1;
    if( ( (abs(deltaX) == 1 && abs( deltaY) == 0) || ( abs(deltaX) == 1 && abs(deltaY) == 1 ) || (abs(deltaX) == 0 && abs(deltaY) == 1) ))
    {
        if(isValidEat(x1,y1,x2,y2,type))
        {
            if(!simulation)//checks if it's an actual move or checkmate simulation
            {
                addToGraveyard();
                addToDeadPieces();
            }
            movePiece();//moves the piece
            return true;
        }
        if( isNotOccupied())
        {
            movePiece();//moves the piece
            if(!simulation)//checks if it's an actual move or checkmate simulation
            {
                deadPieces[turn] = 0;
            }
            return true;
        }
    }
        return false;
}

bool checkRook( int x1, int y1, int x2, int y2, char type)//checks if the given move is a valid move for rook
{
    int deltaX = x2 - x1;
    int deltaY = y2 - y1;
    if( ( (deltaX == 0 && deltaY != 0) || ( deltaX != 0 && deltaY == 0) ) && !obstaclesExist(x1,y1,x2,y2,type))
    {
        if( isNotOccupied() )
        {
            movePiece();//moves the piece
            if(!simulation)//checks if it's an actual move or checkmate simulation
            {
                deadPieces[turn] = 0;
            }
            return true;
        }
        else if( isValidEat(x1,y1,x2,y2,type) )
        {
            if(!simulation)//checks if it's an actual move or checkmate simulation
            {
                addToGraveyard();
                addToDeadPieces();
            }
            movePiece();//moves the piece
            return true;
        }
    }
    return false;
}

bool checkHorse(int x1, int y1, int x2, int y2, char type)//checks if the given move is a valid move for horse
{
    int deltaX = x2 - x1;
    int deltaY = y2 - y1;
    if( ( abs(deltaX) == 1 && abs(deltaY) == 2) || ( abs(deltaX) == 2 && abs(deltaY) == 1 ) )
    {
        if( isNotOccupied() )
        {
            movePiece();//moves the piece
            if(!simulation)//checks if it's an actual move or checkmate simulation
            {
                deadPieces[turn] = 0;
            }
            return true;
        }
        else if( isValidEat(x1,y1,x2,y2,type) )
        {
            if(!simulation)//checks if it's an actual move or checkmate simulation
            {
                addToGraveyard();
                addToDeadPieces();
            }
            movePiece();//moves the piece
            return true;
        }
    }
    return false;
}
bool checkBishop(int x1, int y1, int x2, int y2, char type)//checks if the given move is a valid move for bishop
{
    int deltaX = x2 - x1;
    int deltaY = y2 - y1;
    // checks if the move is valid and if there are any obstacles in the way
    if( abs(deltaX) == abs(deltaY) && !obstaclesExist(x1,y1,x2,y2,type))
    {
        if( isNotOccupied() )//checks if the new position is already occupied by another piece
        {
            movePiece();//moves the piece
            if(!simulation)//checks if it's an actual move or checkmate simulation
            {
                deadPieces[turn] = 0;
            }
            return true;
        }
        else if( isValidEat(x1,y1,x2,y2,type) )
        {
            if(!simulation)//checks if it's an actual move or checkmate simulation
            {
                addToGraveyard();
                addToDeadPieces();
            }
            movePiece();//moves the piece
            return true;
        }
    }
    return false;
}
bool checkQueen(int x1, int y1, int x2, int y2, char type)//checks if the given move is a valid move for queen
{
    int deltaX = x2 - x1;
    int deltaY = y2 - y1;
    // checks if the move is valid and if there are any obstacles in the way
    if( ( (deltaX == 0 && deltaY != 0) || ( deltaX != 0 && deltaY == 0) ) && !obstaclesExist(x1,y1,x2,y2,type))
    {
        if( isNotOccupied() )//checks if the new position is already occupied by another piece
        {
            movePiece();//moves the piece
            if(!simulation)//checks if it's an actual move or checkmate simulation
            {
                deadPieces[turn] = 0;
            }
            return true;
        }
        else if( isValidEat(x1,y1,x2,y2,type) )//if the position is occupied checks if the piece in it can be captured
        {
            if(!simulation)
            {
                addToGraveyard();//adds the captured piece to the graveyard of its owner
                addToDeadPieces();//adds the captured piece to the list of captured pieces for undo
            }
            movePiece();//moves the piece
            return true;
        }
    }
    // checks if the move is valid and if there are any obstacles in the way
    else if( abs(deltaX) == abs(deltaY) && !obstaclesExist(x1,y1,x2,y2,type))
    {
        if( isNotOccupied() )
        {
            movePiece();
            if(!simulation)
            {
                deadPieces[turn] = 0;
            }
            return true;
        }
        else if( isValidEat(x1,y1,x2,y2,type) )
        {
            if(!simulation)
            {
                addToGraveyard();
                addToDeadPieces();
            }
            movePiece();
            return true;
        }
    }
    return false;
}
void addToGraveyard(void)// adds the piece to the players' respective graveyar
{
    if(currentPlayer == firstPlayer)
    {
        player2Graveyard[graveyard2Size++] = board[command.nextY][command.nextX];
    }
    else
    {
        player1Graveyard[graveyard1Size++] = board[command.nextY][command.nextX];
    }
}
bool isChecked()//finds the king and checks if the king is in check
{
    char king;
    if( currentPlayer == firstPlayer)
        king = 'K';
    else
        king = 'k';
    bool kingCase = isupper(king);
    char piece = '\0';
    int kingX,kingY;
    for(int y1 = 0; y1 < 8 ; y1++)//finds the King of the current player
    {
        for(int x1 = 0 ; x1 < 8 ; x1++)
        {
            if(board[y1][x1] == king)
            {
                kingY = y1;
                kingX = x1;
                break;
            }
        }
    }
    for(int y1 = 0 ; y1 < 8 ; y1++)//finds out whether the King is in check or not
    {
        for(int x1 = 0 ; x1 < 8 ; x1++)
        {
            if(board[y1][x1]!= '#' && board[y1][x1]!= '-')
                {
                    if(isupper(board[y1][x1]) != kingCase)
                    {
                        piece = board[y1][x1];
                        switch(piece)
                        {
                        case 'p':
                        case 'P':
                            if(isValidEat(x1,y1,kingX,kingY,piece))
                            {
                                return true;
                            }
                            break;
                        case 'q':
                        case 'Q':
                            if(checkQueen(x1,y1,kingX,kingY,piece) && !obstaclesExist(x1,y1,kingX,kingY,piece))
                            {
                                return true;
                            }
                            break;
                        case 'b':
                        case 'B':
                            if(checkBishop(x1,y1,kingX,kingY,piece) && !obstaclesExist(x1,y1,kingX,kingY,piece))
                            {
                                return true;
                            }
                            break;
                        case 'r':
                        case 'R':
                            if(checkRook(x1,y1,kingX,kingY,piece) && !obstaclesExist(x1,y1,kingX,kingY,piece))
                            {
                                return true;
                            }
                            break;
                        case 'n':
                        case 'N':
                            if(checkHorse(x1,y1,kingX,kingY,piece))
                            {
                                return true;
                            }
                            break;
//                        case 'k':
//                        case 'K':
//                            if( checkKing(x1,y1,kingX,kingY,piece))
//                            {
//                                return true;
//                            }
//                            break;
                        }
                    }
                }
        }
    }
    return false;
}
