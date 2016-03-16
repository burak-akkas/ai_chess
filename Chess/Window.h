#pragma once
#include <iostream>
#include "Board.h"

class Board;

enum Pieces
{
	KING,    // 0
	QUEEN,   // 1 
	ROOK,    // 2
	BISHOP,  // 3
	KNIGHT,  // 4
	PAWN     // 5
};

class Window
{
	private:
		// screen dimension constants
		const int SCREEN_WIDTH = 600;
		const int SCREEN_HEIGHT = 600;

		SDL_Window* window = NULL;
		SDL_Renderer* renderer = NULL;
		SDL_Texture* board = NULL;
		SDL_Texture* black_pieces[6];
		SDL_Texture* white_pieces[6];

	public:
		bool init();
		bool loadMedia();
		void close();
		SDL_Texture* loadTexture(std::string path);
		SDL_Renderer* getRenderer();
		void render(Board *b);
};