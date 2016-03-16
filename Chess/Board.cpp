// Board.cpp  
#pragma once
#include "Board.h"

Board::Board()
{
	initBoardRect();
	initDevPositions();

	devel_w = development(1);
	devel_b = development(-1);
	
	matval_w = matValues(1);
	matval_b = matValues(-1);

	center_w = centeral(1);
	center_b = centeral(-1);

	moves_w = new std::vector<Move>();
	moves_b = new std::vector<Move>();

	children_w = new std::vector<Board>();
	children_b = new std::vector<Board>();

	parent = NULL;

	this->setPossibleMoves();
	this->generateChildren();

	value_w = evaluate(1);
	value_b = evaluate(-1);
	
	if (Config::cside == 1)
	{
		value = value_w - value_b;
	}
	else
	{
		value = value_b - value_w;
	}
}

Board::Board(Board par, Move move)
{
	parent = &par;
	
	if (move.piece != 0) // not a castling move
	{
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				if (i == move.to_x && j == move.to_y)
				{
					piecePositions[i][j] = move.piece; // fill in new position

					if (move.piece == 1 && i == 7)
					{
						piecePositions[i][j] = 5;
					}
					else if (move.piece == -1 && i == 0)
					{
						piecePositions[i][j] = -5;
					}
					if (move.piece == 'p')
					{
						if (i == 5) piecePositions[i][j] = 1;
						if (i == 2) piecePositions[i][j] = -1;
					}
				}
				else if (i == move.from_x && j == move.from_y)
				{
					piecePositions[i][j] = 0;
				}
				else
				{
					piecePositions[i][j] = par.piecePositions[i][j];
				}
				if (move.piece == 'p')
				{
					piecePositions[move.from_x][move.to_y] = 0;
				}
			}
		}

		// set development array
		for (int j = 0; j < 8; j++)
		{
			for (int i = 0; i < 2; i++) // white
			{
				if (i == move.from_x && j == move.from_y)
				{
					devArr[i][j] = 1;
				}
				else
				{
					devArr[i][j] = par.devArr[i][j];
				}
			}
			for (int i = 2; i < 4; i++) // black
			{ 
				if (i + 4 == move.from_x && j == move.from_y)
				{
					devArr[i][j] = 1;
				}
				else
				{
					devArr[i][j] = par.devArr[i][j];
				}
			}
		}
	}
	else if (move.piece == 0) // castling move
	{
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				piecePositions[i][j] = par.piecePositions[i][j];
				if (i < 4)
				{
					devArr[i][j] = par.devArr[i][j];
				}
			}
		}
		//white castling on queenside
		if (move.from_x == 0 && move.from_y == 4 && move.to_x == 0 && move.to_y == 2)
		{
			piecePositions[0][4] = 0;
			piecePositions[0][2] = 6;
			piecePositions[0][0] = 0;
			piecePositions[0][3] = 2;
			devArr[0][0] = 1;
			devArr[0][4] = 1;
		}
		// white castling on kingside
		else if (move.from_x == 0 && move.from_y == 4 && move.to_x == 0 && move.to_y == 6)
		{
			piecePositions[0][4] = 0; 
			piecePositions[0][6] = 6; piecePositions[0][7] = 0; 
			piecePositions[0][5] = 2;
			devArr[0][4] = 1; 
			devArr[0][7] = 1;
		}
		//black castling on queenside
		else if (move.from_x == 7 && move.from_y == 4 && move.to_x == 7 && move.to_y == 2)
		{
			piecePositions[7][4] = 0;
			piecePositions[7][2] = -6;
			piecePositions[7][0] = 0;
			piecePositions[7][3] = -2;
			devArr[3][0] = 1;
			devArr[3][4] = 1;
		}
		//black castling on kingside
		else if (move.from_x == 7 && move.from_y == 4 && move.to_x == 7 && move.to_y == 6)
		{
			piecePositions[7][4] = 0;
			piecePositions[7][6] = -6;
			piecePositions[7][7] = 0;
			piecePositions[7][5] = -2;
			devArr[3][4] = 1;
			devArr[3][7] = 1;
		}
	}
	devel_w = development(1);
	devel_b = development(-1);
	matval_w = matValues(1);
	matval_b = matValues(-1);
	center_w = centeral(1);
	center_b = centeral(-1);

	moves_w = new std::vector<Move>();
	moves_b = new std::vector<Move>();
	children_w = new std::vector<Board>();
	children_b = new std::vector<Board>();

	this->setPossibleMoves();

	value_w = evaluate(1);
	value_b = evaluate(-1);

	if (Config::cside == 1)
	{
		value = value_w - value_b;
	}
	else
	{
		value = value_b - value_w;
	}
}

Board::~Board()
{
	
}

int Board::matValues(int side) 
{ 
	int m = 0;

	if (side == 1) //white
	{ 
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				if (piecePositions[i][j] == 1) //pawn
				{ 
					m += 100; 
				} 
				
				else if (piecePositions[i][j] == 2) //rook
				{ 
					m += 500; 
				} 
				else if (piecePositions[i][j] == 3 || piecePositions[i][j] == 4) //knight or bishop
				{ 
					m += 320; 
				} 
				else if (piecePositions[i][j] == 5) //queen
				{ 
					m += 900; 
				} 
				else if (piecePositions[i][j] == 6) //king
				{
					m += 9999; 
				} 
			}
		}
	}
	else if (side == -1) //black
	{ 
		for (int i = 0; i<8; i++)
		{
			for (int j = 0; j<8; j++)
			{
				if (piecePositions[i][j] == -1) //pawn
				{ 
					m += 100; 
				} 
				else if (piecePositions[i][j] == -2) //rook
				{ 
					m += 500; 
				} 
				else if (piecePositions[i][j] == -3 || piecePositions[i][j] == -4) //knight or bishop
				{ 
					m += 320;
				} 
				else if (piecePositions[i][j] == -5) //queen
				{ 
					m += 900; 
				}
				else if (piecePositions[i][j] == -6) //king
				{
					m += 9999; 
				} 
			}
		}
	}

	return m;
}

int Board::development(int side)
{
	int d = 0;

	if (side == 1)
	{
		for (int i = 0; i < 2; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				if (devArr[i][j] == 1)
				{
					d++;
				}
			}
		}
	}
	else if (side == -1)
	{
		for (int i = 2; i < 4; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				if (devArr[i][j] == 1)
				{
					d++;
				}
			}
		}
	}

	return d;
}

