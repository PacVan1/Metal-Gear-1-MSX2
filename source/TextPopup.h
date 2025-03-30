#pragma once

class TextPopup
{
private:
	int2	position; 
	char	str[16];
	int		strLen;
	int		curLen;
	float	writeTime;
	Timer	writeTimer;
	bool	writing; 

public:
			TextPopup(); 
	void	Update(float const dt);
	void	Render(Surface8* screen) const;
	void	SetString(char const* str);
	void	SetPosition(int2 position) { this->position = position; }
	bool	IsWriting() const { return writing; }
};

