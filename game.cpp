// Template, 2024 IGAD Edition
// Get the latest version from: https://github.com/jbikker/tmpl8
// IGAD/NHTV/BUAS/UU - Jacco Bikker - 2006-2024

#include "precomp.h"

#include "Game.h"

#include "TilePalette.h"
#include "LandmineObject.h" 

int constexpr CLOSED_DOOR_TILE_IDX	= 183; 
int constexpr OPEN_DOOR_TILE_IDX	= 0; 

PassageProps		doorNorth;
PassageProps		doorSouth;
PassageProps		doorEast; 
PassageProps		doorWest; 
PassageProps		jeepNorth;
PassageProps		jeepEast; 

//Soldier testSoldier; 

void Game::Init()
{
#if BAKE_MODE
	Image* image1 = Image::LoadFromFile("assets/images/outer_heaven_tpalette.png");
	SubSurface8::Save(image1, "assets/sprites/outer_heaven.sprite");
	//Image* image3 = Image::LoadFromFile("assets/images/explosion_cpalette.png");
	//ColorPalette8::Save(image3, "assets/color_palettes/explosion.cpalette");
#endif BAKE_MODE 

	sSplashLogo.palette = &sSplashPalette;
	LandmineObject::sSpriteSheetExplosion.palette = &LandmineObject::sColorPaletteExplosion;
	sInput				= &mInput;
	sSceneTracker.mIdx	= 1000; 

	TilePalette::InitPalettes();
	InitMusic();
	InitPlayer();
	InitPassages(); 
	InitWorld();

	sSplashMoveAlarm = Alarm(sMusic[MUSIC_OPENING].getDurationInSeconds() / 15.0f - 0.065f);
	 
	EnterOpeningState(screen8); 
}

void Game::Tick(float const dt)  
{
	switch (sState) 
	{
	case GAME_STATES_OPENING:		OpeningState(screen8, dt);		break;
	case GAME_STATES_PLAYING:		PlayingState(screen8, dt);		break;
	case GAME_STATES_PEAK:			PeakState(screen8, dt);			break; 
	case GAME_STATES_TEXT_POPUP:	TextPopupState(screen8, dt);	break;
	case GAME_STATES_INVENTORY:		InventoryState(screen8, dt);	break;
	case GAME_STATES_GAME_OVER:		GameOverState(screen8, dt);		break;
	case GAME_STATES_RESTART:		RestartState(screen8, dt);		break;
	case GAME_STATES_ENDING:		EndingState(screen8, dt);		break;
	default: break; 
	}

	PerformanceReport(dt);
	sInput->Update();
}

void Game::OpeningState(Surface8* screen, float const dt)
{
	if (sSplashTimesMoved < 10)
	{
		if (sSplashMoveAlarm.Elapsed())
		{
			sSplashTimesMoved++;
			sSplashMoveAlarm.Reset();
			sSplashY -= sSplashMoveDistance;
			sSplashLogo.Render(screen, 41, sSplashY);
		}

		if (sSplashTimesMoved == 10)
		{
			screen->Clear(0);
			sSplashLogo.Render(screen, 41, 30);
			sGameStartBlinkAlarm.Reset(); 
		}
	}
	else
	{
		if (sGameStartBlinkAlarm.Elapsed())
		{
			if (sShowGameStart)
			{
				screen->Print("Push enter key", 86, 150, 181);
			}
			else
			{
				screen->Bar(86, 150, 86 + 84, 150 + 6, 0); 
			}
			sShowGameStart = !sShowGameStart;
			sGameStartBlinkAlarm.Reset(); 
		}

		if (sInput->IsKeyReleased(CONTROLS_START_GAME))	EnterPlayingState(screen);  
	}
}

