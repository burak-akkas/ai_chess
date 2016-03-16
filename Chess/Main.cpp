#pragma once
#include "Window.h"

void copyPiecePositions(int from[][8], int to[][8]);
void initPiecePositions(int piecePositions[][8]);
void dragDrop(Board b, Sint32 x, Sint32 y, int &x1, int &y1, int &x2, int &y2, int &p, int type); // type 0 : drag, type 1 : drop

/* MESSAGE BOX SETTINGS */
const SDL_MessageBoxButtonData buttons[] = {
	{ /* .flags, .buttonid, .text */        0, 0, "QUIT" },
	{ SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 1, "BLACK" },
	{ SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT, 2, "WHITE" },
};

const SDL_MessageBoxColorScheme colorScheme = {
	{ /* .colors (.r, .g, .b) */
		/* [SDL_MESSAGEBOX_COLOR_BACKGROUND] */
		{ 255, 0, 0 },
		/* [SDL_MESSAGEBOX_COLOR_TEXT] */
		{ 0, 255, 0 },
		/* [SDL_MESSAGEBOX_COLOR_BUTTON_BORDER] */
		{ 255, 255, 0 },
		/* [SDL_MESSAGEBOX_COLOR_BUTTON_BACKGROUND] */
		{ 0, 0, 255 },
		/* [SDL_MESSAGEBOX_COLOR_BUTTON_SELECTED] */
		{ 255, 0, 255 }
	}
};

const SDL_MessageBoxData messageboxdata = {
	SDL_MESSAGEBOX_INFORMATION, /* .flags */
	NULL, /* .window */
	"Choose your side!", /* .title */
	"Please choose your side.", /* .message */
	SDL_arraysize(buttons), /* .numbuttons */
	buttons, /* .buttons */
	&colorScheme /* .colorScheme */
};

