#include "precomp.h"
#include "InputManager.h"

void InputManager::Update()
{
	memcpy(mPrevKeyStates, mKeyStates, 256 * sizeof(bool));  
}

bool InputManager::IsKeyDown(uint const key) const
{
	return mKeyStates[key & 255]; 
}

bool InputManager::IsKeyUp(uint const key) const
{
	return !mKeyStates[key & 255];
}

bool InputManager::IsKeyReleased(uint const key) const
{
	return mPrevKeyStates[key & 255] && !mKeyStates[key & 255];
}
