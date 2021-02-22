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


TicTacToeBoard::TicTacToeBoard(int width, int height)
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

void TicTacToeBoard::ResizeBoard(int width, int height)
{
	delete[] cBoard;
	iBoardWidth = width;
	iBoardHeight = height;
	CheckAndAdjustSizes();
	AllocateBoardMemory();
}

void TicTacToeBoard::ResetBoard()
{
	for (int i = 0; i < iBoardWidth * iBoardHeight; i++)
	{
		cBoard[i] = ' ';
	}
	iNumMovesMadeSoFar = 0;
}

void TicTacToeBoard::PlacePlayerPiece(int location)
{
	// TODO extra validation here?

	cBoard[location] = cPlayerPiece;
	iMoves[iNumMovesMadeSoFar++] = location;
	PrintBoard();
	if (DidSomeoneWin(cPlayerPiece) || IsGameADraw()) return;

	std::cout << "It is now the Computer's turn...\n";
	int computerMoveLocation = CalculateBestComputerMove();
	PlaceComputerPiece(computerMoveLocation);
	PrintBoard();
}
void TicTacToeBoard::PlaceComputerPiece(int location)
{
	cBoard[location] = cComputerPiece;
	iMoves[iNumMovesMadeSoFar++] = location;
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
	iMoves = new int[iBoardWidth * iBoardHeight];
	iNumMovesMadeSoFar = 0;
	ResetBoard();
}



bool TicTacToeBoard::IsLegalPlayerMove(uint8_t moveLocation)
{
	if (moveLocation >= iBoardHeight * iBoardHeight) return false;
	if (cBoard[moveLocation] == ' ') return true;
	return false;
}

