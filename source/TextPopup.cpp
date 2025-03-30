#include "precomp.h"
#include "TextPopup.h"

#include "Game.h" 

TextPopup::TextPopup() :
	writeTime(0.1f),
	writing(false)
{}

void TextPopup::Update(float const dt)
{
	if (curLen < strLen)
	{
		if (writeTimer.elapsed() >= writeTime)
		{
			curLen++;
			writeTimer.reset();
		}
		return;
	}
	writing = false;
	Game::textPopupTimer.reset(); 
}

void TextPopup::Render(Surface8* screen) const
{
	screen->Box(position.x, position.y, position.x + 100, position.y + 20, 181);
	screen->Bar(position.x + 1, position.y + 1, position.x + 99, position.y + 19, 0);

	for (int i = 0; i < curLen; i++) 
	{
		screen->Print(str[i], position.x + 5 + FONT_SIZE * i, position.y + 5, 181); 
	}
}

void TextPopup::SetString(char const* str)
{
	writing = true; 
	curLen	= 0;
	strLen	= strlen(str);
	strcpy(this->str, str);
}
