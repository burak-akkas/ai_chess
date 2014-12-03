#include <SDL.h>

class Input
{
	private:
		bool dragging = false;
		const Uint8 Button = SDL_BUTTON_LEFT;
		const int dimensions = 72;
		int offsetX = 0;
		int offsetY = 0;
	public:
		bool intersects(int objectX, int objectY, int mouseX, int mouseY);
		bool isDragging();
		Uint8 getButton();
		int getOffSetX();
		int getOffSetY();
		int getDimensions();
		void setDragging(bool b);
		void setOffsetX(int x);
		void setOffsetY(int y);
};