// starting program execution
int main(int argc, char* args[])
{
	Window* window = new Window();

	bool dragging = false;
	bool quit = false;

	int from_x = -1, from_y = -1, to_x = -1, to_y = -1, p = -9;

	int board_initializer[8][8];
	initPiecePositions(board_initializer);

	Board *b = new Board();
	copyPiecePositions(board_initializer, b->piecePositions);
	b->loadBoard();

	if (!window->init())
	{
		SDL_ShowSimpleMessageBox(SDL_MessageBoxFlags::SDL_MESSAGEBOX_ERROR, "Failed to initialize!", "Failed to initialize!", NULL);
	}
	else
	{
		if (!window->loadMedia())
		{
			SDL_ShowSimpleMessageBox(SDL_MessageBoxFlags::SDL_MESSAGEBOX_ERROR, "Failed to load media!", "Failed to load media!", NULL);
		}
		else
		{
			// game label for restarting
		GAME:

			SDL_Event e;

			Board *render = new Board();
			Board *next = new Board();
			Board *prev = new Board();
			Board *current;
			Move *bestmove = new Move();

			int index = 0;
			int input = 0;
			int buttonid;
			render = b;

			// render initialized board once
			window->render(render);

			copyPiecePositions(board_initializer, prev->piecePositions);
			prev->loadBoard();

			SDL_ShowSimpleMessageBox(SDL_MessageBoxFlags::SDL_MESSAGEBOX_INFORMATION, "Welcome!", "WELCOME TO THE AI CHESS!", NULL);

			// show selection message box
			SDL_ShowMessageBox(&messageboxdata, &buttonid);
			if (buttonid != -1)
			{
				if (buttons[buttonid].text == "WHITE")
				{
					Config::cside = -1;
				}
				else if (buttons[buttonid].text == "BLACK")
				{
					Config::cside = 1;
				}
				else
				{
					window->close();
					return 0;
				}
			}

			if (Config::cside == 1)
			{
				prev->copy(prev->Play());
				prev->loadBoard();
				Config::depth = 0;

				render->copy(*prev);
				render->loadBoard();
			}

			while (!quit)
			{
				while (SDL_PollEvent(&e) != 0)
				{
					// quit
					if (e.type == SDL_QUIT)
					{
						quit = true;
					}
					// quit
					if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)
					{
						quit = true;
					}
					// restart
					if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_F1)
					{
						dragging = false;
						quit = false;

						Config::cside = -1;
						Config::count = 0;
						Config::depth = 0;

						initPiecePositions(board_initializer);

						b = new Board();
						copyPiecePositions(board_initializer, b->piecePositions);

						render->copy(*b);
						render->loadBoard();

						window->render(render);

						goto GAME;
					}
					// resign
					if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_F2)
					{
						SDL_ShowSimpleMessageBox(SDL_MessageBoxFlags::SDL_MESSAGEBOX_INFORMATION, "Player RESIGNED!", "Computer WINS! F1 to restart.", NULL);
					}
					// handle mouse input
					if (!dragging)
					{
						if (e.type == SDL_MOUSEMOTION && e.motion.state & SDL_BUTTON_LMASK)
						{
							from_x = -1;
							from_y = -1;
							p = -9;

							render->loadBoard();
							dragDrop(*render, e.button.x, e.button.y, from_x, from_y, to_x, to_y, p, 0);

							dragging = true;
						}
					}
					if (dragging)
					{
						if (e.type == SDL_MOUSEBUTTONUP && e.button.button & SDL_BUTTON_LEFT)
						{
							render->loadBoard();
							dragDrop(*render, e.button.x, e.button.y, from_x, from_y, to_x, to_y, p, 1);
							Move *plmove;

							// control player plays his side
							if (p * Config::cside < 0)
							{
								plmove = new Move(from_x, from_y, to_x, to_y, p);
							}
							else
							{
								plmove = new Move(-1, -1, -1, -1, -1);
							}

							if (render->moveIsValid(*plmove))
							{
								current = new Board(*prev, *plmove);
								current->generateChildren();

								render->copy(*current);
								render->loadBoard();

								if (current->isInCheck(Config::cside))
								{
									SDL_ShowSimpleMessageBox(SDL_MessageBoxFlags::SDL_MESSAGEBOX_INFORMATION, "CHECK!", "Computer is in CHECK!", NULL);
								}
								if (current->isCheckMate(Config::cside)
									|| (Config::cside == 1 && current->children_w->size() == 0 && current->isInCheck(Config::cside))
									|| (Config::cside == -1 && current->children_b->size() == 0 && current->isInCheck(Config::cside)))
								{
									SDL_ShowSimpleMessageBox(SDL_MessageBoxFlags::SDL_MESSAGEBOX_INFORMATION, "CHECK MATE!", "Computer is in CHECK MATE! YOU WIN! F1 to restart.", NULL);
									break;
								} // C Loses
								else if (current->isDraw() || (Config::cside == 1 && current->children_w->size() == 0 && !current->isInCheck(Config::cside))
									|| (Config::cside == -1 && current->children_b->size() == 0 && !current->isInCheck(Config::cside)))
								{
									SDL_ShowSimpleMessageBox(SDL_MessageBoxFlags::SDL_MESSAGEBOX_INFORMATION, "DRAW!", "It's a DRAW! F1 to restart.", NULL);
								}


								next->copy(current->Play());
								next->loadBoard();
								Config::depth = 0;

								if (Config::cside == 1)
								{
									for (size_t i = 0; i < current->children_w->size(); i++){
										if (current->children_w->at(i).equals(*next)){
											index = i; break;
										}
									}

									bestmove->copy(current->moves_w->at(index));

									if (next->isInCheck(-1))
									{
										SDL_ShowSimpleMessageBox(SDL_MessageBoxFlags::SDL_MESSAGEBOX_INFORMATION, "CHECK!", "Player is in CHECK! F2 to resign.", NULL);
									}
									if (next->isCheckMate(-1) || (current->children_b->size() == 0 && current->isInCheck(-1)))
									{
										SDL_ShowSimpleMessageBox(SDL_MessageBoxFlags::SDL_MESSAGEBOX_INFORMATION, "CHECK MATE!", "Player is in CHECK MATE! You LOSE! F1 to restart.", NULL);

										break;
									} // P loses
									else if (current->isDraw() || current->children_b->size() == 0 && !current->isInCheck(-1))
									{
										SDL_ShowSimpleMessageBox(SDL_MessageBoxFlags::SDL_MESSAGEBOX_INFORMATION, "DRAW!", "It's a DRAW! F1 to restart.", NULL);
										break;
									}

								}
								else if (Config::cside == -1)
								{
									for (size_t i = 0; i < current->children_b->size(); i++)
									{
										if (current->children_b->at(i).equals(*next))
										{
											index = i; break;
										}
									} //find index of best child

									bestmove->copy(current->moves_b->at(index));

									if (next->isInCheck(1))
									{
										SDL_ShowSimpleMessageBox(SDL_MessageBoxFlags::SDL_MESSAGEBOX_INFORMATION, "CHECK!", "Player is in CHECK! F2 to resign.", NULL);
									}
									if (next->isCheckMate(1) || (current->children_w->size() == 0 && current->isInCheck(1)))
									{
										SDL_ShowSimpleMessageBox(SDL_MessageBoxFlags::SDL_MESSAGEBOX_INFORMATION, "CHECK!", "Player is in CHECK MATE! You LOSE! F1 to restart.", NULL);
										break;
									} // P loses

									else if (current->isDraw() || current->children_w->size() == 0 && !current->isInCheck(1)){
										SDL_ShowSimpleMessageBox(SDL_MessageBoxFlags::SDL_MESSAGEBOX_INFORMATION, "DRAW!", "It's a DRAW! F1 to restart.", NULL);
										break;
									}

								}

								render->copy(*next);
								render->loadBoard();

								prev->copy(*next);
								prev->loadBoard();

							}
							else
							{
								to_x = -1;
								to_y = -1;
								p = -9;
							}

							dragging = false;
						}

					}
				}

				window->render(render);
			}
		}
	}

	// free all resources and close SDL
	window->close();

	return 0;
}