int Board::centeral(int side)
{
	int c = 0;
	if (side == 1)
	{
		for (int i = 2; i < 6; i++)
		{
			for (int j = 2; j < 6; j++)
			{
				if (piecePositions[i][j] == 1)
				{
					c += 1;
				}
				else if (piecePositions[i][j] == 2)
				{
					c += 3;
				}
				else if (piecePositions[i][j] == 3 || piecePositions[i][j] == 4)
				{
					c += 2;
				}
				else if (piecePositions[i][j] == 5)
				{
					c += 4;
				}
			}
		}

		for (int i = 3; i < 5; i++)
		{
			for (int j = 3; j < 5; j++)
			{
				if (piecePositions[i][j] == 1)
				{
					c += 1;
				}
				else if (piecePositions[i][j] == 2)
				{
					c += 3;
				}
				else if (piecePositions[i][j] == 3 || piecePositions[i][j] == 4)
				{
					c += 2;
				}
				else if (piecePositions[i][j] == 5)
				{
					c += 4;
				}
			}
		}
	}
	else if (side == -1)
	{
		for (int i = 2; i < 6; i++)
		{
			for (int j = 2; j < 6; j++)
			{
				if (piecePositions[i][j] == -1)
				{
					c += 1;
				}
				else if (piecePositions[i][j] == -2)
				{
					c += 3;
				}
				else if (piecePositions[i][j] == -3 || piecePositions[i][j] == -4)
				{
					c += 2;
				}
				else if (piecePositions[i][j] == -5)
				{
					c += 4;
				}
			}
		}
		
		for (int i = 3; i < 5; i++)
		{
			for (int j = 3; j < 5; j++)
			{
				if (piecePositions[i][j] == -1)
				{
					c += 1;
				}
				else if (piecePositions[i][j] == -2)
				{
					c += 3;
				}
				else if (piecePositions[i][j] == -3 || piecePositions[i][j] == -4)
				{
					c += 2;
				}
				else if (piecePositions[i][j] == -5)
				{
					c += 4;
				}
			}
		}
	}

	return c;
}

int Board::inPassing(int side)
{
	if (parent == NULL)
	{
		return 0;
	}

	int mp = 0;

	if (side == 1) //white
	{ 
		for (int j = 0; j < 8; j++)
		{
			if (piecePositions[4][j] == 1)
			{
				if (j != 0 && piecePositions[4][j - 1] == -1)
				{
					Move *m = new Move(6, j - 1, 4, j - 1, -1);

					for (size_t i = 0; i < parent->moves_b->size(); i++)
					{
						if (parent->moves_b->at(i).equals(*m))
						{
							Board *b = new Board(*parent, parent->moves_b->at(i));
							if (b->equals(*this))
							{
								mp++;
								Move *m2 = new Move(4, j, 5, j - 1, 'p');
								moves_w->push_back(*m2);
							}
						}
					}
				}
				if (j != 7 && piecePositions[4][j + 1] == -1)
				{
					Move *m = new Move(6, j + 1, 4, j + 1, -1);

					for (size_t i = 0; i < parent->moves_b->size(); i++)
					{
						if (parent->moves_b->at(i).equals(*m))
						{
							Board *b = new Board(*parent, parent->moves_b->at(i));

							if (b->equals(*this))
							{
								mp++;
								Move *m2 = new Move(4, j, 5, j + 1, 'p');
								moves_w->push_back(*m2);
							}
						}
					}
				}
			}
		}
	}
	else
	{ 
		for (int j = 0; j < 8; j++)
		{
			if (piecePositions[3][j] == -1)
			{
				if (j != 0 && piecePositions[3][j - 1] == 1)
				{
					Move *m = new Move(1, j - 1, 3, j - 1, 1);

					for (size_t i = 0; i < parent->moves_w->size(); i++)
					{
						if (parent->moves_w->at(i).equals(*m))
						{
							Board *b = new Board(*parent, parent->moves_w->at(i));

							if (b->equals(*this))
							{
								mp++;
								Move *m2 = new Move(3, j, 2, j - 1, 'p');
								moves_b->push_back(*m2);
							}
						}
					}
				}

				if (j != 7 && piecePositions[3][j + 1] == 1)
				{
					Move *m = new Move(1, j + 1, 3, j + 1, 1);

					for (size_t i = 0; i < parent->moves_w->size(); i++)
					{
						if (parent->moves_w->at(i).equals(*m))
						{
							Board *b = new Board(*parent, parent->moves_w->at(i));

							if (b->equals(*this))
							{
								mp++;
								Move *m2 = new Move(3, j, 2, j + 1, 'p');
								moves_b->push_back(*m2);
							}
						}
					}
				}
			}
		}
	}

	return mp;
}

int Board::pawnMoves(int side)
{ 
	int mp = 0;

	if (side == 1)
	{ 
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				if (piecePositions[i][j] == 1)
				{
					if (i != 7 && piecePositions[i + 1][j] == 0) 
					{
						mp++;
						Move *m = new Move(i, j, i + 1, j, 1);
						moves_w->push_back(*m);
					}
					if (i == 1 && piecePositions[i + 1][j] == 0 && piecePositions[i + 2][j] == 0) 
					{
						mp++;
						Move *m = new Move(i, j, i + 2, j, 1);
						moves_w->push_back(*m);
					}
					if (i != 7 && j != 7 && piecePositions[i + 1][j + 1] < 0) 
					{
						mp++;
						Move *m = new Move(i, j, i + 1, j + 1, 1);
						moves_w->push_back(*m);
					}
					if (i != 7 && j != 0 && piecePositions[i + 1][j - 1] < 0) 
					{
						mp++;
						Move *m = new Move(i, j, i + 1, j - 1, 1);
						moves_w->push_back(*m);
					}
				}
			}
		}
	}
	else if (side == -1)
	{
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				if (piecePositions[i][j] == -1)
				{
					if (i != 0 && piecePositions[i - 1][j] == 0) 
					{
						mp++;
						Move *m = new Move(i, j, i - 1, j, -1);
						moves_b->push_back(*m);
					}
					if (i == 6 && piecePositions[i - 1][j] == 0 && piecePositions[i - 2][j] == 0) 
					{
						mp++;
						Move *m = new Move(i, j, i - 2, j, -1);
						moves_b->push_back(*m);
					}
					if (i != 0 && j != 7 && piecePositions[i - 1][j + 1] > 0) 
					{
						mp++;
						Move *m = new Move(i, j, i - 1, j + 1, -1);
						moves_b->push_back(*m);
					}
					if (i != 0 && j != 0 && piecePositions[i - 1][j - 1] > 0) 
					{
						mp++;
						Move *m = new Move(i, j, i - 1, j - 1, -1);
						moves_b->push_back(*m);
					}
				}
			}
		}
	}

	return mp;
}

