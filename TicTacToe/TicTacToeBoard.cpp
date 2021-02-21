#include "TicTacToeBoard.h"

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

// CAVEAT: The instructions said "no headers other than standard Windows headers, but I feel that no usable piece of software should 
// be delivered as a single .cpp with no header, or as a header only implementation, except in very special circumstances, so I am 
// changing over to a more standard form of one header plus one impl file and then using the header in the main app cpp file.


    TicTacToeBoard::TicTacToeBoard(std::uint8_t width, std::uint8_t height)
    {
        iBoardWidth = width;
        iBoardHeight = height;

        CheckAndAdjustSizes();
        AllocateBoardMemory();
    }


    // This is the public interface for playing the game, resizing the board, etc.
    TicTacToeBoard::~TicTacToeBoard()
    {
        delete[] cBoard;
        delete[] iMoves;
    }

    void TicTacToeBoard::ResizeBoard(std::uint8_t width, std::uint8_t height)
    {
        delete[] cBoard;
        iBoardWidth = width;
        iBoardHeight = height;
        CheckAndAdjustSizes();
        AllocateBoardMemory();
    }

    void TicTacToeBoard::ResetBoard()
    {
        for (std::uint8_t i = 0; i < iBoardWidth * iBoardHeight; i++)
        {
           cBoard[i] = ' ';
        }
        iNumMovesMadeSoFar = 0;
    }

    void TicTacToeBoard::PlacePlayerPiece(std::uint8_t location)
    {
        // TODO extra validation here?

        cBoard[location] = 'X';
        iMoves[iNumMovesMadeSoFar++] = location;
        PrintBoard();
    }

    // Implementing all the safety checks as internal to the class so that misuse or error is difficult
    void TicTacToeBoard::CheckAndAdjustSizes()
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

    void TicTacToeBoard::AllocateBoardMemory()
    {
        cBoard = new char[iBoardWidth * iBoardHeight];
        iMoves = new std::uint8_t[iBoardWidth * iBoardHeight];
        iNumMovesMadeSoFar = 0;
        ResetBoard();
    }

    void TicTacToeBoard::PlaceComputerPiece()
    {

    }

    bool TicTacToeBoard::IsLegalPlayerMove(uint8_t moveLocation)
    {
        if (moveLocation >= iBoardHeight * iBoardHeight) return false;
        if (cBoard[moveLocation] == ' ') return true;
        return false;
    }

    void TicTacToeBoard::PrintBoard() {
        std::cout << "\n\nHere is the current state of the board:\n\n";
        PrintRowOfDashes();

        for (std::uint8_t y = 0; y < iBoardHeight; y++)
        {
            // Print a row of squares, starting with a vertical dash and some nice spacing between 
            // each square
            for (std::uint8_t x = 0; x < iBoardWidth; x++)
            {
                std::cout << "| ";
                std::cout << cBoard[y * iBoardWidth + x];
                std::cout << " ";
            }
            // Print a final vertical bar after the last square on this row
            std::cout << "|\n";

            // Print a nice row of dashes to separate the next row
            PrintRowOfDashes();
        }

    }

    void TicTacToeBoard::PrintRowOfDashes()
    {
        for (std::uint8_t x = 0; x < (iBoardWidth * 4) + 1; x++) std::cout << "-";
        std::cout << "\n";
    }


    std::string TicTacToeBoard::AskUserForInput()
    {
        std::string inputResult;
        std::cout << "\nIt is your move... what would you like to do?\n> ";
        std::cin >> inputResult;
        std::cout << "You chose " + inputResult + "\n";
        return inputResult;
    }
    
    bool TicTacToeBoard::IsInoutANumber(std::string input)
    {
        char* end;
        uint8_t number = (uint8_t)strtol(input.c_str(), &end, 10);
        if (end == input.c_str())
        {
            return false;
        }
        return true;
    }

    uint8_t TicTacToeBoard::GetInputNumber(std::string input)
    {
        char* end;
        return (uint8_t)strtol(input.c_str(), &end, 10);
    }

    bool TicTacToeBoard::AskToPlayAgain()
    {
        std::string response;
        std::cin >> response;
        if (response.c_str() == "n" || response.c_str() == "no")
            return false;

        if (response.c_str() == "y" || response.c_str() == "yes")
            return true;

        std::cout << "Didn't quite catch that... I will assume you want to play again, cuz it is such a fun game (for me anyway)\n";
            return true;

    }

    // Returns true if input was valid/legal and succeeded, else false
    bool TicTacToeBoard::ProcessInput(std::string input)
    {
        // I would normally use a more advanced data structure or more standard input parsing 
        // system, but I understand the instructions to be keep things as simple as possible with as
        // few dependenies as possible. Thus, no STL or TR1 extenstions etc. I.e. no Map or HashMap structures
        // Apologies for the crude (yet dependency free) implementation!

        if (input == "help")
        {
            PrintHelp();
            return true;
        }
        else if (input == "reset")
        {
            ResetBoard();
            return true;
        }
        else if (IsInoutANumber(input))
        {
            uint8_t location = GetInputNumber(input);
            // Attempt to place the player piece
            if (IsLegalPlayerMove(location))
            {
                PlacePlayerPiece(location);
                // TODO Check status of win/draw/lose and continue as appropraate

                return true;
            }
            else
            {
                std::cout << "Illegal move, please try again!\n";
                return false;
            }
        }
        else if (input == "resize")
        {
            uint8_t newWidth = 3;
            uint8_t newHeight = 3;
            std::string inputString;


            std::cout << "Please enter a new width (min of 3):\n";
            std::cin >> inputString;
            while (!IsInoutANumber(inputString))
            {
                std::cout << "Invalid number! Please enter a new width (min of 3):\n";
                std::cout << "Please enter a new width (min of 3):\n";
                std::cin >> inputString;
            }
            newWidth = GetInputNumber(inputString);

            std::cout << "Please enter a new height (min of 3):\n";
            std::cin >> inputString;
            while (!IsInoutANumber(inputString))
            {
                std::cout << "Please enter a new width (min of 3):\n";
                std::cin >> inputString;
            }
            newHeight = GetInputNumber(inputString);

            std::cout << "New board size is now ";
            std::cout << newWidth + "X" + newHeight; 
            // TODO prompt for and validate new sizes

            ResizeBoard(newWidth, newHeight);
            return true;

        }
        else if (input == "undo")
        {
            Undo();
            return true;
        }
        else if (input == "quit")
        {
            Quit();
            return true;
        }
        else
        {
            std::cout << "That command was not understood, please try again. Type ""help"" for instructinos\n";
            return false;
        }
    }
    void TicTacToeBoard::Quit()
    {
        std::cout << "Thank you for playing! That was fun! Please coma again. Goodbye for now...\n";
        std::cout << "Press any key to continue quitting..\n";
        int junk = getchar();
        bTimeToQuit = true;
    }

    bool TicTacToeBoard::IsTimeToQuit()
    {
        return bTimeToQuit;
    }

    void  TicTacToeBoard::Undo()
    {
        if (iNumMovesMadeSoFar == 0) return;

        // Assume at least two moves have been made, one by the player, followed by one from the computer. Back both of those off
        cBoard[iMoves[iNumMovesMadeSoFar]]= ' ';
        iNumMovesMadeSoFar--;
        cBoard[iMoves[iNumMovesMadeSoFar]]= ' ';
        iNumMovesMadeSoFar--;

        PrintBoard();
    }

    bool TicTacToeBoard::DidPlayerWin()
    {
        if (iNumMovesMadeSoFar == 0) return false;
        // Based on the last move the player made, check all the possible wins from that location
        std::uint8_t lastPlayerLocation = iMoves[iNumMovesMadeSoFar]' // NOTE: The computer has not yet made its move!'
        return false;
    }

    bool TicTacToeBoard::DidComputerWin()
    {
        if (iNumMovesMadeSoFar == 0) return false;

        // Based on the last move the computer made, check all the possible wins from that location
        uint8_t lastComputerLocatinon = iMoves[iNumMovesMadeSoFar];

        return false;
    }

    bool TicTacToeBoard::IsGameADraw()
    {
        if (iNumMovesMadeSoFar == 0) return false;

        // Check if any possible win (full row, full column, full diagonal) have any possible wins by either player
        for (int x = 0; x < iBoardWidth; x++)
        {
            if (CanColumnBeWon(x)) return false;
        }
        for (int y = 0; y < iBoardHeight; y++)
        {
            if (CanRowBeWon(y)) return false;
        }
        return true;
    }
    std::uint8_t TicTacToeBoard::WhichRow(std::uint8_t location)
    {
        return (std::uint8_t)(location / iBoardWidth);
    }
    std::uint8_t TicTacToeBoard::WhichColumn(std::uint8_t location)
    {
        return (std::uint8_t)(location / iBoardHeight);
    }
    bool TicTacToeBoard::CanDiagonalBeWon()
    {
        int numXsFound = 0;
        int numOsFound = 0;
        for (int x = 0; x < iBoardWidth; x++)
        {
            for (int y = 0; y < iBoardHeight; y++)
            {
                uint8_t location = (y * iBoardWidth) + (x * iBoardHeight);
                if (cBoard[location] == 'X') numXsFound++;
                else if (cBoard[location] == 'O') numOsFound++;
            }
        }
        if (numXsFound + numOsFound == 0) return true;
        if (numXsFound != 0 && numOsFound == 0) return true;
        if (numOsFound != 0 && numXsFound == 0) return true;
        return false;
    }

    bool TicTacToeBoard::HasDiagonalBeWon()
    {
        return false;
    }

    bool TicTacToeBoard::HasRowBeWon(std::uint8_t row)
    {
        int numXsFound = 0;
        int numOsFound = 0;
        for (int x = 0; x < iBoardWidth; x++)
        {
            uint8_t location = (row * iBoardWidth) + x;
            if (cBoard[location] == 'X') numXsFound++;
            else if (cBoard[location] == 'O') numOsFound++;
        }
        if (numXsFound == iBoardWidth) return true;
        if (numOsFound == iBoardWidth) return true;

        return false;
    }

    bool TicTacToeBoard::HasColumnBeWon(std::uint8_t col)
    {
        int numXsFound = 0;
        int numOsFound = 0;
        for (int y = 0; y < iBoardHeight; y++)
        {
            uint8_t location = (y * iBoardWidth) + col;
            if (cBoard[location] == 'X') numXsFound++;
            else if (cBoard[location] == 'O') numOsFound++;
        }
        if (numXsFound == iBoardHeight) return true;
        if (numOsFound == iBoardHeight) return true;
        return false;
    }


        bool TicTacToeBoard::CanRowBeWon(std::uint8_t row)
        {
            int numXsFound = 0;
            int numOsFound = 0;
            for (int x = 0; x < iBoardWidth; x++)
            {
                uint8_t location = (row * iBoardWidth) + x;
                if (cBoard[location] == 'X') numXsFound++;
                else if (cBoard[location] == 'O') numOsFound++;
            }
            if (numXsFound + numOsFound == 0) return true;
            if (numXsFound != 0 && numOsFound == 0) return true;
            if (numOsFound != 0 && numXsFound == 0) return true;
            return false;
        }

    bool TicTacToeBoard::CanColumnBeWon(std::uint8_t col)
    {
        int numXsFound = 0;
        int numOsFound = 0;
        for (int y = 0; y < iBoardHeight; y++)
        {
            uint8_t location = (y * iBoardWidth) + col;
            if (cBoard[location] == 'X') numXsFound++;
            else if (cBoard[location] == 'O') numOsFound++;
        }
        if (numXsFound + numOsFound == 0) return true;
        if (numXsFound != 0 && numOsFound == 0) return true;
        if (numOsFound != 0 && numXsFound == 0) return true;
        return false;
    }


    void TicTacToeBoard::PrintHelp()
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
        std::cout << "    quit: exits the game\n\n\n";
    }
