#pragma once

#include "Cardinals.h"
#include "Scene.h"

class World
{
public:
	inline static Scene const*	currentScene = nullptr;
	inline static int2			currentSceneCoord = { 0 };
	inline static int			currentSceneIdx = 0;

public: 
	Scene*	scenes;
	uint	columns;
	uint	rows;
	uint	sceneCount;

public:
	void InitWorld(); 
	void SwitchScene(cardinals cardinal);  
};