int Board::knightMoves(int side) //calculate knight mobility & generate knight moves
{
	int mn = 0;
	for (int i = 0; i<8; i++){
		for (int j = 0; j<8; j++){
			if (side == 1 && piecePositions[i][j] == 3){ //white
				if (i<7 && j<6 && piecePositions[i + 1][j + 2] <= 0) { mn++; Move *m = new Move(i, j, i + 1, j + 2, 3); moves_w->push_back(*m); }
				if (i<7 && j>1 && piecePositions[i + 1][j - 2] <= 0) { mn++; Move *m = new Move(i, j, i + 1, j - 2, 3); moves_w->push_back(*m); }
				if (i>0 && j<6 && piecePositions[i - 1][j + 2] <= 0) { mn++; Move *m = new Move(i, j, i - 1, j + 2, 3); moves_w->push_back(*m); }
				if (i>0 && j>1 && piecePositions[i - 1][j - 2] <= 0) { mn++; Move *m = new Move(i, j, i - 1, j - 2, 3); moves_w->push_back(*m); }
				if (i<6 && j<7 && piecePositions[i + 2][j + 1] <= 0) { mn++; Move *m = new Move(i, j, i + 2, j + 1, 3); moves_w->push_back(*m); }
				if (i<6 && j>0 && piecePositions[i + 2][j - 1] <= 0) { mn++; Move *m = new Move(i, j, i + 2, j - 1, 3); moves_w->push_back(*m); }
				if (i>1 && j<7 && piecePositions[i - 2][j + 1] <= 0) { mn++; Move *m = new Move(i, j, i - 2, j + 1, 3); moves_w->push_back(*m); }
				if (i>1 && j>0 && piecePositions[i - 2][j - 1] <= 0) { mn++; Move *m = new Move(i, j, i - 2, j - 1, 3); moves_w->push_back(*m); }
			}
			else if (side == -1 && piecePositions[i][j] == -3){ //black
				if (i<7 && j<6 && piecePositions[i + 1][j + 2] >= 0) { mn++; Move *m = new Move(i, j, i + 1, j + 2, -3); moves_b->push_back(*m); }
				if (i<7 && j>1 && piecePositions[i + 1][j - 2] >= 0) { mn++; Move *m = new Move(i, j, i + 1, j - 2, -3); moves_b->push_back(*m); }
				if (i>0 && j<6 && piecePositions[i - 1][j + 2] >= 0) { mn++; Move *m = new Move(i, j, i - 1, j + 2, -3); moves_b->push_back(*m); }
				if (i>0 && j>1 && piecePositions[i - 1][j - 2] >= 0) { mn++; Move *m = new Move(i, j, i - 1, j - 2, -3); moves_b->push_back(*m); }
				
				if (i<6 && j<7 && piecePositions[i + 2][j + 1] >= 0) { mn++; Move *m = new Move(i, j, i + 2, j + 1, -3); moves_b->push_back(*m); }
				if (i<6 && j>0 && piecePositions[i + 2][j - 1] >= 0) { mn++; Move *m = new Move(i, j, i + 2, j - 1, -3); moves_b->push_back(*m); }
				if (i>1 && j<7 && piecePositions[i - 2][j + 1] >= 0) { mn++; Move *m = new Move(i, j, i - 2, j + 1, -3); moves_b->push_back(*m); }
				if (i>1 && j>0 && piecePositions[i - 2][j - 1] >= 0) { mn++; Move *m = new Move(i, j, i - 2, j - 1, -3); moves_b->push_back(*m); }
			}
		}
	}
	return mn;
}

int Board::bishopMoves(int side) //calculate bishop mobility & generate bishop moves
{ 
	int mb = 0;
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (side == 1 && piecePositions[i][j] == 4)
			{ //white
				int k = std::min(7 - i, 7 - j);

				for (int l = 1; l <= k; l++)
				{
					if (piecePositions[i + l][j + l] == 0) 
					{ 
						mb++; 
						Move *m = new Move(i, j, i + l, j + l, 4);
						moves_w->push_back(*m); 
					}
					else if (piecePositions[i + l][j + l] < 0) 
					{ 
						mb++; 
						Move *m = new Move(i, j, i + l, j + l, 4);
						moves_w->push_back(*m); 
						l = k; 
					}
					else l = k;
				}

				k = std::min(i, j);

				for (int l = 1; l <= k; l++)
				{
					if (piecePositions[i - l][j - l] == 0) 
					{ 
						mb++; 
						Move *m = new Move(i, j, i - l, j - l, 4); 
						moves_w->push_back(*m); 
					}
					else if (piecePositions[i - l][j - l] < 0) 
					{ 
						mb++; 
						Move *m = new Move(i, j, i - l, j - l, 4); 
						moves_w->push_back(*m); 
						l = k; 
					}
					else l = k;
				}

				k = std::min(7 - i, j);

				for (int l = 1; l <= k; l++)
				{
					if (piecePositions[i + l][j - l] == 0) 
					{
						mb++; 
						Move *m = new Move(i, j, i + l, j - l, 4); 
						moves_w->push_back(*m); 
					}
					else if (piecePositions[i + l][j - l] < 0) 
					{
						mb++; 
						Move *m = new Move(i, j, i + l, j - l, 4);
						moves_w->push_back(*m);
						l = k;
					}
					else l = k;
				}

				k = std::min(i, 7 - j);

				for (int l = 1; l <= k; l++)
				{
					if (piecePositions[i - l][j + l] == 0) 
					{ 
						mb++; 
						Move *m = new Move(i, j, i - l, j + l, 4); 
						moves_w->push_back(*m); 
					}
					else if (piecePositions[i - l][j + l]<0) 
					{ 
						mb++; 
						Move *m = new Move(i, j, i - l, j + l, 4);
						moves_w->push_back(*m);
						l = k; 
					}
					else l = k;
				}
			}
			else if (side == -1 && piecePositions[i][j] == -4)
			{ 
				int k = std::min(7 - i, 7 - j);

				for (int l = 1; l <= k; l++)
				{
					if (piecePositions[i + l][j + l] == 0) 
					{
						mb++; 
						Move *m = new Move(i, j, i + l, j + l, -4);
						moves_b->push_back(*m); 
					}
					else if (piecePositions[i + l][j + l]>0) 
					{
						mb++; 
						Move *m = new Move(i, j, i + l, j + l, -4); 
						moves_b->push_back(*m);
						l = k; 
					}
					else l = k;
				}

				k = std::min(i, j);

				for (int l = 1; l <= k; l++)
				{
					if (piecePositions[i - l][j - l] == 0) 
					{
						mb++; 
						Move *m = new Move(i, j, i - l, j - l, -4); 
						moves_b->push_back(*m);
					}
					else if (piecePositions[i - l][j - l] > 0) 
					{ 
						mb++; 
						Move *m = new Move(i, j, i - l, j - l, -4); 
						moves_b->push_back(*m);
						l = k; 
					}
					
					else l = k;
				}

				k = std::min(7 - i, j);

				for (int l = 1; l <= k; l++)
				{
					if (piecePositions[i + l][j - l] == 0) 
					{
						mb++; 
						Move *m = new Move(i, j, i + l, j - l, -4); 
						moves_b->push_back(*m); 
					}
					else if (piecePositions[i + l][j - l] > 0) 
					{
						mb++; 
						Move *m = new Move(i, j, i + l, j - l, -4); 
						moves_b->push_back(*m); 
						l = k; 
					}
					else l = k;
				}

				k = std::min(i, 7 - j);

				for (int l = 1; l <= k; l++)
				{
					if (piecePositions[i - l][j + l] == 0) 
					{
						mb++; 
						Move *m = new Move(i, j, i - l, j + l, -4); 
						moves_b->push_back(*m);
					}
					else if (piecePositions[i - l][j + l] > 0) 
					{
						mb++; 
						Move *m = new Move(i, j, i - l, j + l, -4); 
						moves_b->push_back(*m); 
						l = k;
					}
					else l = k;
				}
			}
		}
	}

	return mb;
}

