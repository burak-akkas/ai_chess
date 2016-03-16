// Board.h  
#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <vector>
#include <algorithm>
#include "Move.h"
#include "Config.h"

class Board
{
	public:
		double value;
		double value_w;
		double value_b;
		int matval_w;
		int matval_b;
		int mobility_w;
		int mobility_b;
		int devel_w;
		int devel_b;
		int center_w;
		int center_b;
		int side; // 1: white, -1: black
		double alpha;
		double beta;

		int devArr[4][8]; // development array 0: not moved 1: moved
		int piecePositions[8][8];
		SDL_Rect boardRectangle[8][8];

		std::vector<Move> *moves_w; // all possible moves of white
		std::vector<Move> *moves_b;
		std::vector<Board> *children_w; // children boards of white
		std::vector<Board> *children_b;

		struct MiniValues;

		Board *parent;

		Board();
		Board(Board par, Move move); // create new board = old Board + last Move
		~Board();
		int matValues(int side); // calculate material values of board
		int development(int side); // calculate development values of board
		int centeral(int side); // calculate centeral values of board
		int inPassing(int side); // calculate number of possible in passing moves and generate these moves
		int pawnMoves(int side); // calculate pawn mobility and generate pawn moves
		int knightMoves(int side); // calculate knight mobility and generate knight moves
		int bishopMoves(int side); // calculate bishop mobility and generate bishop moves
		int rookMoves(int side); // calculate rook mobility and generate bishop moves
		int queenMoves(int side); // calculate queen mobility and generate bishop moves
		int kingMoves(int side); //  calculate king mobility and generate bishop moves
		int possibleMoves(int side); // calculate total mobilities and generate all possible moves of board
		void setPossibleMoves(); // re-calculate mobilities
		bool isUnderAttack(int x, int y, int side); // cell(x,y) is in danger by opponent side pieces
		bool isCastling(int side, int type); // given board is ok for castling
		void generateChildren(); // generate all possible child nodes
		bool moveIsValid(Move m); // given move is possible on this board
		double evaluate(int side); // evaluation function
		bool equals(Board b); // check if two boards are the same
		void copy(Board b); // copy board matrix and development values
		void loadBoard(); // load board values after copy
		bool isInCheck(int side); // control if the board is in check for one side
		bool isDraw(); // check if the current board is a draw
		bool isCheckMate(int side); // check if game ends

		Board Play();  // best move ..
		MiniValues Minimax_AB(int side, int turn, Board current, double alpha, double beta); // minimax_alpha beta pruning

		void initBoardRect(); // init/rendering rect. board
		void initPiecePositions(); // init piece positions
		void initDevPositions(); // init development array positions

	private:
		
};