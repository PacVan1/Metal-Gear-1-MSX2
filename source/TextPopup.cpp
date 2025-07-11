#include "precomp.h"
#include "TextPopup.h"

#include "Game.h" 

TextPopup::TextPopup() :
	writeTime(0.1f),
	writing(false)
{}

void TextPopup::Update()
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
}

void TextPopup::Render(Surface8* screen) const
{
	screen->Box(mPosition.x, mPosition.y, mPosition.x + 100, mPosition.y + 20, 181);
	screen->Bar(mPosition.x + 1, mPosition.y + 1, mPosition.x + 99, mPosition.y + 19, 0);

	for (int i = 0; i < curLen; i++) 
	{
		screen->Print(str[i], mPosition.x + 5 + FONT_SIZE * i, mPosition.y + 5, 181);
	}
}

void TextPopup::SetString(char const* string)
{
	writing = true; 
	curLen	= 0;
	strLen	= strlen(string);
	strcpy(str, string);
}
