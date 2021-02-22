// This is an implementation of TicTacToe for Windows Console. This was written by Max Elliott as part of a programming test/assignment for 
// Psyonix in February of 2021
#include "TicTacToeBoard.h"


// The code should essentially be self documenting, but if I have time, I will add some simple HTML docs
int main()
{
    std::cout << "Welcome to the TicTacToe Game!\n";

    // To start things off, I am just going to create and test a 3x3 game
    TicTacToeBoard* theGame = new TicTacToeBoard(3, 3);

    TicTacToeBoard::PrintHelp();
    theGame->PrintBoard();

    while (!theGame->IsTimeToQuit())
    {

        std::string result = theGame->AskUserForInput();
        theGame->ProcessInput(result);

        if (theGame->DidSomeoneWin(TicTacToeBoard::cPlayerPiece))
        {
            std::cout << "You won!\n";
            if (theGame->AskToPlayAgain())
            {
                theGame->PrintHelp();
                theGame->ResetBoard();
                theGame->PrintBoard();
            }
            else
            {
                theGame->Quit();
            }
        }

        if (theGame->DidSomeoneWin(TicTacToeBoard::cComputerPiece))
        {
            std::cout << "The Computer won!\n";
            if (theGame->AskToPlayAgain())
            {
                theGame->PrintHelp();
                theGame->ResetBoard();
                theGame->PrintBoard();
            }
            else
            {
                theGame->Quit();
            }
        }

        if (theGame->IsGameADraw())
        {
            std::cout << "The game is a draw!\n";
            if (theGame->AskToPlayAgain())
            {
                theGame->PrintHelp();
                theGame->ResetBoard();
                theGame->PrintBoard();
            }
            else
            {
                theGame->Quit();
            }
        }
    }








}
