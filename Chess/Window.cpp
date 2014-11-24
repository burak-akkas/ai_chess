#include "Board.h"
#include <iostream>
#include <iomanip>

enum Pieces
{
	KING,    // 0
	QUEEN,   // 1 
	ROOK,    // 2
	BISHOP,  // 3
	KNIGHT,  // 4
	PAWN     // 5
};

enum Positions
{
	A8, B8, C8, D8, E8, F8, G8, H8, // 0-7
	A7, B7, C7, D7, E7, F7, G7, H7, // 8-15
	A6, B6, C6, D6, E6, F6, G6, H6, // 16-23
	A5, B5, C5, D5, E5, F5, G5, H5, // 24-31
	A4, B4, C4, D4, E4, F4, G4, H4, // 32-39
	A3, B3, C3, D3, E3, F3, G3, H3, // 40-47
	A2, B2, C2, D2, E2, F2, G2, H2, // 48-55
	A1, B1, C1, D1, E1, F1, G1, H1  // 56-63
};

// screen dimension constants
const int SCREEN_WIDTH = 600;
const int SCREEN_HEIGHT = 600;

bool init();
bool loadMedia();
void close();
SDL_Texture* loadTexture(std::string path);
void render(Board b);

int check(std::string s);

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

SDL_Texture* board = NULL;
SDL_Texture* black_pieces[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
SDL_Texture* white_pieces[6] = {NULL, NULL, NULL, NULL, NULL, NULL};

int main(int argc, char* args[])
{
	bool quit = false;

	if (!init())
	{
		std::cout << "Failed to initialize!" << std::endl;
	}
	else
	{
		if (!loadMedia())
		{
			std::cout << "Failed to load media!" << std::endl;
		}
		else
		{
			SDL_Event e;

			Board* chessBoard = new Board();

			while (!quit)
			{	
				while (SDL_PollEvent(&e) != 0)
				{
					if (e.type == SDL_QUIT)
					{
						quit = true;
					}
					if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)
					{
						quit = true;
					}
				}

				render(*chessBoard);

				std::string x, y;
				std::cin >> x >> y;

				chessBoard->movePiece(check(x), check(y));
			}
		}
	}

	// free all resources and close SDL
	close();

	return 0;
}

bool init()
{
	// initialization flag
	bool success = true;

	// initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "SDL failed to initialize! SDL Error: " << SDL_GetError() << std::endl;

		success = false;
	}
	else
	{
		// create window
		window = SDL_CreateWindow("Chess", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

		if (window == NULL)
		{
			std::cout << "Window could not be created! SDL Error: " << SDL_GetError() << std::endl;
			
			success = false;
		}
		else
		{
			renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

			if (renderer == NULL)
			{
				std::cout << "Renderer could not be created! SDL Error: " << SDL_GetError() << std::endl;
				
				success = false;
			}
			else
			{
				SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

				int imgFlag = IMG_INIT_PNG;

				if (!(IMG_Init(imgFlag) & imgFlag))
				{
					std::cout << "SDL_Image could not be initialized! SDL Image Error: " << IMG_GetError() << std::endl;

					success = false;
				}
			}
		}
	}

	return success;
}

void close()
{
	// destroy board
	SDL_DestroyTexture(board);
	board = NULL;

	// destroy pieces
	for (int i = 0; i < 6; i++)
	{
		SDL_DestroyTexture(black_pieces[i]);
		black_pieces[i] = NULL;

		SDL_DestroyTexture(white_pieces[i]);
		white_pieces[i] = NULL;
	}

	// destroy renderer
	SDL_DestroyRenderer(renderer);
	renderer = NULL;

	// destroy window
	SDL_DestroyWindow(window);
	window = NULL;

	// quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

bool loadMedia()
{
	bool success = true;
	std::string path = "C:/Users/Burak/Desktop/Chess_Devel/Graphics/";

	// load board texture
	board = loadTexture(path + "Board.png");

	if (board == NULL)
	{
		std::cout << "Failed to load media!" << std::endl;
		success = false;
	}

	// load white pieces
	for (int i = 0; i < 6; i++)
	{
		white_pieces[i] = loadTexture(path + "white_" + std::to_string(i) + ".png");

		if (white_pieces[i] == NULL)
		{
			std::cout << "Failed to load media!" << std::endl;
		}
	}

	// load black pieces
	for (int i = 0; i < 6; i++)
	{
		black_pieces[i] = loadTexture(path + "black_" + std::to_string(i) + ".png");

		if (black_pieces[i] == NULL)
		{
			std::cout << "Failed to load media!" << std::endl;
		}
	}

	return success;
}

SDL_Texture* loadTexture(std::string path)
{
	// new texture
	SDL_Texture* newTexture = NULL;

	// load png image to surface
	SDL_Surface* tempSurface = IMG_Load(path.c_str());

	if (tempSurface == NULL)
	{
		std::cout << "Could not load file " << path.c_str() << " SDL_image Error: " << IMG_GetError() << std::endl;
	}
	else
	{
		newTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);

		if (newTexture == NULL)
		{
			std::cout << "Could not create texture from surface! SDL Error: " << SDL_GetError() << std::endl;
		}
		
		SDL_FreeSurface(tempSurface);
	}

	return newTexture;
}

void render(Board b)
{
	/* CLEAR */
	SDL_RenderClear(renderer);
	
	/* DRAW */
	// BOARD
	SDL_RenderCopy(renderer, board, NULL, NULL);

	int temp = -1;

	// PIECES
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			temp = b.piecePositions[i][j];

			if (temp != -1)
			{
				switch (temp)
				{
					case 0:
						SDL_RenderCopy(renderer, black_pieces[PAWN], NULL, &(b.boardRectangle[i][j]));
						break;
					case 1:
						SDL_RenderCopy(renderer, black_pieces[ROOK], NULL, &(b.boardRectangle[i][j]));
						break;
					case 2:
						SDL_RenderCopy(renderer, black_pieces[KNIGHT], NULL, &(b.boardRectangle[i][j]));
						break;
					case 3:
						SDL_RenderCopy(renderer, black_pieces[BISHOP], NULL, &(b.boardRectangle[i][j]));
						break;
					case 4:
						SDL_RenderCopy(renderer, black_pieces[QUEEN], NULL, &(b.boardRectangle[i][j]));
						break;
					case 5:
						SDL_RenderCopy(renderer, black_pieces[KING], NULL, &(b.boardRectangle[i][j]));
						break;
					case 6:
						SDL_RenderCopy(renderer, white_pieces[PAWN], NULL, &(b.boardRectangle[i][j]));
						break;
					case 7:
						SDL_RenderCopy(renderer, white_pieces[ROOK], NULL, &(b.boardRectangle[i][j]));
						break;
					case 8:
						SDL_RenderCopy(renderer, white_pieces[KNIGHT], NULL, &(b.boardRectangle[i][j]));
						break;
					case 9:
						SDL_RenderCopy(renderer, white_pieces[BISHOP], NULL, &(b.boardRectangle[i][j]));
						break;
					case 10:
						SDL_RenderCopy(renderer, white_pieces[QUEEN], NULL, &(b.boardRectangle[i][j]));
						break;
					case 11:
						SDL_RenderCopy(renderer, white_pieces[KING], NULL, &(b.boardRectangle[i][j]));
						break;
					default:
						break;
				}
			}
		}
	}

	SDL_RenderPresent(renderer);
}

int check(std::string s)
{
	if (s == "A2") return 48;
	if (s == "A3") return 40;
	else return 0; // for now

}