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

    TicTacToeBoard(std::uint8_t width, std::uint8_t height);

    // This is the public interface for playing the game, resizing the board, etc.



    void PrintBoard();

    std::string AskUserForInput();

    // Returns true if input was valid/legal and succeeded, else false
    bool ProcessInput(std::string input);

    void Quit();

    bool IsTimeToQuit();

    void Undo();

    bool DidPlayerWin();

    bool DidComputerWin();

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

    void ResizeBoard(std::uint8_t width, std::uint8_t height);


    void PlacePlayerPiece(std::uint8_t location);

    std::uint8_t WhichRow(std::uint8_t location);
    std::uint8_t WhichColumn(std::uint8_t location);

    bool CanRowBeWon(std::uint8_t row);

    bool CanColumnBeWon(std::uint8_t row);
    bool CanDiagonalBeWon();
    bool HasDiagonalBeWon();
    bool HasRowBeWon(std::uint8_t row);
    bool HasColumnBeWon(std::uint8_t row);



    // Implementing all the safety checks as internal to the class so that misuse or error is difficult
    void CheckAndAdjustSizes();

    void AllocateBoardMemory();

    void PlaceComputerPiece();

    bool IsLegalPlayerMove(uint8_t moveLocation);

    void PrintRowOfDashes();

    bool IsInoutANumber(std::string input);

    uint8_t GetInputNumber(std::string input);



    // The dimensions of the board
    std::uint8_t iBoardWidth, iBoardHeight;

    // The board data structure. We are choosing char as the data type as it is compact, human readable and easily printable
    // A value of 'X' indicates that the X player owns the square
    // A value of 'O' indicates that the O player owns the square
    // A value of ' ' indicates that the square is currently empty
    char* cBoard = NULL;

    // This is the history of the moves made by the player
    std::uint8_t iNumMovesMadeSoFar;
    std::uint8_t* iMoves;

    bool bTimeToQuit = false;

};



