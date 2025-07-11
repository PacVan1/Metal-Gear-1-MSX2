// Template, 2024 IGAD Edition
// Get the latest version from: https://github.com/jbikker/tmpl8
// IGAD/NHTV/BUAS/UU - Jacco Bikker - 2006-2024

#pragma once

#include "World.h"
#include "Player.h" 
#include "Passage.h" 
#include "Soldiers.h"
#include "SoldierTypes.h" 
#include "Inventory.h"
#include "Projectile.h"
#include "LandmineObject.h" 
#include "InputManager.h"
#include "TextPopup.h"
#include "InventoryUI.h"
#include "PlayerUI.h"

int constexpr	ENDING_SCENE_INDEX			= -1;

int2 const		INIT_PLAYER_POSITION		= int2(128, 85);
//int2 const		INIT_PLAYER_POSITION		= int2(128, 50);
int constexpr	INIT_SCENE_INDEX			= 16; 

float constexpr INIT_GAME_START_BLINK_TIME	= 0.5f;
bool constexpr	INIT_SHOW_GAME_START		= true;
int constexpr	INIT_SPLASH_LOGO_STEP_COUNT = 10; 
int constexpr	INIT_SPLASH_LOGO_START_Y	= 248; 
int constexpr	INIT_SPLASH_LOGO_TARGET_Y	= 30;
int constexpr	INIT_SPLASH_MOVE_DISTANCE	= (INIT_SPLASH_LOGO_START_Y - INIT_SPLASH_LOGO_TARGET_Y) / INIT_SPLASH_LOGO_STEP_COUNT;

namespace Audio
{
	class Sound; 
}

enum gameStates : uint8_t
{
	GAME_STATES_OPENING,
	GAME_STATES_TEXT_POPUP,
	GAME_STATES_PLAYING,
	GAME_STATES_PEAK,		// for peaking into a certain room
	GAME_STATES_INVENTORY,
	GAME_STATES_GAME_OVER,
	GAME_STATES_RESTART,
	GAME_STATES_ENDING
};

enum music : uint8_t 
{
	MUSIC_OPENING, 
	MUSIC_MAIN,
	MUSIC_ALERT,
	MUSIC_DEAD,
	MUSIC_RESTART,
	MUSIC_COUNT
}; 

namespace Tmpl8
{
class Game final : public TheApp
{
// SOUND ///////////////////////////////////////////////////////////////////////////
#if SOUND_ON
public: 
	inline static Audio::Sound* sCurrentMusic = nullptr; 
	inline static Audio::Sound sMusic[MUSIC_COUNT];
#endif
public:
	static void SetMusic(int const idx); 

public:
	inline static InputManager*	sInput; 
	inline static Player		sPlayer; 
	inline static SoldierTypes	sSoldierTypes; 
	inline static Soldiers		sSoldiers;
	inline static Inventory		sInventory;
	inline static TextPopup		sTextPopup;
	inline static Projectiles	sProjectiles;
	inline static auto			sLandmines = ObjectPool<LandmineObject>(5);

private:
	inline static SceneTracker	sSceneTracker;
	inline static SceneTracker	sSceneTrackerPrev;
	inline static World			sWorld;
	inline static Tilemap*		sTilemap; 
	inline static InventoryUI	sInventoryUI	= InventoryUI(&sInventory);
	inline static PlayerUI		sPlayerUI		= PlayerUI(&sPlayer, &sInventory); 
	inline static int			sState; 
	inline static Alarm			sTextPopupAlarm = Alarm(0.5f);

	inline static int			sSplashY		= 248;
	inline static Sprite		sSplashLogo		= Sprite("assets/sprites/splash.sprite");
	inline static ColorPalette8 sSplashPalette	= ColorPalette8("assets/color_palettes/splash.cpalette");
	inline static Alarm			sSplashMoveAlarm;
	inline static int			sSplashMoveDistance = 18;
	inline static int			sSplashTimesMoved = 0;
	inline static Alarm			sGameStartBlinkAlarm = Alarm(INIT_GAME_START_BLINK_TIME); 
	inline static bool			sShowGameStart = true; 

public:
	static void EnterPeakState(cardinals const cardinal); 
	static void EnterGameOverState();
	static void SetTextPopup(int2 const position, char const* str);
	static bool SwitchScene(cardinals const cardinal);
	static bool SwitchScene(int2 const coordinate);   
	static bool SwitchScene(int const idx);

	inline static int2				GetSceneCoord()			{ return sSceneTracker.mCoord; }
	inline static Tilemap&			GetTilemap()			{ return *sTilemap; }
	inline static Scene const&		GetScene()				{ return *sSceneTracker.mScene; }
	inline static Player const&		GetPlayer()				{ return sPlayer; }

private: 
	static void InitPlayer(); 
	static void InitMusic(); 
	static void InitWorld();
	static void InitPassages(); // TODO TEMP

	static void UpdateLandmines(float const dt);
	static void RenderLandmines(Surface8* screen);

	static void SetSceneTrackerPrev(SceneTracker const& sceneTracker);  
	static void SetScene(Scene* scene);  
	static void HandlePlayerLeaveScreen();
	static void PrepareSoldiers(Scene const& scene);

	static void EnterOpeningState(Surface8* screen);
	static void EnterPlayingState(Surface8* screen);
	static void EnterEndingState();

	static void OpeningState(Surface8* screen, float const dt); 
	static void PlayingState(Surface8* screen, float const dt);
	static void PeakState(Surface8* screen, float const dt);  
	static void TextPopupState(Surface8* screen, float const dt);
	static void InventoryState(Surface8* screen, float const dt);
	static void GameOverState(Surface8* screen, float const dt);
	static void RestartState(Surface8* screen, float const dt);
	static void EndingState(Surface8* screen, float const dt);  

public:
	// game flow methods
	void Init() override;
	void Tick(float const dt) override;
	void Shutdown() override;

	void PerformanceReport(float const dt) const; 

	// input handling
	void MouseUp( int ) {}
	void MouseDown( int ) {}
	void MouseMove(int, int) {}
	void MouseWheel( float ) {}
	void KeyUp( int ) {}
	void KeyDown( int ) {} 
};

inline void Game::Shutdown()
{
	delete screen;
	delete screen8;
}

} // namespace Tmpl8