int Board::rookMoves(int side) //calculate rook mobility & generate rook moves
{ 
	int mr = 0;

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (side == 1 && piecePositions[i][j] == 2)
			{
				int k = 7 - i;

				for (int l = 1; l <= k; l++)
				{
					if (piecePositions[i + l][j] == 0) 
					{ 
						mr++; 
						Move *m = new Move(i, j, i + l, j, 2); 
						moves_w->push_back(*m); 
					}
					else if (piecePositions[i + l][j] < 0) 
					{ 
						mr++; 
						Move *m = new Move(i, j, i + l, j, 2); 
						moves_w->push_back(*m); 
						l = k; 
					}
					else l = k;
				}

				k = i;

				for (int l = 1; l <= k; l++)
				{
					if (piecePositions[i - l][j] == 0) 
					{ 
						mr++; 
						Move *m = new Move(i, j, i - l, j, 2); 
						moves_w->push_back(*m);
					}
					else if (piecePositions[i - l][j] < 0) 
					{ 
						mr++; 
						Move *m = new Move(i, j, i - l, j, 2);
						moves_w->push_back(*m); 
						l = k; 
					}
					else l = k;
				}

				k = 7 - j;

				for (int l = 1; l <= k; l++)
				{
					if (piecePositions[i][j + l] == 0) 
					{ 
						mr++; 
						Move *m = new Move(i, j, i, j + l, 2); 
						moves_w->push_back(*m); 
					}
					else if (piecePositions[i][j + l] < 0) 
					{ 
						mr++; 
						Move *m = new Move(i, j, i, j + l, 2); 
						moves_w->push_back(*m);
						l = k; 
					}
					else l = k;
				}

				k = j;

				for (int l = 1; l <= k; l++)
				{
					if (piecePositions[i][j - l] == 0) 
					{ 
						mr++;
						Move *m = new Move(i, j, i, j - l, 2);
						moves_w->push_back(*m); 
					}
					else if (piecePositions[i][j - l]<0) 
					{ 
						mr++; 
						Move *m = new Move(i, j, i, j - l, 2); 
						moves_w->push_back(*m); 
						l = k; 
					}
					else l = k;
				}
			}
			else if (side == -1 && piecePositions[i][j] == -2)
			{ 
				int k = 7 - i;

				for (int l = 1; l <= k; l++)
				{
					if (piecePositions[i + l][j] == 0) 
					{ 
						mr++; 
						Move *m = new Move(i, j, i + l, j, -2);
						moves_b->push_back(*m); 
					}
					else if (piecePositions[i + l][j]>0) 
					{
						mr++; 
						Move *m = new Move(i, j, i + l, j, -2); 
						moves_b->push_back(*m); 
						l = k; 
					}
					else l = k;
				}

				k = i;

				for (int l = 1; l <= k; l++)
				{
					if (piecePositions[i - l][j] == 0) 
					{ 
						mr++; 
						Move *m = new Move(i, j, i - l, j, -2); 
						moves_b->push_back(*m); 
					}
					else if (piecePositions[i - l][j] > 0) 
					{
						mr++; 
						Move *m = new Move(i, j, i - l, j, -2); 
						moves_b->push_back(*m); 
						l = k; 
					}
					else l = k;
				}

				k = 7 - j;

				for (int l = 1; l <= k; l++)
				{
					if (piecePositions[i][j + l] == 0) 
					{
						mr++; 
						Move *m = new Move(i, j, i, j + l, -2); 
						moves_b->push_back(*m); 
					}
					else if (piecePositions[i][j + l] > 0) 
					{ 
						mr++; 
						Move *m = new Move(i, j, i, j + l, -2);
						moves_b->push_back(*m); 
						l = k; 
					}
					else l = k;
				}

				k = j;

				for (int l = 1; l <= k; l++)
				{
					if (piecePositions[i][j - l] == 0)
					{
						mr++; 
						Move *m = new Move(i, j, i, j - l, -2);
						moves_b->push_back(*m); 
					}
					else if (piecePositions[i][j - l] > 0) 
					{ 
						mr++; 
						Move *m = new Move(i, j, i, j - l, -2); 
						moves_b->push_back(*m); 
						l = k; 
					}
					else l = k;
				}
			}
		}
	}
	return mr;
}