//void Game::PlayingState(Surface8* screen, float const dt)
//{
//	// update: ----------------------------------
//	if (sInput->IsKeyReleased(CONTROLS_TOGGLE_INVENTORY)) sState = GAME_STATES_INVENTORY; 
//
//	sPlayer.mBbox.mHit = false;
//	sPlayer.mBboxHead.mHit = false;
//	sPlayer.mBboxBody.mHit = false;
//	testSoldier.mBbox.mHit = false;
//	testSoldier.mBboxHead.mHit = false;
//	testSoldier.mBboxBody.mHit = false;
//
//	sSceneTracker.mScene->Update(dt);
//	sPlayer.Update(dt);
//	//sSoldiers.Update(dt);
//	//sProjectiles.Update(dt);
//
//	if (sPlayer.mBbox.Detect(testSoldier.mBbox))
//	{
//		sPlayer.mHit = testSoldier.mHit =
//			Actor::DetectPixel(sPlayer, testSoldier, sPlayer.mBboxHead, testSoldier.mBboxHead) ||
//			Actor::DetectPixel(sPlayer, testSoldier, sPlayer.mBboxHead, testSoldier.mBboxBody) ||
//			Actor::DetectPixel(sPlayer, testSoldier, sPlayer.mBboxBody, testSoldier.mBboxHead) ||
//			Actor::DetectPixel(sPlayer, testSoldier, sPlayer.mBboxBody, testSoldier.mBboxBody);
//	}
//
//	// render game: -----------------------------
//	screen->Clear(100);
//	testSoldier.Render(screen); 
//	sPlayer.Render(screen);
//
//	if (sPlayer.mHit)
//	{
//		sPlayer.mBbox.Render(screen, BBOX_MISS_COLOR, BBOX_PIXEL_PERFECT_COLOR);
//		sPlayer.mBboxHead.Render(screen, BBOX_MISS_COLOR, BBOX_PIXEL_PERFECT_COLOR); 
//		sPlayer.mBboxBody.Render(screen, BBOX_MISS_COLOR, BBOX_PIXEL_PERFECT_COLOR); 
//		testSoldier.mBbox.Render(screen, BBOX_MISS_COLOR, BBOX_PIXEL_PERFECT_COLOR);
//		testSoldier.mBboxHead.Render(screen, BBOX_MISS_COLOR, BBOX_PIXEL_PERFECT_COLOR);
//		testSoldier.mBboxBody.Render(screen, BBOX_MISS_COLOR, BBOX_PIXEL_PERFECT_COLOR);
//	}
//	else
//	{
//		sPlayer.mBbox.Render(screen);
//		sPlayer.mBboxHead.Render(screen);
//		sPlayer.mBboxBody.Render(screen);
//		testSoldier.mBbox.Render(screen);
//		testSoldier.mBboxHead.Render(screen);
//		testSoldier.mBboxBody.Render(screen); 
//	}
//
//	//if (sSceneTracker.mScene) sSceneTracker.mScene->Render(screen);
//	//sSoldiers.Render(screen);
//	//sProjectiles.Render(screen);
//
//	// render ui: -------------------------------
//	sPlayerUI.Render(screen); 
//
//	HandlePlayerLeaveScreen();
//}

void Game::PlayingState(Surface8* screen, float const dt)
{
	// update: ----------------------------------
	if (sInput->IsKeyReleased(CONTROLS_TOGGLE_INVENTORY)) sState = GAME_STATES_INVENTORY;

	sSceneTracker.mScene->Update(dt);
	sPlayer.Update(dt);
	sSoldiers.Update(dt);
	sProjectiles.Update(dt);
	UpdateLandmines(dt); 

	// render game: -----------------------------
	if (sSceneTracker.mScene)
	{
		sSceneTracker.mScene->Render(screen);
	}
	RenderLandmines(screen); 
	sSoldiers.Render(screen);
	sPlayer.Render(screen);
	sProjectiles.Render(screen);

	// render ui: -------------------------------
	sPlayerUI.Render(screen);

	HandlePlayerLeaveScreen();
}

void Game::PeakState(Surface8* screen, float const dt)
{
	// update: ----------------------------------
	if (sInput->IsKeyReleased(CONTROLS_TOGGLE_INVENTORY))	sState = GAME_STATES_INVENTORY;

	if (sInput->IsKeyReleased(CONTROLS_EXIT))
	{
		sState = GAME_STATES_PLAYING;
		SwitchScene(sSceneTrackerPrev.mCoord);  
	}

	//sSceneTracker.mScene->Update(dt);
	sSoldiers.Update(dt);
	sProjectiles.Update(dt);

	// render game: -----------------------------
	if (sSceneTracker.mScene) sSceneTracker.mScene->Render(screen);
	sSoldiers.Render(screen);
	sProjectiles.Render(screen);

	// render ui: -------------------------------
	sPlayerUI.Render(screen);
	screen->Bar(32, 10, 224, 16, 0);  
	screen->Print("Push backspace to exit the scope", 32, 10, 181); 
}

void Game::TextPopupState(Surface8* screen, float const dt)
{
	if (sTextPopup.IsWriting())
	{
		sTextPopup.Update();
		sTextPopup.Render(screen);

		if (!sTextPopup.IsWriting())
		{
			sTextPopupAlarm.Reset(); 
		}
	}
	else if (sTextPopupAlarm.Elapsed())
	{
		sState = GAME_STATES_PLAYING;
	}
}

void Game::InventoryState(Surface8* screen, float const dt)
{
	if (sInput->IsKeyReleased(CONTROLS_TOGGLE_INVENTORY)) sState = GAME_STATES_PLAYING;

	sInventoryUI.Update(dt); 

	sInventoryUI.Render(screen);
	sPlayerUI.Render(screen); 
}

void Game::GameOverState(Surface8* screen, float const dt)
{
	// Wait for theme to finish
	if (sCurrentMusic->isEnd())
	{
		screen->Clear(0);
		screen->Print("Game over", 101, 50, 181); 
		screen->Print("Press enter to restart", 62, 97, 181);
		screen->Print("Press backspace to quit", 59, 109, 181);
		SetMusic(MUSIC_RESTART);  
		sState = GAME_STATES_RESTART; 
	}
}

void Game::EnterPlayingState(Surface8* screen)
{
	screen->Clear(0); 
	sPlayer.SetPosition(INIT_PLAYER_POSITION);
	sPlayer.ResetHealth();
	sPlayer.SetState(PLAYER_STATES_IDLE, NORTH);
	sSoldiers.SetAlertLevel(ALERT_LEVELS_OFF);

	SwitchScene(INIT_SCENE_INDEX);

	sState = GAME_STATES_PLAYING;
	SetMusic(MUSIC_MAIN);
}