void copyPiecePositions(int from[][8], int to[][8])
{
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			to[i][j] = from[i][j];
		}
	}
}

void dragDrop(Board b, Sint32 x, Sint32 y, int &x1, int &y1, int &x2, int &y2, int &p, int type)
{
	//int t = 0;

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (type == 0)
			{
				if (b.piecePositions[i][j] != 0 && &(b.boardRectangle[i][j]) != NULL)
				{
					if (x < b.boardRectangle[i][j].x + b.boardRectangle[i][j].w &&
						x > b.boardRectangle[i][j].x &&
						y < b.boardRectangle[i][j].y + b.boardRectangle[i][j].h &&
						y > b.boardRectangle[i][j].y)
					{
						p = b.piecePositions[i][j];

						x1 = i;
						y1 = j;
					}
				}
			}
			else
			{
				if (b.piecePositions[i][j] == 0 || b.piecePositions[i][j] == 1 || b.piecePositions[i][j] == 2 || b.piecePositions[i][j] == 3 || b.piecePositions[i][j] == 4 || b.piecePositions[i][j] == 5 || b.piecePositions[i][j] == 6 || b.piecePositions[i][j] == -1 || b.piecePositions[i][j] == -2 || b.piecePositions[i][j] == -3 || b.piecePositions[i][j] == -4 || b.piecePositions[i][j] == -5 || b.piecePositions[i][j] == -6)
				{
					if (x < b.boardRectangle[i][j].x + b.boardRectangle[i][j].w &&
						x > b.boardRectangle[i][j].x &&
						y < b.boardRectangle[i][j].y + b.boardRectangle[i][j].h &&
						y > b.boardRectangle[i][j].y)
					{
						//p = b.piecePositions[i][j];

						x2 = i;
						y2 = j;
					}
				}
			}
		}
	}
}

void initPiecePositions(int piecePositions[][8])
{
	piecePositions[0][0] = 2;
	piecePositions[0][1] = 3;
	piecePositions[0][2] = 4;
	piecePositions[0][3] = 5;
	piecePositions[0][4] = 6;
	piecePositions[0][5] = 4;
	piecePositions[0][6] = 3;
	piecePositions[0][7] = 2;

	piecePositions[1][0] = 1;
	piecePositions[1][1] = 1;
	piecePositions[1][2] = 1;
	piecePositions[1][3] = 1;
	piecePositions[1][4] = 1;
	piecePositions[1][5] = 1;
	piecePositions[1][6] = 1;
	piecePositions[1][7] = 1;

	piecePositions[7][0] = -2;
	piecePositions[7][1] = -3;
	piecePositions[7][2] = -4;
	piecePositions[7][3] = -5;
	piecePositions[7][4] = -6;
	piecePositions[7][5] = -4;
	piecePositions[7][6] = -3;
	piecePositions[7][7] = -2;

	piecePositions[6][0] = -1;
	piecePositions[6][1] = -1;
	piecePositions[6][2] = -1;
	piecePositions[6][3] = -1;
	piecePositions[6][4] = -1;
	piecePositions[6][5] = -1;
	piecePositions[6][6] = -1;
	piecePositions[6][7] = -1;


	// empty positions (0)
	for (int i = 2; i < 6; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			piecePositions[i][j] = 0;
		}
	}
}