int Board::queenMoves(int side) //calculate queen mobility & generate queen moves
{ 
	int mq = 0;

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (side == 1 && piecePositions[i][j] == 5)
			{ //white
				//rook moves
				int k = 7 - i;

				for (int l = 1; l <= k; l++)
				{
					if (piecePositions[i + l][j] == 0) 
					{
						mq++; 
						Move *m = new Move(i, j, i + l, j, 5); 
						moves_w->push_back(*m);
					}
					else if (piecePositions[i + l][j] < 0) 
					{ 
						mq++; 
						Move *m = new Move(i, j, i + l, j, 5); 
						moves_w->push_back(*m); 
						l = k;
					}
					else l = k;
				}

				k = i;

				for (int l = 1; l <= k; l++)
				{
					if (piecePositions[i - l][j] == 0) 
					{ 
						mq++; 
						Move *m = new Move(i, j, i - l, j, 5);
						moves_w->push_back(*m);
					}
					else if (piecePositions[i - l][j] < 0)
					{ 
						mq++; 
						Move *m = new Move(i, j, i - l, j, 5); 
						moves_w->push_back(*m); 
						l = k; 
					}
					else l = k;
				}

				k = 7 - j;

				for (int l = 1; l <= k; l++)
				{
					if (piecePositions[i][j + l] == 0) 
					{
						mq++;
						Move *m = new Move(i, j, i, j + l, 5); 
						moves_w->push_back(*m); 
					}
					else if (piecePositions[i][j + l] < 0) 
					{ 
						mq++; 
						Move *m = new Move(i, j, i, j + l, 5);
						moves_w->push_back(*m); 
						l = k; 
					}
					else l = k;
				}

				k = j;

				for (int l = 1; l <= k; l++)
				{
					if (piecePositions[i][j - l] == 0) 
					{ 
						mq++; 
						Move *m = new Move(i, j, i, j - l, 5); 
						moves_w->push_back(*m); 
					}
					else if (piecePositions[i][j - l] < 0) 
					{ 
						mq++; 
						Move *m = new Move(i, j, i, j - l, 5); 
						moves_w->push_back(*m); 
						l = k; 
					}
					else l = k;
				}
				//bishop moves
				k = std::min(7 - i, 7 - j);

				for (int l = 1; l <= k; l++)
				{
					if (piecePositions[i + l][j + l] == 0)
					{ 
						mq++; 
						Move *m = new Move(i, j, i + l, j + l, 5);
						moves_w->push_back(*m);
					}
					else if (piecePositions[i + l][j + l] < 0)
					{
						mq++; 
						Move *m = new Move(i, j, i + l, j + l, 5); 
						moves_w->push_back(*m); 
						l = k; 
					}
					else l = k;
				}

				k = std::min(i, j);

				for (int l = 1; l <= k; l++)
				{
					if (piecePositions[i - l][j - l] == 0) 
					{
						mq++; 
						Move *m = new Move(i, j, i - l, j - l, 5); 
						moves_w->push_back(*m); 
					}
					else if (piecePositions[i - l][j - l] < 0) 
					{ 
						mq++; 
						Move *m = new Move(i, j, i - l, j - l, 5);
						moves_w->push_back(*m);
						l = k; 
					}
					else l = k;
				}

				k = std::min(7 - i, j);

				for (int l = 1; l <= k; l++)
				{
					if (piecePositions[i + l][j - l] == 0)
					{
						mq++; 
						Move *m = new Move(i, j, i + l, j - l, 5);
						moves_w->push_back(*m); 
					}
					else if (piecePositions[i + l][j - l] < 0)
					{
						mq++; 
						Move *m = new Move(i, j, i + l, j - l, 5);
						moves_w->push_back(*m); 
						l = k; }
					else l = k;
				}

				k = std::min(i, 7 - j);

				for (int l = 1; l <= k; l++)
				{
					if (piecePositions[i - l][j + l] == 0) 
					{
						mq++; 
						Move *m = new Move(i, j, i - l, j + l, 5); 
						moves_w->push_back(*m);
					}
					else if (piecePositions[i - l][j + l]<0) 
					{
						mq++; 
						Move *m = new Move(i, j, i - l, j + l, 5); 
						moves_w->push_back(*m); 
						l = k; 
					}
					else l = k;
				}
			}
			else if (side == -1 && piecePositions[i][j] == -5)
			{ //black
				//rook moves
				int k = 7 - i;

				for (int l = 1; l <= k; l++)
				{
					if (piecePositions[i + l][j] == 0) 
					{
						mq++;
						Move *m = new Move(i, j, i + l, j, -5); 
						moves_b->push_back(*m); 
					}
					else if (piecePositions[i + l][j]>0) 
					{ 
						mq++; 
						Move *m = new Move(i, j, i + l, j, -5);
						moves_b->push_back(*m); 
						l = k; 
					}
					else l = k;
				}

				k = i;

				for (int l = 1; l <= k; l++)
				{
					if (piecePositions[i - l][j] == 0) 
					{ 
						mq++; 
						Move *m = new Move(i, j, i - l, j, -5); 
						moves_b->push_back(*m); 
					}
					else if (piecePositions[i - l][j] > 0) 
					{ 
						mq++; 
						Move *m = new Move(i, j, i - l, j, -5);
						moves_b->push_back(*m); 
						l = k; 
					}
					else l = k;
				}

				k = 7 - j;

				for (int l = 1; l <= k; l++)
				{
					if (piecePositions[i][j + l] == 0) 
					{
						mq++;
						Move *m = new Move(i, j, i, j + l, -5); 
						moves_b->push_back(*m);
					}
					else if (piecePositions[i][j + l] > 0) 
					{ 
						mq++; 
						Move *m = new Move(i, j, i, j + l, -5);
						moves_b->push_back(*m); 
						l = k;
					}
					else l = k;
				}

				k = j;

				for (int l = 1; l <= k; l++)
				{
					if (piecePositions[i][j - l] == 0) 
					{
						mq++; 
						Move *m = new Move(i, j, i, j - l, -5); 
						moves_b->push_back(*m); 
					}
					else if (piecePositions[i][j - l] > 0) 
					{ 
						mq++; 
						Move *m = new Move(i, j, i, j - l, -5);
						moves_b->push_back(*m); 
						l = k; 
					}
					else l = k;
				}
				//bishop moves
				k = std::min(7 - i, 7 - j);

				for (int l = 1; l <= k; l++)
				{
					if (piecePositions[i + l][j + l] == 0) 
					{
						mq++; 
						Move *m = new Move(i, j, i + l, j + l, -5); 
						moves_b->push_back(*m); 
					}
					else if (piecePositions[i + l][j + l] > 0)
					{
						mq++; 
						Move *m = new Move(i, j, i + l, j + l, -5); 
						moves_b->push_back(*m);
						l = k; 
					}
					else l = k;
				}

				k = std::min(i, j);

				for (int l = 1; l <= k; l++)
				{
					if (piecePositions[i - l][j - l] == 0) 
					{
						mq++; 
						Move *m = new Move(i, j, i - l, j - l, -5);
						moves_b->push_back(*m); 
					}
					else if (piecePositions[i - l][j - l] > 0) 
					{
						mq++; 
						Move *m = new Move(i, j, i - l, j - l, -5); 
						moves_b->push_back(*m); 
						l = k; 
					}
					else l = k;
				}
				k = std::min(7 - i, j);

				for (int l = 1; l <= k; l++)
				{
					if (piecePositions[i + l][j - l] == 0) 
					{
						mq++; 
						Move *m = new Move(i, j, i + l, j - l, -5); 
						moves_b->push_back(*m); 
					}
					else if (piecePositions[i + l][j - l] > 0) 
					{ 
						mq++; 
						Move *m = new Move(i, j, i + l, j - l, -5);
						moves_b->push_back(*m); 
						l = k; 
					}
					else l = k;
				}

				k = std::min(i, 7 - j);

				for (int l = 1; l <= k; l++)
				{
					if (piecePositions[i - l][j + l] == 0) 
					{ 
						mq++; 
						Move *m = new Move(i, j, i - l, j + l, -5); 
						moves_b->push_back(*m);
					}
					else if (piecePositions[i - l][j + l] > 0) 
					{
						mq++;
						Move *m = new Move(i, j, i - l, j + l, -5); 
						moves_b->push_back(*m); 
						l = k;
					}
					else l = k;
				}
			}
		}
	}

	return mq;
}

