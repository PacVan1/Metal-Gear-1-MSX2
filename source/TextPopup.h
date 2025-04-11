#pragma once

class TextPopup
{
private:
	int2	mPosition; 
	char	str[16];
	int		strLen;
	int		curLen;
	float	writeTime;
	Timer	writeTimer;
	bool	writing; 

public:
			TextPopup(); 
	void	Update();
	void	Render(Surface8* screen) const;
	void	SetString(char const* str);
	void	SetPosition(int2 const position) { this->mPosition = position; }
	bool	IsWriting() const { return writing; }
};

