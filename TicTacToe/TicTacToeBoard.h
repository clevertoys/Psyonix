#pragma once
#include <iostream>
#include <cstdint>

// We will need a few things as part of the core architecture/functionality
// 1) A data structure to represent the state of the board
// 2) A method for checking the state of the board for win/lose/draw
// 3) A way to display the current state of the board
// 4) Some command/input parsing
// 5) A help system
// 6) Some documentation

// BONUS POINTS:
// 1) An undo system that lets the user rewind/forward the game arbitrarily
// 2) Support a generalized m,n,k variation of the game 

// Normally I would separate things out into a number of header and implementation files, but since this is so small
// and because I want to make it easy for the reviewers, I am just going to implement the whole shebang right here

class TicTacToeBoard
{
public:

    static const char cPlayerPiece = 'X';
    static const char cComputerPiece = 'O';


    TicTacToeBoard(int width, int height);

    // This is the public interface for playing the game, resizing the board, etc.



    void PrintBoard();

    std::string AskUserForInput();

    // Returns true if input was valid/legal and succeeded, else false
    bool ProcessInput(std::string input);

    void Quit();

    bool IsTimeToQuit();

    void Undo();

    bool DidSomeoneWin(const char piece);

    bool IsGameADraw();

    void ResetBoard();

    bool AskToPlayAgain();

    // Marking this method as static as it does not depend on any members of the instance. Might change this for simplicity of use,
    // But wanted to at least make this observation
static void PrintHelp();

    ~TicTacToeBoard();

private:

    // Revoke copy construction and assignment
    TicTacToeBoard(const TicTacToeBoard&);
    TicTacToeBoard& operator=(const TicTacToeBoard& rhs);

    void ResizeBoard(int width, int height);


    void PlacePlayerPiece(int location);
    void PlaceComputerPiece(int location);

    int WhichRow(int location);
    int WhichColumn(int location);

    int CalculateBestComputerMove();

    bool HasDiagonalBeenWon(int diag, const char piece);
    bool HasRowBeenWon(int row, const char piece);
    bool HasColumnBeenWon(int row, const char piece);

    // These functions return the empty square that remains in a row or column that a player
    // is about to win. If the player is not about to win, they return -1
    int CheckSomeoneAboutToWinRow(int row, const char piece);
    int CheckSomeoneAboutToWinCol(int col, const char piece);
    int CheckSomeoneAboutToWinDiag(int diag, const char piece);



    // Implementing all the safety checks as internal to the class so that misuse or error is difficult
    void CheckAndAdjustSizes();

    void AllocateBoardMemory();

    bool IsLegalPlayerMove(uint8_t moveLocation);

    void PrintRowOfDashes();

    bool IsInputANumber(std::string input);

    uint8_t GetInputNumber(std::string input);



    // The dimensions of the board
    int iBoardWidth, iBoardHeight;

    // The board data structure. We are choosing char as the data type as it is compact, human readable and easily printable
    // A value of 'X' indicates that the X player owns the square
    // A value of 'O' indicates that the O player owns the square
    // A value of ' ' indicates that the square is currently empty
    char* cBoard = NULL;

    // This is the history of the moves made by the player
    int iNumMovesMadeSoFar;
    int* iMoves;

    bool bTimeToQuit = false;


};



