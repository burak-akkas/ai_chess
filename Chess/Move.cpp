#include "Move.h"

Move::Move()
{
	from_x = 0;
	from_y = 0;
	to_x = 0;
	to_y = 0;
	piece = 0;
}

Move::Move(int x, int y, int x2, int y2, int p)
{
	from_x = x;
	from_y = y;
	to_x = x2;
	to_y = y2;
	piece = p;
}

Move::~Move()
{

}

bool Move::equals(Move m)
{
	if (from_x == m.from_x && from_y == m.from_y && to_x == m.to_x && to_y == m.to_y && piece == m.piece)
	{
		return true;
	}
		
	return false;
}

void Move::copy(Move m)
{
	from_x = m.from_x;
	from_y = m.from_y;
	to_x = m.to_x;
	to_y = m.to_y;
	piece = m.piece;
}