int Board::kingMoves(int side)
{ //calculate king mobility & generate king moves
	int mk = 0;
	for (int i = 0; i<8; i++)
	{
		for (int j = 0; j<8; j++)
		{
			if (side == 1 && piecePositions[i][j] == 6)
			{ //white
				if (i<7 && piecePositions[i + 1][j] <= 0 && !isUnderAttack(i + 1, j, side))
				{
					mk++; 
					Move *m = new Move(i, j, i + 1, j, 6); 
					moves_w->push_back(*m);
				}
				if (i>0 && piecePositions[i - 1][j] <= 0 && !isUnderAttack(i - 1, j, side))
				{
					mk++; 
					Move *m = new Move(i, j, i - 1, j, 6);
					moves_w->push_back(*m);
				}
				if (j<7 && piecePositions[i][j + 1] <= 0 && !isUnderAttack(i, j + 1, side)) 
				{
					mk++; 
					Move *m = new Move(i, j, i, j + 1, 6); 
					moves_w->push_back(*m);
				}
				if (j>0 && piecePositions[i][j - 1] <= 0 && !isUnderAttack(i, j - 1, side)) 
				{ 
					mk++;
					Move *m = new Move(i, j, i, j - 1, 6);
					moves_w->push_back(*m);
				}
				if (i<7 && j<7 && piecePositions[i + 1][j + 1] <= 0 && !isUnderAttack(i + 1, j + 1, side)) 
				{ 
					mk++; 
					Move *m = new Move(i, j, i + 1, j + 1, 6); 
					moves_w->push_back(*m);
				}
				if (i<7 && j>0 && piecePositions[i + 1][j - 1] <= 0 && !isUnderAttack(i + 1, j - 1, side)) 
				{ 
					mk++; 
					Move *m = new Move(i, j, i + 1, j - 1, 6); 
					moves_w->push_back(*m);
				}
				if (i>0 && j<7 && piecePositions[i - 1][j + 1] <= 0 && !isUnderAttack(i - 1, j + 1, side)) 
				{ 
					mk++;
					Move *m = new Move(i, j, i - 1, j + 1, 6); 
					moves_w->push_back(*m);
				}
				if (i>0 && j>0 && piecePositions[i - 1][j - 1] <= 0 && !isUnderAttack(i - 1, j - 1, side)) 
				{
					mk++; 
					Move *m = new Move(i, j, i - 1, j - 1, 6); 
					moves_w->push_back(*m);
				}
			}
			else if (side == -1 && piecePositions[i][j] == -6)
			{ //black
				if (i<7 && piecePositions[i + 1][j] >= 0 && !isUnderAttack(i + 1, j, side)) 
				{ 
					mk++; 
					Move *m = new Move(i, j, i + 1, j, -6);
					moves_b->push_back(*m);
				}
				if (i>0 && piecePositions[i - 1][j] >= 0 && !isUnderAttack(i - 1, j, side)) 
				{ 
					mk++; 
					Move *m = new Move(i, j, i - 1, j, -6); 
					moves_b->push_back(*m);
				}
				if (j<7 && piecePositions[i][j + 1] >= 0 && !isUnderAttack(i, j + 1, side)) 
				{ 
					mk++;
					Move *m = new Move(i, j, i, j + 1, -6); 
					moves_b->push_back(*m); 
				}
				if (j>0 && piecePositions[i][j - 1] >= 0 && !isUnderAttack(i, j - 1, side)) 
				{
					mk++;
					Move *m = new Move(i, j, i, j - 1, -6); 
					moves_b->push_back(*m);
				}
				if (i<7 && j<7 && piecePositions[i + 1][j + 1] >= 0 && !isUnderAttack(i + 1, j + 1, side)) 
				{ 
					mk++; 
					Move *m = new Move(i, j, i + 1, j + 1, -6); 
					moves_b->push_back(*m);
				}
				if (i<7 && j>0 && piecePositions[i + 1][j - 1] >= 0 && !isUnderAttack(i + 1, j - 1, side)) 
				{
					mk++; 
					Move *m = new Move(i, j, i + 1, j - 1, -6); 
					moves_b->push_back(*m);
				}
				if (i>0 && j<7 && piecePositions[i - 1][j + 1] >= 0 && !isUnderAttack(i - 1, j + 1, side))
				{
					mk++; 
					Move *m = new Move(i, j, i - 1, j + 1, -6);
					moves_b->push_back(*m);
				}
				if (i>0 && j>0 && piecePositions[i - 1][j - 1] >= 0 && !isUnderAttack(i - 1, j - 1, side)) 
				{
					mk++;
					Move *m = new Move(i, j, i - 1, j - 1, -6); 
					moves_b->push_back(*m);
				}
			}
		}
	}
	//castling moves
	if (side == 1)
	{ //white
		if (isCastling(1, 0))
		{ //queenside
			mk++;
			Move *m = new Move(0, 4, 0, 2, 0);
			moves_w->push_back(*m);
		}
		if (isCastling(1, 1))
		{ //kingside
			mk++;
			Move *m = new Move(0, 4, 0, 6, 0);
			moves_w->push_back(*m);
		}
	}
	else if (side == -1)
	{ //black
		if (isCastling(-1, 0))
		{ //queenside
			mk++; 
			Move *m = new Move(7, 4, 7, 2, 0);
			moves_b->push_back(*m);
		}
		if (isCastling(-1, 1))
		{ //kingside
			mk++; 
			Move *m = new Move(7, 4, 7, 6, 0); 
			moves_b->push_back(*m);
		}
	}
	return mk;
}

int Board::possibleMoves(int side)
{ //calculate total mobilities & generate all possible moves of board
	int m = 0;
	if (side == 1)
	{ //white
		moves_w->clear();
		m = pawnMoves(1) + knightMoves(1) + bishopMoves(1) + rookMoves(1) + queenMoves(1) + kingMoves(1);
		mobility_w = m;
	}
	else if (side == -1)
	{ //black
		moves_b->clear();
		m = pawnMoves(-1) + knightMoves(-1) + bishopMoves(-1) + rookMoves(-1) + queenMoves(-1) + kingMoves(-1);
		mobility_b = m;
	}
	return m;
}


void Board::setPossibleMoves()
{ //re-calculate mobilities
	moves_w->clear();
	moves_b->clear();
	mobility_w = pawnMoves(1) + knightMoves(1) + bishopMoves(1) + rookMoves(1) + queenMoves(1) + kingMoves(1);
	mobility_b = pawnMoves(-1) + knightMoves(-1) + bishopMoves(-1) + rookMoves(-1) + queenMoves(-1) + kingMoves(-1);
}

