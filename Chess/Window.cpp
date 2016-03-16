// Window.cpp
#pragma once
#include "Window.h"

bool Window::init()
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
		window = SDL_CreateWindow("AI Chess - [F1 to RESTART, F2 to RESIGN, ESC to QUIT]", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

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

void Window::close()
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

bool Window::loadMedia()
{
	bool success = true;
	// test
	//std::string path = "C:/Users/Burak/Desktop/Chess_Devel/Graphics/";
	// release
	std::string path = "Graphics/";

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

SDL_Texture* Window::loadTexture(std::string path)
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

void Window::render(Board *b)
{
	/* CLEAR */
	SDL_RenderClear(renderer);
	
	/* DRAW */
	// BOARD
	SDL_RenderCopy(renderer, board, NULL, NULL);

	int temp = 0;

	// PIECES
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			temp = b->piecePositions[i][j];

			if (temp != 0)
			{
				switch (temp)
				{
					case -1:
						SDL_RenderCopy(renderer, black_pieces[PAWN], NULL, &(b->boardRectangle[i][j]));
						break;
					case -2:
						SDL_RenderCopy(renderer, black_pieces[ROOK], NULL, &(b->boardRectangle[i][j]));
						break;
					case -3:
						SDL_RenderCopy(renderer, black_pieces[KNIGHT], NULL, &(b->boardRectangle[i][j]));
						break;
					case -4:
						SDL_RenderCopy(renderer, black_pieces[BISHOP], NULL, &(b->boardRectangle[i][j]));
						break;
					case -5:
						SDL_RenderCopy(renderer, black_pieces[QUEEN], NULL, &(b->boardRectangle[i][j]));
						break;
					case -6:
						SDL_RenderCopy(renderer, black_pieces[KING], NULL, &(b->boardRectangle[i][j]));
						break;
					case 1:
						SDL_RenderCopy(renderer, white_pieces[PAWN], NULL, &(b->boardRectangle[i][j]));
						break;
					case 2:
						SDL_RenderCopy(renderer, white_pieces[ROOK], NULL, &(b->boardRectangle[i][j]));
						break;
					case 3:
						SDL_RenderCopy(renderer, white_pieces[KNIGHT], NULL, &(b->boardRectangle[i][j]));
						break;
					case 4:
						SDL_RenderCopy(renderer, white_pieces[BISHOP], NULL, &(b->boardRectangle[i][j]));
						break;
					case 5:
						SDL_RenderCopy(renderer, white_pieces[QUEEN], NULL, &(b->boardRectangle[i][j]));
						break;
					case 6:
						SDL_RenderCopy(renderer, white_pieces[KING], NULL, &(b->boardRectangle[i][j]));
						break;
					default:
						break;
				}
			}
		}
	}

	SDL_RenderPresent(renderer);
}

SDL_Renderer* Window::getRenderer()
{
	return this->renderer;
}