void TicTacToeBoard::PrintBoard() {
	//std::cout << "\n\nHere is the current state of the board:\n\n";
	PrintRowOfDashes();

	for (int y = 0; y < iBoardHeight; y++)
	{
		// Print a row of squares, starting with a vertical dash and some nice spacing between 
		// each square
		for (int x = 0; x < iBoardWidth; x++)
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
	for (int x = 0; x < (iBoardWidth * 4) + 1; x++) std::cout << "-";
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

bool TicTacToeBoard::IsInputANumber(std::string input)
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
	std::cout << "Would you like to play another game?\n";

	std::string response;
	std::cin >> response;

 
	if (response == "n" || response == "no")
		return false;

	if (response == "y" || response == "yes")
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
	else if (IsInputANumber(input))
	{
	uint8_t location = GetInputNumber(input);
	// Attempt to place the player piece
	if (IsLegalPlayerMove(location))
	{
		PlacePlayerPiece(location);
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
	while (!IsInputANumber(inputString))
	{
		std::cout << "Invalid number! Please enter a new width (min of 3):\n";
		std::cout << "Please enter a new width (min of 3):\n";
		std::cin >> inputString;
	}
	newWidth = GetInputNumber(inputString);

	std::cout << "Please enter a new height (min of 3):\n";
	std::cin >> inputString;
	while (!IsInputANumber(inputString))
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
	cBoard[iMoves[iNumMovesMadeSoFar - 1]] = ' ';
	iNumMovesMadeSoFar--;
	cBoard[iMoves[iNumMovesMadeSoFar - 1]] = ' ';
	iNumMovesMadeSoFar--;

	PrintBoard();
}

bool TicTacToeBoard::DidSomeoneWin(const char piece)
{
	if (iNumMovesMadeSoFar < 6) return false;

	for (int row = 0; row < iBoardHeight; row++)
	{
		if (HasRowBeenWon(row, piece)) return true;
	}
	for (int col = 0; col < iBoardWidth; col++)
	{
		if (HasColumnBeenWon(col, piece)) return true;
	}

	if (HasDiagonalBeenWon(0, piece)) return true;
	if (HasDiagonalBeenWon(1, piece)) return true;

	return false;
}

bool TicTacToeBoard::IsGameADraw()
{
	if (iNumMovesMadeSoFar != iBoardHeight * iBoardWidth) return false;

	if (DidSomeoneWin(cComputerPiece)) return false;
	if (DidSomeoneWin(cPlayerPiece)) return false;
	return true;
}
int TicTacToeBoard::WhichRow(int location)
{
	return (int)(location / iBoardHeight);
}
int TicTacToeBoard::WhichColumn(int location)
{
	return (int)(location % iBoardWidth);
}

int TicTacToeBoard::CalculateBestComputerMove()
{
	// There are some basic strategies to employ...
	// First, if the user is about to win, a blocking move should be made
	// Second, if any row or column can be won by the computer, then pick one of those starting
	// with one that blocks as many other possible wins by the player
	// TODO

	int location = -1;

	// Check to see if we are about to win any row or column
	for (int x = 0; x < iBoardWidth; x++)
	{
		location = CheckSomeoneAboutToWinCol(x, cComputerPiece);
		if (location != -1) return location;
	}

	for (int y = 0; y < iBoardHeight; y++)
	{
		location = CheckSomeoneAboutToWinRow(y, cComputerPiece);
		if (location != -1) return location;
	}

	// Check if we are about to win diagonally
	location = CheckSomeoneAboutToWinDiag(0, cComputerPiece);
	if (location != -1) return location;
	location = CheckSomeoneAboutToWinDiag(1, cComputerPiece);
	if (location != -1) return location;

	// Now check for good row and column blocking moves
	for (int x = 0; x < iBoardWidth; x++)
	{
		location = CheckSomeoneAboutToWinCol(x, cPlayerPiece);
		if (location != -1) return location;
	}

	for (int y = 0; y < iBoardHeight; y++)
	{
		location = CheckSomeoneAboutToWinRow(y, cPlayerPiece);
		if (location != -1) return location;
	}

	// Check if we are about to win diagonally
	location = CheckSomeoneAboutToWinDiag(0, cComputerPiece);
	if (location != -1) return location;
	location = CheckSomeoneAboutToWinDiag(1, cComputerPiece);
	if (location != -1) return location;

	// Just pick a random one for now, later we will pick a 'smart' one
	int min = 0;
	int max = iBoardHeight * iBoardWidth - 1;
	location = min + (rand() % static_cast<int>(max - min + 1));
	while (cBoard[location] != ' ')
	{
		location = min + (rand() % static_cast<int>(max - min + 1));
	}
	return location;
}

bool TicTacToeBoard::HasDiagonalBeenWon(int diag, const char piece)
{
	int numPiecesFound = 0;

	int increment = iBoardWidth + 1;
	int location = 0;
	if (diag == 1)
	{
		increment = -(iBoardWidth + 1);
		location = iBoardWidth - 1;
	}

	for (int i = 0; i < iBoardWidth; i++)
	{
		if (cBoard[location] == piece)
		{
			numPiecesFound++;
		}
		location += increment;
	}
	if (numPiecesFound == iBoardWidth) return true;
	return false;
}

bool TicTacToeBoard::HasRowBeenWon(int row, const char piece)
{
	int numPiecesFound = 0;
	for (int x = 0; x < iBoardWidth; x++)
	{
		uint8_t location = (row * iBoardWidth) + x;
		if (cBoard[location] == piece) numPiecesFound++;
	}
	if (numPiecesFound == iBoardWidth) return true;

	return false;
}

bool TicTacToeBoard::HasColumnBeenWon(int col, const char piece)
{
	int numPiecesFound = 0;
	for (int y = 0; y < iBoardHeight; y++)
	{
		uint8_t location = (y * iBoardWidth) + col;
		if (cBoard[location] == piece) numPiecesFound++;
	}
	if (numPiecesFound == iBoardHeight) return true;
	return false;
}

int TicTacToeBoard::CheckSomeoneAboutToWinRow(int row, const char piece)
{
	int numPiecesFound = 0;
	int blankSquare = -1;
	for (int x = 0; x < iBoardWidth; x++)
	{
		int location = row * iBoardHeight + x;
		if (cBoard[location] == piece)
		{
			numPiecesFound++;
		}
		else if (cBoard[location] == ' ')
		{
			blankSquare = location;
		}
	}
	if (numPiecesFound == iBoardWidth - 1) return blankSquare;
	return -1;
}

int TicTacToeBoard::CheckSomeoneAboutToWinCol(int col, const char piece)
{
	int numPiecesFound = 0;
	int blankSquare = -1;
	for (int y = 0; y < iBoardHeight; y++)
	{
		int location = col + iBoardWidth * y;
		if (cBoard[location] == piece)
		{
			numPiecesFound++;
		}
		else if(cBoard[location] == ' ')
		{
			blankSquare = location;
		}
	}
	if (numPiecesFound == iBoardWidth - 1) return blankSquare;
	return -1;
}

// This is a bit screwy if the board is non square... I will do some research on it...
int TicTacToeBoard::CheckSomeoneAboutToWinDiag(int diag, const char piece)
{
	int numPiecesFound = 0;
	int blankSquare = -1;

	int increment = iBoardWidth + 1;
	int location = 0;
	if (diag == 1)
	{
		increment = -(iBoardWidth + 1);
		location = iBoardWidth - 1;
	}

	for (int i = 0; i < iBoardWidth; i++)
	{
		if (cBoard[location] == piece)
		{
			numPiecesFound++;
		}
		else if (cBoard[location] == ' ')
		{
			blankSquare = location;
		}

		location += increment;
	}
	if (numPiecesFound == iBoardWidth - 1) return blankSquare;
	return -1;
}

void TicTacToeBoard::PrintHelp()
{
	std::cout << "You will play against the computer. Your will play cPlayerPiece and the computer will play cComputerPiece\n";
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
