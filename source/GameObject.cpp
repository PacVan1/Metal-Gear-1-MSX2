#include "precomp.h"
#include "GameObject.h"

GameObject::GameObject(int2 size, int2 offset) : 
	bbox(size, offset) 
{
	bbox.Update(60.0f); 
}

void GameObject::SetPosition(float2 const position) 
{
	bbox.Update(position);
}