void Game::RestartState(Surface8* screen, float const dt)
{
	if (sCurrentMusic->isEnd())
	{
		if (sInput->IsKeyReleased(CONTROLS_RESTART_GAME))	EnterPlayingState(screen); 
		if (sInput->IsKeyReleased(CONTROLS_EXIT))			EnterOpeningState(screen);  
 	}
}

void Game::EndingState(Surface8* screen, float const dt)
{
	screen->Clear(0);  
	screen->Print("You made it to the end", 62, 74, 181);
	screen->Print("Push escape to quit application", 35, 103, 181); 
}

void Game::SetMusic(int const idx)
{
#if SOUND_ON
	if (sCurrentMusic) sCurrentMusic->stop();
	sCurrentMusic = &sMusic[idx];
	sCurrentMusic->replay();
#endif
}

void Game::InitPlayer()
{
	sPlayer.mNormalPalette = ColorPalette8("assets/color_palettes/snake.cpalette");
	sPlayer.mDamagedPalette = ColorPalette8("assets/color_palettes/damaged.cpalette");
	AnimationSpriteSheet* spriteSheet = new AnimationSpriteSheet("assets/spritesheets/snake_v2/snake.animsheet");
	sPlayer.SetSpriteSheet(*spriteSheet, 7);
	sPlayer.SetPalette(&sPlayer.mNormalPalette);

	sPlayer.SetPosition(INIT_PLAYER_POSITION); 
	sPlayer.SetInventory(&sInventory);  
}

void Game::InitMusic()
{
#if SOUND_ON
	// load music:
	sMusic[MUSIC_OPENING]	= Audio::Sound("assets/audio/00. Opening.mp3");
	sMusic[MUSIC_MAIN]		= Audio::Sound("assets/audio/02. Theme of Tara.mp3");
	sMusic[MUSIC_ALERT]		= Audio::Sound("assets/audio/03. Red Alert.mp3");
	sMusic[MUSIC_DEAD]		= Audio::Sound("assets/audio/04. Dead.mp3");
	sMusic[MUSIC_RESTART]	= Audio::Sound("assets/audio/05. Restart.mp3");

	// set looping:
	sMusic[MUSIC_MAIN ].setLooping(true);
	sMusic[MUSIC_ALERT].setLooping(true);
#endif
}

void Game::EnterPeakState(cardinals const cardinal)
{
	if (sSoldiers.GetAlertLevel() <= ALERT_LEVELS_SPOTTED)
	{
		if (SwitchScene(cardinal))sState = GAME_STATES_PEAK;
	}
}

void Game::EnterGameOverState()
{
	SetMusic(MUSIC_DEAD); 
	sState = GAME_STATES_GAME_OVER;  
}

void Game::EnterEndingState() 
{
	SetMusic(MUSIC_RESTART);
	sState = GAME_STATES_ENDING;
}

void Game::SetTextPopup(int2 const position, char const* str)
{
	sState = GAME_STATES_TEXT_POPUP;
	sTextPopup.SetPosition(position); 
	sTextPopup.SetString(str);  
}

bool Game::SwitchScene(cardinals const cardinal)
{
	int2 const	coordinate = sSceneTracker.mCoord + cardinalToInt2(cardinal);
	int const	idx = sWorld.GetSceneIdx(coordinate);
	int const	type = idx < sWorld.GetAdjSceneCount() ? SCENE_TYPES_ADJACENT : SCENE_TYPES_EXTRA;

	if (sSceneTracker.mType == SCENE_TYPES_EXTRA || type == SCENE_TYPES_EXTRA ||
		idx < 0 || idx >= sWorld.GetSceneCount() || idx == sSceneTracker.mIdx) return false; 

	SetSceneTrackerPrev(sSceneTracker);  
	sSceneTracker.mIdx		= idx;
	sSceneTracker.mType		= type;
	sSceneTracker.mCoord	= coordinate;

	sSoldiers.mPool.ReturnAll();
	sLandmines.ReturnAll();

	if (sSoldiers.GetAlertLevel() <= ALERT_LEVELS_LOW)
	{
		sSoldiers.SetAlertLevel(ALERT_LEVELS_OFF);
		SetScene(&sWorld.mScenes[sSceneTracker.mIdx]);
	}
	else
	{
		sSceneTracker.mScene = &sWorld.mScenes[sSceneTracker.mIdx];
		sTilemap = sSceneTracker.mScene->mTilemap;

		sTilemap->tilePalette->sheet.palette = sSceneTracker.mScene->mColorPalette;

		for (int i = 0; i < sSceneTracker.mScene->mSoldierCount; i++)
		{
			sSoldiers.mPool[sSoldiers.mPool.WakeObject()].SetPath(sSceneTracker.mScene->mPaths[i]);
		}
	}

	return true; 
}