bool Board::isUnderAttack(int x, int y, int side)
{ //cell(x,y) is in danger by opponent side pieces
	if (side == 1)
	{ //white
		for (size_t i = 0; i < moves_b->size(); i++)
		{ // all threats other than pawn
			if (x == moves_b->at(i).to_x && y == moves_b->at(i).to_y && moves_b->at(i).piece != -1)
			{
				return true;
			}
		}
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				if (piecePositions[i][j] == -1)
				{ //pawn threat check
					if (i != 0 && j != 0 && x == i - 1 && y == j - 1) return true;
					if (i != 0 && j != 7 && x == i - 1 && y == j + 1) return true;
				}
				else if (piecePositions[i][j] == -6)
				{ //king threat check
					if (i != 0 && j != 0 && x == i - 1 && y == j - 1) return true;
					if (i != 0 && x == i - 1 && y == j) return true;
					if (i != 0 && j != 7 && x == i - 1 && y == j + 1) return true;
					if (j != 0 && x == i && y == j - 1) return true;
					if (j != 7 && x == i && y == j + 1) return true;
					if (i != 7 && j != 0 && x == i + 1 && y == j - 1) return true;
					if (i != 7 && x == i + 1 && y == j) return true;
					if (i != 7 && j != 7 && x == i + 1 && y == j + 1) return true;
				}
			}
		}
	}
	else if (side == -1)
	{ //black
		for (size_t i = 0; i < moves_w->size(); i++)
		{ //all threats
			if (x == moves_w->at(i).to_x && y == moves_w->at(i).to_y && moves_w->at(i).piece != 1) return true;
		}
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				if (piecePositions[i][j] == 1)
				{ //pawn threats
					if (i != 7 && j != 0 && x == i + 1 && y == j - 1) return true;
					if (i != 7 && j != 7 && x == i + 1 && y == j + 1) return true;
				}
				else if (piecePositions[i][j] == 6)
				{ // king threats
					if (i != 0 && j != 0 && x == i - 1 && y == j - 1) return true;
					if (i != 0 && x == i - 1 && y == j) return true;
					if (i != 0 && j != 7 && x == i - 1 && y == j + 1) return true;
					if (j != 0 && x == i && y == j - 1) return true;
					if (j != 7 && x == i && y == j + 1) return true;
					if (i != 7 && j != 0 && x == i + 1 && y == j - 1) return true;
					if (i != 7 && x == i + 1 && y == j) return true;
					if (i != 7 && j != 7 && x == i + 1 && y == j + 1) return true;
				}
				
			}
		}
	}
	return false;
}

bool Board::isCastling(int side, int type)
{ //given board is ok for castling
	//type=0: castling on queenside
	//type=1: castling on kingside
	if (side == 1)
	{ //white
		if (type == 0 && piecePositions[0][0] == 2 && piecePositions[0][4] == 6 && piecePositions[0][1] == 0 && piecePositions[0][2] == 0 && piecePositions[0][3] == 0 //no pieces between
			&& devArr[0][0] == 0 && devArr[0][4] == 0)
		{ //not moved before
			for (int i = 2; i <= 4; i++)
			{
				if (isUnderAttack(0, i, 1)) return false; //king safe
			}
			return true;
		}
		else if (type == 1 && piecePositions[0][7] == 2 && piecePositions[0][4] == 6 && piecePositions[0][5] == 0 && piecePositions[0][6] == 0
			&& devArr[0][7] == 0 && devArr[0][4] == 0)
		{
			for (int i = 4; i <= 6; i++)
			{
				if (isUnderAttack(0, i, 1)) return false;
			}
			return true;
		}
	}
	else if (side == -1)
	{ //black
		if (type == 0 && piecePositions[7][0] == -2 && piecePositions[7][4] == -6 && piecePositions[7][1] == 0 && piecePositions[7][2] == 0 && piecePositions[7][3] == 0
			&& devArr[3][0] == 0 && devArr[3][4] == 0)
		{
			for (int i = 2; i <= 4; i++)
			{
				if (isUnderAttack(7, i, -1)) return false;
			}
			return true;
		}
		else if (type == 1 && piecePositions[7][7] == -2 && piecePositions[7][4] == -6 && piecePositions[7][5] == 0 && piecePositions[7][6] == 0
			&& devArr[3][7] == 0 && devArr[3][4] == 0)
		{
			for (int i = 4; i <= 6; i++)
			{
				if (isUnderAttack(7, i, -1)) return false;
			}
			return true;
		}
	}
	return false;
}

void Board::generateChildren()
{ //generate all possible child nodes(boards)

	children_w->clear();
	children_b->clear();

	for (size_t i = 0; i < moves_w->size(); i++)
	{
		Board *b = new Board(*this, moves_w->at(i));

		if (b->isInCheck(1))
		{
			moves_w->erase(moves_w->begin() + i);
			i--;
		}
		else
			children_w->push_back(*b);
	}
	for (size_t i = 0; i < moves_b->size(); i++)
	{
		Board *b = new Board(*this, moves_b->at(i));

		if (b->isInCheck(-1))
		{
			moves_b->erase(moves_b->begin() + i); 
			i--;
		}
		else
		{
			children_b->push_back(*b);
		}
			
	}

	mobility_w = children_w->size();
	mobility_b = children_b->size();
	value_w = evaluate(1);
	value_b = evaluate(-1);

	if (Config::cside == 1)
	{
		value = value_w - value_b;
	}
	else
	{
		value = value_b - value_w;
	}
}

bool Board::moveIsValid(Move m)
{ //given move is possible on this board
	if (m.piece <= 0)
	{ // black's move
		for (size_t i = 0; i < moves_b->size(); i++)
		{
			if (moves_b->at(i).equals(m)) return true;
		}
	}
	if (m.piece >= 0)
	{ // white's move
		for (size_t i = 0; i < moves_w->size(); i++)
		{
			if (moves_w->at(i).equals(m)) return true;
		}
	}

	return false;
}

double Board::evaluate(int side)
{ //evaluation function
	double val = 0;
	if (side == 1)
	{ //white
		val = matval_w * Config::Wmat / 40 + mobility_w*Config::Wmob / 50 + devel_w*Config::Wdev / 16 + center_w*Config::Wcen / 20;
	}
	else
	{ //black
		val = matval_b * Config::Wmat / 40 + mobility_b*Config::Wmob / 50 + devel_b*Config::Wdev / 16 + center_b*Config::Wcen / 20;
	}

	return val;
}

bool Board::equals(Board b)
{ //check if two board configurations are the same
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (this->piecePositions[i][j] != b.piecePositions[i][j]) return false;
		}
	}
	return true;
	
}

void Board::copy(Board b)
{ //copy board matrix and development values
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			this->piecePositions[i][j] = b.piecePositions[i][j];
			if (i < 4) this->devArr[i][j] = b.devArr[i][j];
		}
	}
	this->parent = b.parent;
}

