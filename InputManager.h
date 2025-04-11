#pragma once

class InputManager
{
public: 
	bool mKeyStates[256]		= { false }; 
	bool mPrevKeyStates[256]	= { false }; 

public:
	void	Update();
	bool	IsKeyDown(uint const key) const; 
	bool	IsKeyUp(uint const key) const;
	bool	IsKeyReleased(uint const key) const;
};

