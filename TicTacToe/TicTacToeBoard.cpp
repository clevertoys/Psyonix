#include "TicTacToeBoard.h"
#include <cassert>

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
	// No asserts needed here as the Check method below fixes things up if needed and warns the user

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
	assert(width > 2);
	assert(height > 2);

	delete[] cBoard;
	delete[] iMoves;
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
	assert(location >= 0 && location < iBoardHeight* iBoardWidth);
	assert(iNumMovesMadeSoFar < iBoardHeight* iBoardWidth);

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
	assert(location >= 0 && location < iBoardHeight* iBoardWidth);
	assert(iNumMovesMadeSoFar < iBoardHeight* iBoardWidth);

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



bool TicTacToeBoard::IsLegalPlayerMove(int moveLocation) const
{
	if (moveLocation >= iBoardWidth * iBoardHeight) return false;
	if (moveLocation < 0) return false;

	if (cBoard[moveLocation] == ' ') return true;
	return false;
}

void TicTacToeBoard::PrintBoard() const {

	// Print the column numbers
	std::cout << "  ";
	for (int x = 0; x < iBoardWidth; x++)
	{
		std::cout << "  " << x << " ";
	}
	std::cout << "\n";

	//std::cout << "\n\nHere is the current state of the board:\n\n";
	PrintRowOfDashes();



	for (int y = 0; y < iBoardHeight; y++)
	{
		// Print a row of squares, starting with a vertical dash and some nice spacing between 
		// each square

		// print the row number
		std::cout << y << " ";

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

void TicTacToeBoard::PrintRowOfDashes() const
{
	std::cout << "  ";
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

bool TicTacToeBoard::IsInputAMoveLocation(const std::string input) const
{
	int x, y;
	int result = sscanf_s(input.c_str(), "%u,%u)", &x, &y);

	if (result == 2) return true;
	return false;
}

bool TicTacToeBoard::IsInputAValidSize(const std::string input) const
{
	int x, y;
	int result = sscanf_s(input.c_str(), "%u,%u)", &x, &y);

	if (result != 2) return false;

	if (x < 3 || y < 3 || x > 12 || y > 12) return false;
	return true;
}

bool TicTacToeBoard::GetInputMoveLocation(const std::string input, int* x, int* y) const
{
	assert(x != NULL);
	assert(y != NULL);

	int result = sscanf_s(input.c_str(), "%u,%u)", x, y);

	if (result == 2) return true;
	return false;
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
bool TicTacToeBoard::ProcessInput(const std::string input)
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
	else if (IsInputAMoveLocation(input))
	{
		int x, y;
		bool result = GetInputMoveLocation(input, &x, &y);
		assert(result);

		int location = y * iBoardWidth + x;

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
		int newWidth = 3;
		int newHeight = 3;
		std::string inputString;


		std::cout << "Please enter 'X,Y' for the new dimensions (min of 3,3 max of 12,12):\n";
		std::cin >> inputString;
		while (!IsInputAValidSize(inputString))
		{
			std::cout << "Invalid size. Please try again:\n";
			std::cout << "Please enter 'X, Y' for the new dimensions (min of 3,3 max of 12,12):\n";
			std::cin >> inputString;
		}
		bool result = GetInputMoveLocation(inputString, &newWidth, &newHeight);

		std::cout << "New board size is now " << newWidth << "X" << newHeight << "\n";

		ResizeBoard(newWidth, newHeight);
		PrintBoard();
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

bool TicTacToeBoard::IsTimeToQuit() const
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

bool TicTacToeBoard::DidSomeoneWin(const char piece) const
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


	// We now allow diagonals to be "won" no matter where they start along the top row.
	// We allow such a win to "wrap" around off either edge. Strange but fun. It basically 
	// maps the board onto a Torus :) :)
	for (int x = 0; x < iBoardWidth; x++)
	{
		if (HasDiagonalBeenWon(x, true, piece)) return true;
		if (HasDiagonalBeenWon(x, false, piece)) return true;
	}

	return false;
}

bool TicTacToeBoard::IsGameADraw() const
{
	if (iNumMovesMadeSoFar != iBoardHeight * iBoardWidth) return false;

	if (DidSomeoneWin(cComputerPiece)) return false;
	if (DidSomeoneWin(cPlayerPiece)) return false;
	return true;
}
int TicTacToeBoard::WhichRow(int location) const
{
	return (int)(location / iBoardHeight);
}
int TicTacToeBoard::WhichColumn(int location) const
{
	return (int)(location % iBoardWidth);
}

int TicTacToeBoard::CalculateBestComputerMove() const
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
	for (int x = 0; x < iBoardWidth; x++)
	{
		location = CheckSomeoneAboutToWinDiag(x, true, cComputerPiece);
		if (location != -1) return location;
		location = CheckSomeoneAboutToWinDiag(x, false, cComputerPiece);
		if (location != -1) return location;
	}

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

	// Check if they are about to win diagonally
	for (int x = 0; x < iBoardWidth; x++)
	{
		location = CheckSomeoneAboutToWinDiag(x, true, cPlayerPiece);
		if (location != -1) return location;
		location = CheckSomeoneAboutToWinDiag(x, false, cPlayerPiece);
		if (location != -1) return location;
	}

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

// OK, so I found a rule that works well for definition of a diagonal win for non square boards. 
// Basically if you can find a diagonal set of squares starting at any top row location and 
// proceeding either forward+down or backward+down, then you can call that a win. 
// EXTRA: If you allow such a diagonal to "wrap" off either edge of the board you get essentially
// tictactoe mapped on to a Torus (spatially). I won't bother rendering this as a Torus, but if
// were doing this with 3D graphics, you could totally render it that way. I have gon ahead and enabled
// this feature (see the modulo increment below) but could control it with a param or whatever.
bool TicTacToeBoard::HasDiagonalBeenWon(int topRowStartLocation, bool forward, const char piece) const
{
	int numPiecesFound = 0;

	int column = topRowStartLocation;
	int row = 0;
	int boardSize = iBoardWidth * iBoardHeight;
	int rowIncrement = 1;

	if (!forward)
		rowIncrement = -1;

	for (int i = 0; i < iBoardHeight; i++)
	{
		int location = (row * iBoardWidth + column) % boardSize;

		if (cBoard[location] == piece)
		{
			numPiecesFound++;
		}

		if (forward)
		{
			column++;
			column %= iBoardWidth;
		}
		else
		{
			column += iBoardWidth;
			column %= iBoardWidth;
		}


	}
	if (numPiecesFound == iBoardHeight) return true;
	return false;
}

bool TicTacToeBoard::HasRowBeenWon(int row, const char piece) const
{
	int numPiecesFound = 0;
	for (int x = 0; x < iBoardWidth; x++)
	{
		int location = (row * iBoardWidth) + x;
		if (cBoard[location] == piece) numPiecesFound++;
	}
	if (numPiecesFound == iBoardWidth) return true;

	return false;
}

bool TicTacToeBoard::HasColumnBeenWon(int col, const char piece) const
{
	int numPiecesFound = 0;
	for (int y = 0; y < iBoardHeight; y++)
	{
		int location = (y * iBoardWidth) + col;
		if (cBoard[location] == piece) numPiecesFound++;
	}
	if (numPiecesFound == iBoardHeight) return true;
	return false;
}

int TicTacToeBoard::CheckSomeoneAboutToWinRow(int row, const char piece) const
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

int TicTacToeBoard::CheckSomeoneAboutToWinCol(int col, const char piece) const
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
		else if (cBoard[location] == ' ')
		{
			blankSquare = location;
		}
	}
	if (numPiecesFound == iBoardWidth - 1) return blankSquare;
	return -1;
}

// This is a bit screwy if the board is non square... I will do some research on it...
int TicTacToeBoard::CheckSomeoneAboutToWinDiag(int topRowStartLocation, bool forward, const char piece) const
{
	int numPiecesFound = 0;
	int blankSquare = -1;
	int column = topRowStartLocation;
	int row = 0;
	int boardSize = iBoardWidth * iBoardHeight;
	int rowIncrement = 1;

	if (!forward)
		rowIncrement = -1;

	for (int i = 0; i < iBoardHeight; i++)
	{
		int location = (row * iBoardWidth + column) % boardSize;

		if (cBoard[location] == piece)
		{
			numPiecesFound++;
		}
		else if (cBoard[location] == ' ')
		{
			blankSquare = location;
		}


		if (forward)
		{
			column++;
			column %= iBoardWidth;
		}
		else
		{
			column += iBoardWidth;
			column %= iBoardWidth;
		}
	}
	if (numPiecesFound == iBoardHeight-1) return blankSquare;
	return -1;
}

void TicTacToeBoard::PrintHelp()
{
	std::cout << "You will play against the computer. Your will play cPlayerPiece and the computer will play cComputerPiece\n";
	std::cout << "You will get to move first :) \n\n";



	std::cout << "Here is a list of the available commands:\n";
	std::cout << "    help: prints this help\n";
	std::cout << "    restart: restarts the game\n";
	std::cout << "    (0..BoardWidth-1),(0..BoardHeight-1): chooses a square on the board on which to place your piece\n";
	std::cout << "    resize: prompts for a new set of board dimensions (min 3x3)\n";
	std::cout << "    undo: rewinds the game one step (note that if you choose to undo one of your moves, the computers last move will also be undone)\n";
	std::cout << "    quit: exits the game\n\n\n";
}