bool Game::SwitchScene(int2 const coordinate)
{
	int const idx	= sWorld.GetSceneIdx(coordinate);

	if (idx < 0 || idx >= sWorld.GetSceneCount() || idx == sSceneTracker.mIdx) return false;

	SetSceneTrackerPrev(sSceneTracker);  
	sSceneTracker.mIdx		= idx;
	sSceneTracker.mType		= idx < sWorld.GetAdjSceneCount() ? SCENE_TYPES_ADJACENT : SCENE_TYPES_EXTRA;
	sSceneTracker.mCoord	= coordinate;

	sSoldiers.mPool.ReturnAll();
	sLandmines.ReturnAll();

	if (sSoldiers.GetAlertLevel() <= ALERT_LEVELS_LOW)
	{
		sSoldiers.SetAlertLevel(ALERT_LEVELS_OFF);
		SetScene(&sWorld.mScenes[sSceneTracker.mIdx]);
	}
	else
	{
		sSceneTracker.mScene = &sWorld.mScenes[sSceneTracker.mIdx];
		sTilemap = sSceneTracker.mScene->mTilemap;

		sTilemap->tilePalette->sheet.palette = sSceneTracker.mScene->mColorPalette;

		for (int i = 0; i < sSceneTracker.mScene->mSoldierCount; i++)
		{
			sSoldiers.mPool[sSoldiers.mPool.WakeObject()].SetPath(sSceneTracker.mScene->mPaths[i]);
		}
	}

	return true; 
}

bool Game::SwitchScene(int const idx)
{
	// TODO TEMP for ending of project
	if (idx == ENDING_SCENE_INDEX) { EnterEndingState(); return false; }

	int2 const coordinate = sWorld.GetSceneCoordinate(idx); 

	if (idx < 0 || idx == sSceneTracker.mIdx) return false; 

	SetSceneTrackerPrev(sSceneTracker);  
	sSceneTracker.mIdx		= idx;
	sSceneTracker.mType		= idx < sWorld.GetAdjSceneCount() ? SCENE_TYPES_ADJACENT : SCENE_TYPES_EXTRA;
	sSceneTracker.mCoord	= coordinate;

	sSoldiers.mPool.ReturnAll();
	sLandmines.ReturnAll(); 

	if (sSoldiers.GetAlertLevel() <= ALERT_LEVELS_LOW)
	{
		sSoldiers.SetAlertLevel(ALERT_LEVELS_OFF);
		SetScene(&sWorld.mScenes[sSceneTracker.mIdx]);
	}
	else
	{
		sSceneTracker.mScene = &sWorld.mScenes[sSceneTracker.mIdx];
		sTilemap = sSceneTracker.mScene->mTilemap;

		sTilemap->tilePalette->sheet.palette = sSceneTracker.mScene->mColorPalette;

		for (int i = 0; i < sSceneTracker.mScene->mSoldierCount; i++)
		{
			int const soldierIdx = sSoldiers.mPool.WakeObject();
			if (soldierIdx != WAKE_TYPES_FULL) sSoldiers.mPool[soldierIdx].SetPath(sSceneTracker.mScene->mPaths[i]);
		}
	}

	return true; 
}