void Board::loadBoard()
{ //load board values after copy
	devel_w = development(1);
	devel_b = development(-1);
	matval_w = matValues(1);
	matval_b = matValues(-1);
	center_w = centeral(1);
	center_b = centeral(-1);
	this->setPossibleMoves();
	this->generateChildren();
}

bool Board::isInCheck(int side)
{ //control if the board is in check for one side
	if (side == 1)
	{ //white
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				if (piecePositions[i][j] == 6)
				{
					if (isUnderAttack(i, j, side)) return true;
				}
			}
		}
	}
	else if (side == -1)
	{ //black
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				if (piecePositions[i][j] == -6)
				{
					if (isUnderAttack(i, j, side)) return true;
				}
			}
		}
	}

	return false;
}

bool Board::isDraw()
{ //check if the current board is a draw condition
	int b;
	int nw = 0, nb = 0;
	int bw = 0, bb = 0;
	int x1 = 0, x2 = 0, y1 = 0, y2 = 0;
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			b = piecePositions[i][j];
			if (b == 1 || b == -1 || b == 2 || b == -2 || b == 5 || b == -5) return false;
			else if (b == 3) nw++;
			else if (b == 4) { bw++; x1 = i; y1 = j; }
			else if (b == -3) nb++;
			else if (b == -4) { bb++; x2 = i; y2 = j; }
		}
	}

	if (nw == 0 && nb == 0 && bw == 0 && bb == 0) return true; // king vs king
	else if (nw == 0 && nb == 0 && bw == 1 && bb == 0) return true; // king vs king&bishop
	else if (nw == 0 && nb == 0 && bw == 0 && bb == 1) return true;
	else if (nw == 1 && nb == 0 && bw == 0 && bb == 0) return true; // king vs king&knight
	else if (nw == 0 && nb == 1 && bw == 0 && bb == 0) return true;
	else if (nw == 0 && nb == 0 && bw == 1 && bb == 1)
	{ // king&bishop vs king&bishop
		
		int A[8][8];

		// unit matrix 8*8
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				if (i % 2 == 0)
				{
					if (j % 2 == 0)
					{
						A[i][j] = 0;
					}
					else
					{
						A[i][j] = 1;
					}
				}
				else
				{
					if (j % 2 == 0)
					{
						A[i][j] = 1;
					}
					else
					{
						A[i][j] = 0;
					}
				}
			}
		}

		if (A[x1][y1] == A[x2][y2]) return true; //both bishops on diagonals of the same colour
	}

	return false;
}

bool Board::isCheckMate(int side)
{ //control if the Config ends
	if (isInCheck(side) && side == 1)
	{ //white is in check
		for (size_t i = 0; i < children_w->size(); i++)
		{
			if (!children_w->at(i).isInCheck(side)) return false;
		}

		return true;
	}
	else if (isInCheck(side) && side == -1)
	{ //black is in check
		for (size_t i = 0; i < children_b->size(); i++)
		{
			if (!children_b->at(i).isInCheck(side)) return false;
		}

		return true;
	}

	return false;
}

struct Board::MiniValues
{
	double val;
	double index;
};

Board Board::Play()
{
	Board *best_move = new Board();
	Board::MiniValues max;
	int index = 0;

	max = Minimax_AB(Config::cside, Config::cside, *this, Config::NEG, Config::POS);

	index = (int)max.index;

	if (Config::cside == 1)
	{
		best_move->copy(children_w->at(index));
		best_move->loadBoard();
	}
	else
	{
		best_move->copy(children_b->at(index));
		best_move->loadBoard();
	}
	return *best_move;
}




Board::MiniValues Board::Minimax_AB(int side, int turn, Board current, double alpha, double beta)
{
	double val = 0;
	double score = 0;
	double index = 0;
	int maxDepth;
	//double *arr = new double();
	Board::MiniValues arr;
	maxDepth = Config::maxDepth / 2;

	if (Config::depth == maxDepth)
	{
		val = current.value;
		arr.val = val;
		arr.index = index;
		return arr;
	}
	else
	{
		current.generateChildren();
		Config::depth++;
		if (side == 1)
		{//computer white and we will maximize children_w
			if (turn == side)
			{ //max node
				for (size_t i = 0; i < current.children_w->size(); i++)
				{
					arr = Minimax_AB(side, (turn*(-1)), current.children_w->at(i), alpha, beta); //call for every child, computer white,white's turn
					score = arr.val;
					if (score>alpha)
					{
						alpha = score;
						index = i;
					}
					if (alpha >= beta)
					{
						val = alpha;
						Config::depth--;
						arr.val = val;
						arr.index = index;
						return arr;
					}
				}

				val = alpha;
				Config::depth--;
				arr.val = val;
				arr.index = index;
				return arr;
			}
			else
			{// for every child that is black's turn (min node)
				for (size_t i = 0; i < current.children_b->size(); i++)
				{
					arr = Minimax_AB(side, (turn*(-1)), current.children_b->at(i), alpha, beta);
					score = arr.val;
					if (score<beta)
					{
						beta = score;
						index = i;
					}
					if (alpha >= beta)
					{
						val = beta;
						Config::depth--;
						arr.val = val;
						arr.index = index;
						return arr;
					}
				}
				val = beta;
				Config::depth--;
				arr.val = val;
				arr.index = index;
				return arr;
			}
		}
		else
		{//computer black and we will maximize children_b
			if (turn == side)
			{ //max node
				for (size_t i = 0; i < current.children_b->size(); i++)
				{
					arr = Minimax_AB(side, (turn*(-1)), current.children_b->at(i), alpha, beta); //call for every child, computer black,black's turn
					score = arr.val;
					if (score>alpha){
						alpha = score;
						index = i;
					}
					if (alpha >= beta)
					{
						val = alpha;
						Config::depth--;
						arr.val = val;
						arr.index = index;
						return arr;
					}
				}
				val = alpha;
				Config::depth--;
				arr.val = val;
				arr.index = index;
				return arr;
			}
			else
			{// for every child that is black's turn (min node)
				for (size_t i = 0; i < current.children_w->size(); i++)
				{
					arr = Minimax_AB(side, (turn*(-1)), current.children_w->at(i), alpha, beta);
					score = arr.val;

					if (score < beta)
					{
						beta = score;
						index = i;
					}
					if (alpha >= beta)
					{
						val = beta;
						Config::depth--;
						arr.val = val;
						arr.index = index;
						return arr;
					}
				}

				val = beta;
				Config::depth--;
				arr.val = val;
				arr.index = index;
				return arr;
			}
		}
	}
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

void Board::initPiecePositions()
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

void Board::initDevPositions()
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			devArr[i][j] = 0;
		}
	}
}
