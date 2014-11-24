#include "Board.h"

enum Board::Pieces
{
	EMPTY = -1,

	BLACK_PAWN,
	BLACK_ROOK,
	BLACK_KNIGHT,
	BLACK_BISHOP,
	BLACK_QUEEN,
	BLACK_KING,

	WHITE_PAWN,
	WHITE_ROOK,
	WHITE_KNIGHT,
	WHITE_BISHOP,
	WHITE_QUEEN,
	WHITE_KING
};

// 0-5 black pieces
// 6-11 white pieces
int Board::piecePositions[8][8] =
{
	{ 1, 2, 3, 4, 5, 3, 2, 1 },
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ -1, -1, -1, -1, -1, -1, -1, -1 },
	{ -1, -1, -1, -1, -1, -1, -1, -1 },
	{ -1, -1, -1, -1, -1, -1, -1, -1 },
	{ -1, -1, -1, 6, -1, -1, -1, -1 },
	{ 6, 6, 6, -1, 6, 6, 6, 6 },
	{ 7, 8, 9, 10, 11, 9, 8, 7 }
};


Board::Board()
{
	initBoardRect();
}

void Board::initBoardRect()
{
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			boardRectangle[i][j].h = 72;
			boardRectangle[i][j].w = 72;
			boardRectangle[i][j].x = (j * 72) + 22;
			boardRectangle[i][j].y = (i * 72);
		}
	}
}

void Board::movePiece(int from, int to)
{
	int x = 0, y = 0, val = 0;
	int x1 = 0, y1 = 0;

	x = calcX(to);
	y = calcY(to);
	val = piecePositions[x][y];

	x1 = calcX(from);
	y1 = calcY(from);
	piecePositions[x][y] = piecePositions[x1][y1];

	piecePositions[x1][y1] = val;
}

int Board::calcY(int index)
{
	return index % 8;
}

int Board::calcX(int index)
{
	int i = 0;

	// 0-7, 8-15, 16-23, 24-31, 32-39, 40-47, 48-55, 56-63
	if (index >= 0 && index <= 7) i = 0;
	if (index >= 8 && index <= 15) i = 1;
	if (index >= 16 && index <= 23) i = 2;
	if (index >= 24 && index <= 31) i = 3;
	if (index >= 32 && index <= 39) i = 4;
	if (index >= 40 && index <= 47) i = 5;
	if (index >= 48 && index <= 55) i = 6;
	if (index >= 56 && index <= 63) i = 7;

	return i;
}