void Game::InitWorld()
{
	sWorld.mSceneCount		= 23;
	sWorld.mAdjSceneCount	= 17;
	sWorld.mScenes			= new Scene[sWorld.mSceneCount]; 
	sWorld.mColumns			= 4;
	sWorld.mRows			= 5;

	sWorld.mScenes[0]	= Scene("assets/scenes/scene1/scene.scene");
	sWorld.mScenes[1]	= Scene("assets/scenes/scene2/scene.scene");
	sWorld.mScenes[2]	= Scene("assets/scenes/scene3/scene.scene");
	sWorld.mScenes[3]	= Scene("assets/scenes/scene4/scene.scene");
	sWorld.mScenes[4]	= Scene("assets/scenes/scene5/scene.scene");
	sWorld.mScenes[5]	= Scene("assets/scenes/scene6/scene.scene");
	sWorld.mScenes[6]	= Scene("assets/scenes/scene7/scene.scene");
	sWorld.mScenes[7]	= Scene("assets/scenes/scene8/scene.scene");
	sWorld.mScenes[8]	= Scene("assets/scenes/scene9/scene.scene");
	sWorld.mScenes[9]	= Scene("assets/scenes/scene10/scene.scene");
	sWorld.mScenes[10]	= Scene("assets/scenes/scene11/scene.scene");
	sWorld.mScenes[11]	= Scene("assets/scenes/scene12/scene.scene");
	sWorld.mScenes[12]	= Scene("assets/scenes/scene13/scene.scene");
	sWorld.mScenes[13]	= Scene("assets/scenes/scene14/scene.scene");
	sWorld.mScenes[14]	= Scene("assets/scenes/scene15/scene.scene");
	sWorld.mScenes[15]	= Scene("assets/scenes/scene16/scene.scene");
	sWorld.mScenes[16]	= Scene("assets/scenes/scene17/scene.scene");
	// extra scenes:
	sWorld.mScenes[17]	= Scene("assets/scenes/jeep2/scene.scene");
	sWorld.mScenes[18]	= Scene("assets/scenes/jeep1/scene.scene");
	sWorld.mScenes[19]	= Scene("assets/scenes/jeep3/scene.scene");
	sWorld.mScenes[20]	= Scene("assets/scenes/jeep4/scene.scene");
	sWorld.mScenes[21]	= Scene("assets/scenes/jeep5/scene.scene");
	sWorld.mScenes[22]	= Scene("assets/scenes/jeep6/scene.scene");

	sWorld.mScenes[0].mColorPalette		= &Scene::sColorPalette1; 
	sWorld.mScenes[1].mColorPalette		= &Scene::sColorPalette1; 
	sWorld.mScenes[2].mColorPalette		= &Scene::sColorPalette1; 
	sWorld.mScenes[3].mColorPalette		= &Scene::sColorPalette1; 
	sWorld.mScenes[4].mColorPalette		= &Scene::sColorPalette1; 
	sWorld.mScenes[5].mColorPalette		= &Scene::sColorPalette2; 
	sWorld.mScenes[6].mColorPalette		= &Scene::sColorPalette2; 
	sWorld.mScenes[7].mColorPalette		= &Scene::sColorPalette1; 
	sWorld.mScenes[8].mColorPalette		= &Scene::sColorPalette1; 
	sWorld.mScenes[9].mColorPalette		= &Scene::sColorPalette2; 
	sWorld.mScenes[10].mColorPalette	= &Scene::sColorPalette2; 
	sWorld.mScenes[12].mColorPalette	= &Scene::sColorPalette1; 
	sWorld.mScenes[13].mColorPalette	= &Scene::sColorPalette1; 
	sWorld.mScenes[14].mColorPalette	= &Scene::sColorPalette1; 
	sWorld.mScenes[11].mColorPalette	= &Scene::sColorPalette1; 
	sWorld.mScenes[15].mColorPalette	= &Scene::sColorPalette1; 
	sWorld.mScenes[16].mColorPalette	= &Scene::sColorPalette1; 
	sWorld.mScenes[17].mColorPalette	= &Scene::sColorPalette1; 
	sWorld.mScenes[18].mColorPalette	= &Scene::sColorPalette1; 
	sWorld.mScenes[19].mColorPalette	= &Scene::sColorPalette1; 
	sWorld.mScenes[20].mColorPalette	= &Scene::sColorPalette1; 
	sWorld.mScenes[21].mColorPalette	= &Scene::sColorPalette1; 
	sWorld.mScenes[22].mColorPalette	= &Scene::sColorPalette1; 

	sWorld.mScenes[0].mSoldierTypeIdx	= SOLDIER_TYPES_GRAY;
	sWorld.mScenes[1].mSoldierTypeIdx	= SOLDIER_TYPES_GRAY;
	sWorld.mScenes[2].mSoldierTypeIdx	= SOLDIER_TYPES_GRAY;
	sWorld.mScenes[3].mSoldierTypeIdx	= SOLDIER_TYPES_GRAY;
	sWorld.mScenes[4].mSoldierTypeIdx	= SOLDIER_TYPES_RED; 
	sWorld.mScenes[5].mSoldierTypeIdx	= SOLDIER_TYPES_RED; 
	sWorld.mScenes[6].mSoldierTypeIdx	= SOLDIER_TYPES_RED; 
	sWorld.mScenes[7].mSoldierTypeIdx	= SOLDIER_TYPES_RED; 
	sWorld.mScenes[8].mSoldierTypeIdx	= SOLDIER_TYPES_RED; 
	sWorld.mScenes[9].mSoldierTypeIdx	= SOLDIER_TYPES_GRAY; 
	sWorld.mScenes[10].mSoldierTypeIdx	= SOLDIER_TYPES_RED; 
	sWorld.mScenes[12].mSoldierTypeIdx	= SOLDIER_TYPES_RED; 
	sWorld.mScenes[13].mSoldierTypeIdx	= SOLDIER_TYPES_RED; 
	sWorld.mScenes[14].mSoldierTypeIdx	= SOLDIER_TYPES_RED; 
	sWorld.mScenes[11].mSoldierTypeIdx	= SOLDIER_TYPES_GRAY;
	sWorld.mScenes[15].mSoldierTypeIdx	= SOLDIER_TYPES_GRAY;
	sWorld.mScenes[16].mSoldierTypeIdx	= SOLDIER_TYPES_GRAY;
	sWorld.mScenes[17].mSoldierTypeIdx	= SOLDIER_TYPES_GRAY;
	sWorld.mScenes[18].mSoldierTypeIdx	= SOLDIER_TYPES_GRAY;
	sWorld.mScenes[19].mSoldierTypeIdx	= SOLDIER_TYPES_GRAY;
	sWorld.mScenes[20].mSoldierTypeIdx	= SOLDIER_TYPES_GRAY;
	sWorld.mScenes[21].mSoldierTypeIdx	= SOLDIER_TYPES_GRAY;
	sWorld.mScenes[22].mSoldierTypeIdx	= SOLDIER_TYPES_GRAY;

	// place items:
	Scene* scene = nullptr;

	scene = &sWorld.mScenes[13];
	scene->mItemCount = 1;
	scene->mItems = new ItemObject[scene->mItemCount];
	scene->mItems[0].SetType(ITEM_TYPES_BINOCULARS);
	scene->mItems[0].SetPosition({ 144, 64 });
	scene = &sWorld.mScenes[17];
	scene->mItemCount = 1;
	scene->mItems = new ItemObject[scene->mItemCount];
	scene->mItems[0].SetType(ITEM_TYPES_HANDGUN); 
	scene->mItems[0].SetPosition({ 68, 106 });
	scene = &sWorld.mScenes[16];
	scene->mItemCount = 1;
	scene->mItems = new ItemObject[scene->mItemCount];
	scene->mItems[0].SetType(ITEM_TYPES_CARD6); 
	scene->mItems[0].SetPosition({ 128, 50 });
	scene = &sWorld.mScenes[9];
	scene->mItemCount = 1;
	scene->mItems = new ItemObject[scene->mItemCount];
	scene->mItems[0].SetType(ITEM_TYPES_GAS_MASK);
	scene->mItems[0].SetPosition({ 128, 160 });

	// place passages:
	sWorld.mPassageCount = 9; 
	sWorld.mSharedData = new PassageSharedData[sWorld.mPassageCount];
	for (int i = 0; i < sWorld.mPassageCount; i++)
	{
		sWorld.mSharedData[i].mUnlocked		= false;
		sWorld.mSharedData[i].mAccessLevel	= 1;
	}

	// ending door:
	{
	scene = &sWorld.mScenes[0];
	scene->mPassageCount	= 2;
	scene->mPassages		= new Passage*[scene->mPassageCount];
	scene->mPassages[0]		= new Passage(doorNorth);  
	scene->mPassages[1]		= new Passage(doorNorth);   
	scene->mPassages[0]->mSceneIdx = ENDING_SCENE_INDEX;
	scene->mPassages[0]->SetPosition({ 12, 0 });
	scene->mPassages[1]->mSceneIdx = ENDING_SCENE_INDEX;
	scene->mPassages[1]->SetPosition({ 12, 0 });
	Passage::Connect(&sWorld.mSharedData[0], sWorld.mScenes[0].mPassages[0], sWorld.mScenes[0].mPassages[1]);
	}

	scene = &sWorld.mScenes[1];
	scene->mPassageCount = 5;
	scene->mPassages	= new Passage*[scene->mPassageCount];
	scene->mPassages[0] = new Passage(jeepNorth);
	scene->mPassages[1] = new Passage(jeepNorth);
	scene->mPassages[2] = new Passage(jeepNorth);
	scene->mPassages[3] = new Passage(doorSouth);
	scene->mPassages[4] = new Passage(doorEast);
	scene->mPassages[0]->mSceneIdx = 1; // jeep 1
	scene->mPassages[1]->mSceneIdx = 1; // jeep 1
	scene->mPassages[2]->mSceneIdx = 1; // jeep 1
	scene->mPassages[3]->mSceneIdx = 1; 
	scene->mPassages[4]->mSceneIdx = 1;
	scene->mPassages[0]->SetPosition({ 4, 19  });
	scene->mPassages[1]->SetPosition({ 8, 15  });
	scene->mPassages[2]->SetPosition({ 12, 15  });
	scene->mPassages[3]->SetPosition({ 24, 23 });
	scene->mPassages[4]->SetPosition({ 30, 15 });

	scene = &sWorld.mScenes[5];
	scene->mPassageCount = 1;
	scene->mPassages = new Passage*[scene->mPassageCount];
	scene->mPassages[0] = new Passage(doorNorth);
	scene->mPassages[0]->mSceneIdx = 5; // jeep 1
	scene->mPassages[0]->SetPosition({ 24, 0 });

	scene = &sWorld.mScenes[9];
	scene->mPassageCount = 3;
	scene->mPassages = new Passage * [scene->mPassageCount];
	scene->mPassages[0] = new Passage(jeepNorth);
	scene->mPassages[1] = new Passage(jeepNorth);
	scene->mPassages[2] = new Passage(jeepNorth);
	scene->mPassages[0]->mSceneIdx = 9; // jeep 1
	scene->mPassages[1]->mSceneIdx = 9; // jeep 1
	scene->mPassages[2]->mSceneIdx = 9; // jeep 1
	scene->mPassages[0]->SetPosition({ 4, 11 });
	scene->mPassages[1]->SetPosition({ 12, 15 });
	scene->mPassages[2]->SetPosition({ 20, 11 });

	scene = &sWorld.mScenes[2];
	scene->mPassageCount = 1;
	scene->mPassages = new Passage*[scene->mPassageCount];
	scene->mPassages[0] = new Passage(doorWest); 
	scene->mPassages[0]->mSceneIdx = 2; // jeep 1
	scene->mPassages[0]->SetPosition({ 1, 15 });

	// jeeps passages:
	{
	scene = &sWorld.mScenes[17];
	scene->mPassageCount = 1;
	scene->mPassages = new Passage*[scene->mPassageCount];
	scene->mPassages[0] = new Passage(jeepEast);
	scene->mPassages[0]->mSceneIdx = 17; // jeep 1
	scene->mPassages[0]->SetPosition({ 26, 11 });

	scene = &sWorld.mScenes[18];
	scene->mPassageCount = 1;
	scene->mPassages = new Passage * [scene->mPassageCount];
	scene->mPassages[0] = new Passage(jeepEast);
	scene->mPassages[0]->mSceneIdx = 18; // jeep 1
	scene->mPassages[0]->SetPosition({ 26, 11 });

	scene = &sWorld.mScenes[19];
	scene->mPassageCount = 1;
	scene->mPassages = new Passage * [scene->mPassageCount];
	scene->mPassages[0] = new Passage(jeepEast);
	scene->mPassages[0]->mSceneIdx = 19; // jeep 1
	scene->mPassages[0]->SetPosition({ 26, 11 });

	scene = &sWorld.mScenes[20];
	scene->mPassageCount = 1;
	scene->mPassages = new Passage * [scene->mPassageCount];
	scene->mPassages[0] = new Passage(jeepEast);
	scene->mPassages[0]->mSceneIdx = 20; // jeep 1
	scene->mPassages[0]->SetPosition({ 26, 11 });

	scene = &sWorld.mScenes[21];
	scene->mPassageCount = 1;
	scene->mPassages = new Passage * [scene->mPassageCount];
	scene->mPassages[0] = new Passage(jeepEast);
	scene->mPassages[0]->mSceneIdx = 21; // jeep 1
	scene->mPassages[0]->SetPosition({ 26, 11 });

	scene = &sWorld.mScenes[22];
	scene->mPassageCount = 1;
	scene->mPassages = new Passage * [scene->mPassageCount];
	scene->mPassages[0] = new Passage(jeepEast);
	scene->mPassages[0]->mSceneIdx = 22; // jeep 1
	scene->mPassages[0]->SetPosition({ 26, 11 });
	}

	Passage::Connect(&sWorld.mSharedData[1], sWorld.mScenes[1].mPassages[0], sWorld.mScenes[17].mPassages[0]);
	Passage::Connect(&sWorld.mSharedData[2], sWorld.mScenes[1].mPassages[1], sWorld.mScenes[18].mPassages[0]);
	Passage::Connect(&sWorld.mSharedData[3], sWorld.mScenes[1].mPassages[2], sWorld.mScenes[19].mPassages[0]);
	Passage::Connect(&sWorld.mSharedData[4], sWorld.mScenes[1].mPassages[3], sWorld.mScenes[5] .mPassages[0]);
	Passage::Connect(&sWorld.mSharedData[5], sWorld.mScenes[1].mPassages[4], sWorld.mScenes[2].mPassages[0]);

	Passage::Connect(&sWorld.mSharedData[6], sWorld.mScenes[9].mPassages[0], sWorld.mScenes[20].mPassages[0]);
	Passage::Connect(&sWorld.mSharedData[7], sWorld.mScenes[9].mPassages[1], sWorld.mScenes[21].mPassages[0]);
	Passage::Connect(&sWorld.mSharedData[8], sWorld.mScenes[9].mPassages[2], sWorld.mScenes[22].mPassages[0]);
}

