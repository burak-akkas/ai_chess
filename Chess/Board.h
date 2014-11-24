#include <SDL.h>
#include <SDL_image.h>
#include <string>

class Board
{
	private:
		void initBoardRect();
		int calcX(int index);
		int calcY(int index);

	public:
		enum Pieces;
		enum Positions;
		static int piecePositions[8][8];
		SDL_Rect boardRectangle[8][8];
		Board();
		void Board::movePiece(int from, int to);
};