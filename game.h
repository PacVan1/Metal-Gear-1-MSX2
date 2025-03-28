// Template, 2024 IGAD Edition
// Get the latest version from: https://github.com/jbikker/tmpl8
// IGAD/NHTV/BUAS/UU - Jacco Bikker - 2006-2024

#pragma once

#include "World.h"
#include "Player.h" 

namespace Tmpl8
{
class Game : public TheApp
{
public:
	inline static World		world; 
	inline static Player	player; 
	int2		mousePos;

	inline static Audio::Sound* currentTheme = nullptr; 
	inline static Audio::Sound mainTheme{ "assets/audio/02. Theme of Tara.mp3" };
	inline static Audio::Sound alertTheme{ "assets/audio/03. Red Alert.mp3" };

public:
	// game flow methods
	void Init();
	void Tick(float deltaTime);
	void Shutdown() {}
	void PerformanceReport(float const dt) const; 

	static void SetTheme(Audio::Sound* sound); 
	void HandlePlayerLeaveScreen(); 
	// input handling
	void MouseUp( int ) {}
	void MouseDown( int ) {}
	void MouseMove( int x, int y ) { mousePos.x = x, mousePos.y = y; }
	void MouseWheel( float ) {}
	void KeyUp( int ) {}
	void KeyDown( int ) {} 
};
} // namespace Tmpl8