void Game::SetSceneTrackerPrev(SceneTracker const& sceneTracker)
{
	sSceneTrackerPrev.mIdx		= sceneTracker.mIdx;
	sSceneTrackerPrev.mType		= sceneTracker.mType;
	sSceneTrackerPrev.mCoord	= sceneTracker.mCoord;
	sSceneTrackerPrev.mScene	= sceneTracker.mScene; 
}

void Game::UpdateLandmines(float const dt)
{
	for (int i = 0; i < sLandmines.SIZE; i++)
	{
		if (sLandmines.IsActive(i))
		{
			LandmineObject& landmine = sLandmines[i];
			if (!landmine.IsExploded())
			{
				if (landmine.Hit(sPlayer))
				{
					sPlayer.Damage(1000000); // enough to kill
				}

				for (int j = 0; j < sSoldiers.mPool.SIZE; j++)
				{
					if (sSoldiers.mPool.IsActive(j) && !sSoldiers.mPool[j].mDestroyed)
					{
						Soldier& soldier = sSoldiers.mPool[j]; 
						if (landmine.Hit(soldier))
						{
							soldier.mDestroyed = true; 
						}
					}
				}

				landmine.Update(dt);
				if (landmine.IsExploded())
				{
					sLandmines.ReturnObject(i);
				}
			}
		}
	}
}

