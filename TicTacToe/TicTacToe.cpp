// This is an implementation of TicTacToe for Windows Console. This was written by Max Elliott as part of a programming test/assignment for 
// Psyonix in February of 2021

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

    TicTacToeBoard(std::uint8_t width, std::uint8_t height)
    {
        iBoardWidth = width;
        iBoardHeight = height;

        CheckAndAdjustSizes();
        AllocateBoardMemory();
    }


    // This is the public interface for playing the game, resizing the board, etc.

    void ResizeBoard(std::uint8_t width, std::uint8_t height)
    {
        delete[] cBoard;
        iBoardWidth = width;
        iBoardHeight = height;

        CheckAndAdjustSizes();
        AllocateBoardMemory();
    }

    void ResetBoard()
    {
        for (std::uint8_t i = 0; i < iBoardWidth * iBoardHeight; i++)
        {
            cBoard[i] = ' ';
        }
        iNumMovesMadeSoFar = 0;
    }

    void PlacePlayerPiece(std::uint8_t location)
    {

    }

    bool DidPlayerWin()
    {

    }

    bool DidComputerWin()
    {

    }

    bool IsGameADraw()
    {

    }

    ~TicTacToeBoard()
    {
        delete[] cBoard;
        delete[] iMoves;
    }

private:

    // Revoke copy construction and assignment
    TicTacToeBoard(const TicTacToeBoard&);
    TicTacToeBoard& operator=(const TicTacToeBoard& rhs);


    // Implementing all the safety checks as internal to the class so that misuse or error is difficult
    void CheckAndAdjustSizes()
    {
        if (iBoardWidth < 3)
        {
            iBoardWidth = 3;
            std::cout << "width was less than 3 during construction of the board\n";
            std::cout << "and has been adjusted so that the board is at least 3x3 in size\n";
        }
        if (iBoardHeight < 3)
        {
            iBoardHeight = 3;
            std::cout << "height was less than 3 during construction of the board\n";
            std::cout << "and has been adjusted so that the board is at least 3x3 in size\n";
        }
    }

    void AllocateBoardMemory()
    {
        cBoard = new char[iBoardWidth * iBoardHeight];
        iMoves = new std::uint8_t[iBoardWidth * iBoardHeight];
        iNumMovesMadeSoFar = 0;
    }

    void PlaceComputerPiece()
    {

    }

    bool IsLegalPlayerMove()
    {

    }


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

};

static void PrintHelp()
{
    std::cout << "You will play against the computer. Your will play 'X' and the computer will play 'O'\n";
    std::cout << "You will get to move first :) \n\n";

    std::cout << "Depending on the size of the board, the numeric identifiers for the squares will look like this:\n\n";
    std::cout << "-------------\n";
    std::cout << "| 0 | 1 | 2 |\n";
    std::cout << "-------------\n";
    std::cout << "| 3 | 4 | 5 |\n";
    std::cout << "-------------\n";
    std::cout << "| 6 | 7 | 8 |\n";
    std::cout << "-------------\n\n";

    std::cout << "Here is a list of the available commands:\n";
    std::cout << "    help: prints this help\n";
    std::cout << "    restart: restarts the game\n";
    std::cout << "    (0-N): chooses a square on the board on which to place your piece\n";
    std::cout << "    resize: prompts for a new set of board dimensions (min 3x3)\n";
    std::cout << "    undo: rewinds the game one step (note that if you choose to undo one of your moves, the computers last move will also be undone)\n";
}

static void PrintBoard()
{

}

static bool IsLegalMove(uint8_t moveLocation)
{

}
// The code should essentially be self documenting, but if I have time, I will add some simple HTML docs
int main()
{
    std::cout << "Welcome to the TicTacToe Game!\n";
    PrintHelp();



}
