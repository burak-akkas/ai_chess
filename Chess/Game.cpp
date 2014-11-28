#include "Window.h"

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

int check(std::string s);

int main(int argc, char* args[])
{
	Window* window = new Window();

	bool quit = false;

	if (!window->init())
	{
		std::cout << "Failed to initialize!" << std::endl;
	}
	else
	{
		if (!window->loadMedia())
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

				window->render(*chessBoard);

				std::string x, y;
				std::cin >> x >> y;

				chessBoard->movePiece(check(x), check(y));
			}
		}
	}

	// free all resources and close SDL
	window->close();

	return 0;
}

int check(std::string s)
{
	if (s == "A2") return 48;
	if (s == "A3") return 40;
	else return 0; // for now

}