void Game::InitPassages()
{
	doorNorth.dataLocked.columns = 4;
	doorNorth.dataLocked.rows = 4;
	doorNorth.dataUnlocked.columns = 4;
	doorNorth.dataUnlocked.rows = 4;
	doorNorth.spawnPosition = { 16, 48 };
	doorNorth.dataLocked.tile = new uint8_t[4 * 4];
	doorNorth.dataUnlocked.tile = new uint8_t[4 * 4];
	memset(doorNorth.dataLocked.tile, CLOSED_DOOR_TILE_IDX, 4 * 4 * sizeof(uint8_t));
	memset(doorNorth.dataUnlocked.tile, OPEN_DOOR_TILE_IDX, 4 * 4 * sizeof(uint8_t));

	doorSouth.dataLocked.columns = 4;
	doorSouth.dataLocked.rows = 1;
	doorSouth.dataUnlocked.columns = 4;
	doorSouth.dataUnlocked.rows = 1;
	doorSouth.spawnPosition = { 16, -16 };
	doorSouth.dataLocked.tile = new uint8_t[4 * 1];
	doorSouth.dataUnlocked.tile = new uint8_t[4 * 1];
	memset(doorSouth.dataLocked.tile, CLOSED_DOOR_TILE_IDX, 4 * 1 * sizeof(uint8_t));
	memset(doorSouth.dataUnlocked.tile, OPEN_DOOR_TILE_IDX, 4 * 1 * sizeof(uint8_t));

	doorEast.dataLocked.columns = 1;
	doorEast.dataLocked.rows = 4;
	doorEast.dataUnlocked.columns = 1;
	doorEast.dataUnlocked.rows = 4;
	doorEast.spawnPosition = { -8, 16 };
	doorEast.dataLocked.tile = new uint8_t[4];
	doorEast.dataUnlocked.tile = new uint8_t[4];
	memset(doorEast.dataLocked.tile, CLOSED_DOOR_TILE_IDX, 4 * sizeof(uint8_t));
	memset(doorEast.dataUnlocked.tile, OPEN_DOOR_TILE_IDX, 4 * sizeof(uint8_t));

	doorWest.dataLocked.columns = 1;
	doorWest.dataLocked.rows = 4;
	doorWest.dataUnlocked.columns = 1;
	doorWest.dataUnlocked.rows = 4;
	doorWest.spawnPosition = { 16, 16 };
	doorWest.dataLocked.tile = new uint8_t[4];
	doorWest.dataUnlocked.tile = new uint8_t[4];
	memset(doorWest.dataLocked.tile, CLOSED_DOOR_TILE_IDX, 4 * 1 * sizeof(uint8_t));
	memset(doorWest.dataUnlocked.tile, OPEN_DOOR_TILE_IDX, 4 * 1 * sizeof(uint8_t));

	jeepEast.dataLocked.columns = 1;
	jeepEast.dataLocked.rows = 4;
	jeepEast.dataUnlocked.columns = 1;
	jeepEast.dataUnlocked.rows = 4;
	jeepEast.spawnPosition = { -8, 16 };

	jeepNorth.dataLocked.columns = 4;
	jeepNorth.dataLocked.rows = 1;
	jeepNorth.dataUnlocked.columns = 4;
	jeepNorth.dataUnlocked.rows = 1;
	jeepNorth.spawnPosition = { 16, 16 };
}

