// Template, 2024 IGAD Edition
// Get the latest version from: https://github.com/jbikker/tmpl8
// IGAD/NHTV/BUAS/UU - Jacco Bikker - 2006-2024

#pragma once

#include "World.h"
#include "Player.h" 
#include "Passage.h" 

enum gameStates : u8
{
	GAME_STATE_OPENING,
	GAME_STATE_TEXT_POPUP,
	GAME_STATE_PLAYING,
	GAME_STATE_INVENTORY,
	GAME_STATE_GAME_OVER
};

namespace Tmpl8
{
class Game : public TheApp
{
public:
	inline static World		world; 
	inline static Player	player;

	inline static Passage*	passage1;
	inline static Passage*	passage2;

	inline static int		state = GAME_STATE_OPENING; 

	inline static Timer textPopupTimer;
	inline static float textPopupTime = 0.5f; 

	int2		mousePos;

	inline static Audio::Sound* currentTheme = nullptr; 
	inline static Audio::Sound mainTheme{ "assets/audio/02. Theme of Tara.mp3" };
	inline static Audio::Sound alertTheme{ "assets/audio/03. Red Alert.mp3" };

public:
	static void SetTheme(Audio::Sound* sound);
	static void SetTextPopup(int2 const position, char const* str); 

public:
	// game flow methods
	void Init();
	void Tick(float const dt);
	void Shutdown() {}

	void OpeningState(float const dt); 
	void PlayingState(float const dt);
	void TextPopupState(float const dt);  
	void InventoryState(float const dt); 
	void GameOverState(float const dt); 

	void PerformanceReport(float const dt) const; 
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