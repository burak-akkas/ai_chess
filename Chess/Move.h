#pragma once
#include <iostream>

class Move
{
	private:

	public:
		int from_x; 

		int from_y;

		int to_x;

		int to_y;

		int piece;

		Move();

		Move(int x, int y, int x2, int y2, int p);

		~Move();

		bool equals(Move m); 

		void copy(Move m); // copies selected move
		
};