void Game::RenderLandmines(Surface8* screen)
{
	for (int i = 0; i < sLandmines.SIZE; i++)
	{
		if (sLandmines.IsActive(i)) sLandmines[i].Render(screen);
	}
}

void Game::SetScene(Scene* scene)
{
	sSceneTracker.mScene = scene;

	sTilemap = sSceneTracker.mScene->mTilemap;
	sTilemap->tilePalette->sheet.palette = sSceneTracker.mScene->mColorPalette;

	for (int i = 0; i < sSceneTracker.mScene->mPassageCount; i++)
	{
		if (sSceneTracker.mScene->mPassages[i]->sharedData->mUnlocked && sSceneTracker.mScene->mPassages[i]->props.dataUnlocked.tile) sTilemap->InsertMetaTile(sSceneTracker.mScene->mPassages[i]->mTileCoord, sSceneTracker.mScene->mPassages[i]->props.dataUnlocked);
		else if (sSceneTracker.mScene->mPassages[i]->props.dataLocked.tile) sTilemap->InsertMetaTile(sSceneTracker.mScene->mPassages[i]->mTileCoord, sSceneTracker.mScene->mPassages[i]->props.dataLocked);
	}

	PrepareSoldiers(*sSceneTracker.mScene); 
}

void Game::HandlePlayerLeaveScreen()
{
	if (sPlayer.GetPositionInt().x > NATIVE_SCREEN_WIDTH - 1)
	{ 
		SwitchScene(EAST);
		sPlayer.SetPosition({ 0.0f, sPlayer.GetPosition().y });
	}
	else if (sPlayer.GetPositionInt().x < 0)
	{ 
		SwitchScene(WEST);
		sPlayer.SetPosition({ NATIVE_SCREEN_WIDTH - 1, sPlayer.GetPosition().y });
	}

	if (sPlayer.GetPositionInt().y > NATIVE_SCREEN_HEIGHT - 21)
	{
		SwitchScene(SOUTH);
		sPlayer.SetPosition({ sPlayer.GetPosition().x, 0.0f });
	}
	else if (sPlayer.GetPositionInt().y < 0)
	{ 
		SwitchScene(NORTH);
		sPlayer.SetPosition({ sPlayer.GetPosition().x, NATIVE_SCREEN_HEIGHT - 21 });
	}
}

void Game::PrepareSoldiers(Scene const& scene)
{
	sSoldiers.SetType(scene.mSoldierTypeIdx);

	for (int i = 0; i < scene.mSoldierCount; i++)
	{
		Soldier& soldier = sSoldiers.mPool[sSoldiers.mPool.WakeObject()];
		soldier.SetBehavior(SOLDIER_BEHAVIOR_PATROL); 
		soldier.SetPath(scene.GetPath(i));
		soldier.Reset();
	}
}

void Game::EnterOpeningState(Surface8* screen)
{
	SetMusic(MUSIC_OPENING);  
	sSplashMoveAlarm.Reset();
	sGameStartBlinkAlarm.Reset(); 
	sSplashTimesMoved	= 0;
	sSplashY			= INIT_SPLASH_LOGO_START_Y; 
	sShowGameStart		= INIT_SHOW_GAME_START; 
	sState				= GAME_STATES_OPENING;

	screen->Clear(0);  
}

#include <iostream> 

void Game::PerformanceReport(float const dt) const
{
#if DEBUG_MODE
	screen8->Print(std::to_string(int(1.0f / (dt / 1000.0f))).c_str(), 0, 0, 181);
#endif
}

