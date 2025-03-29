// Template, 2024 IGAD Edition
// Get the latest version from: https://github.com/jbikker/tmpl8
// IGAD/NHTV/BUAS/UU - Jacco Bikker - 2006-2024

#include "precomp.h"
#include "game.h"

#include <iostream> 

#include "Cardinals.h"
#include "Player.h" 
#include "TilePalette.h"
#include "TileMap.h"
#include "Settings.h" 
#include "AABB.h"
#include "World.h"
#include "Projectile.h"
#include "Soldiers.h"
#include "Passage.h" 
#include "InventoryUI.h" 
#include "PlayerUI.h"

PassageProps		propsNorth;
PassageProps		propsSouth;
PassageSharedData	sharedData;

InventoryUI inventoryUI;
PlayerUI	playerUI; 

void Game::Init()
{
#if BAKE_MODE
	Image* image1 = Image::LoadFromFile("assets/images/dog_sprite.png");
	SubSurface8::Save(image1, "assets/spritesheets/dog/dog.sprite");
	//Image* image2 = Image::LoadFromFile("assets/images/outer_heaven_cpalette.png");
	//ColorPalette8::Save(image2, "assets/color_palettes/outer_heaven.cpalette");
#endif BAKE_MODE 

	TilePalette::InitPalettes();
	Soldier::InitSoldierTypes();
	Enemy::target = &player;
	Projectile::player = &player;

	world.scenes = new Scene[17];
	world.columns = 4;
	world.rows = 5;
	world.sceneCount = 17;

	world.scenes[0] = Scene("assets/scenes/scene1/scene.scene");
	world.scenes[1] = Scene("assets/scenes/scene2/scene.scene");
	world.scenes[2] = Scene("assets/scenes/scene3/scene.scene");
	world.scenes[3] = Scene("assets/scenes/scene4/scene.scene");
	world.scenes[4] = Scene("assets/scenes/scene5/scene.scene");
	world.scenes[5] = Scene("assets/scenes/scene6/scene.scene");
	world.scenes[6] = Scene("assets/scenes/scene7/scene.scene");
	world.scenes[7] = Scene("assets/scenes/scene8/scene.scene");
	world.scenes[8] = Scene("assets/scenes/scene9/scene.scene");
	world.scenes[9] = Scene("assets/scenes/scene10/scene.scene");
	world.scenes[10] = Scene("assets/scenes/scene11/scene.scene");
	world.scenes[11] = Scene("assets/scenes/scene12/scene.scene");
	world.scenes[12] = Scene("assets/scenes/scene13/scene.scene");
	world.scenes[13] = Scene("assets/scenes/scene14/scene.scene");
	world.scenes[14] = Scene("assets/scenes/scene15/scene.scene");
	world.scenes[15] = Scene("assets/scenes/scene16/scene.scene");
	world.scenes[16] = Scene("assets/scenes/scene17/scene.scene");

	world.InitWorld();

	//world.scenes[0].items		= new ItemObject*[1];
	//world.scenes[0].items[0]	= new ItemObject(Inventory::HANDGUN);
	//world.scenes[0].itemCount	= 1; 

	propsNorth.dataLocked.columns	= 4; 
	propsNorth.dataLocked.rows		= 4;
	propsNorth.dataUnlocked.columns = 4;
	propsNorth.dataUnlocked.rows	= 4;
	propsNorth.spawnPosition	 = { 16, 40 };
	propsNorth.dataLocked.tile		= new uint8_t[4 * 4];
	propsNorth.dataUnlocked.tile	= new uint8_t[4 * 4];
	memset(propsNorth.dataLocked.tile, 183, 4 * 4 * sizeof(uint8_t)); 
	memset(propsNorth.dataUnlocked.tile, 0, 4 * 4 * sizeof(uint8_t));

	propsSouth.dataLocked.columns	= 4;
	propsSouth.dataLocked.rows		= 1;
	propsSouth.dataUnlocked.columns = 4;
	propsSouth.dataUnlocked.rows	= 1;
	propsSouth.spawnPosition	= { 16, 16 };
	propsSouth.dataLocked.tile		= new uint8_t[4 * 1];
	propsSouth.dataUnlocked.tile	= new uint8_t[4 * 1];
	memset(propsSouth.dataLocked.tile, 183, 4 * 1 * sizeof(uint8_t));
	memset(propsSouth.dataUnlocked.tile, 0, 4 * 1 * sizeof(uint8_t));

	// -------------------------------------------------------------------------

	passage1 = new Passage(propsNorth); 
	passage2 = new Passage(propsSouth);  

	sharedData.unlocked		= false;
	passage1->other			= passage2;
	passage2->other			= passage1;
	passage1->sharedData	= &sharedData;
	passage2->sharedData	= &sharedData;
	passage1->scene			= &world.scenes[0];
	passage2->scene			= &world.scenes[1];

	passage1->SetPosition(TileToPixel({ 12, 12 })); 
	passage2->SetPosition(TileToPixel({ 20, 12 })); 

	if (!sharedData.unlocked)
	{
		world.scenes[0].tilemap->InsertMetaTile({ 12, 12 }, passage1->props.dataLocked);
		world.scenes[1].tilemap->InsertMetaTile({ 20, 12 }, passage2->props.dataLocked);
	}
	else
	{
		world.scenes[0].tilemap->InsertMetaTile({ 12, 12 }, passage1->props.dataUnlocked);
		world.scenes[1].tilemap->InsertMetaTile({ 20, 12 }, passage2->props.dataUnlocked);
	}

	Soldier::SetType(Soldier::types::RED);      

	alertTheme.setLooping(true); 
	mainTheme.setLooping(true);
	SetTheme(&mainTheme);

	inventoryUI.inv = &player.inventory;
	playerUI.player = &player; 
}

void Game::Tick(float const dt)  
{
	switch (state)
	{
	case GAME_STATE_OPENING:	OpeningState(dt);	break;
	case GAME_STATE_PLAYING:	PlayingState(dt);	break; 
	case GAME_STATE_INVENTORY:	InventoryState(dt); break; 
	case GAME_STATE_GAME_OVER:	GameOverState(dt);	break;
	default: break; 
	}

	PerformanceReport(dt);  
}

void Game::OpeningState(float const dt)
{
	state = GAME_STATE_PLAYING; 
}

void Game::PlayingState(float const dt)
{
	if (GetAsyncKeyState(TOGGLE_INVENTORY))
	{
		state = GAME_STATE_INVENTORY;
		inventoryUI.selected = 0; 
	}

	player.Update(dt);

	if (GetAsyncKeyState(PICKUP_ITEM))
	{
		for (int i = 0; i < world.currentScene->itemCount; i++)
		{
			if (AABB::Detect(world.currentScene->items[i]->bbox, player.bbox))
			{
				player.inventory.PickUp(*world.currentScene->items[i]);
			}
		}
	}
	
	if (world.currentScene->tilemap) world.currentScene->tilemap->Render(screen8); 
	
	Soldiers::Update(dt);
	Soldiers::Render(screen8); 
	player.Render(screen8);
	Projectile::UpdatePool(dt); 
	Projectile::RenderPool(screen8);
	
	for (int i = 0; i < world.currentScene->itemCount; i++)
	{
		if (!world.currentScene->items[i]->pickedUp)
		{
			world.currentScene->items[i]->Render(screen8);
		}
	}
	
	passage1->Render(screen8);
	passage2->Render(screen8);
	passage1->TryEnter(); 
	passage2->TryEnter(); 
	
	HandlePlayerLeaveScreen();

	playerUI.Render(screen8); 
}

void Game::InventoryState(float const dt)
{
	if (GetAsyncKeyState(TOGGLE_INVENTORY)) state = GAME_STATE_PLAYING;

	inventoryUI.Update(dt);  

	inventoryUI.Render(screen8);
	playerUI.Render(screen8); 
}

void Game::GameOverState(float const dt)
{
}

void Game::PerformanceReport(float const dt) const
{
#if DEBUG_MODE
	screen8->Print(std::to_string(int(1.0f / (dt / 1000.0f))).c_str(), 0, 0, 181);
#endif
}

void Game::SetTheme(Audio::Sound* sound)
{
#if SOUND_ON
	if (currentTheme) currentTheme->stop();
	currentTheme = sound;
	currentTheme->replay();
#endif
}

void Game::HandlePlayerLeaveScreen()
{
	if (player.bbox.iPos.x > NATIVE_SCREEN_WIDTH - 1)  
	{ 
		world.SwitchScene(EAST); 
		player.SetPosition({ 0.0f, player.bbox.fPos.y }); 
	}
	else if (player.bbox.iPos.x < 0)
	{ 
		world.SwitchScene(WEST);
		player.SetPosition({ NATIVE_SCREEN_WIDTH - 1, player.bbox.fPos.y });
	}

	if (player.bbox.fPos.y > NATIVE_SCREEN_HEIGHT - 21)
	{
		world.SwitchScene(SOUTH);
		player.SetPosition({ player.bbox.fPos.x, 0.0f });
	}
	else if (player.bbox.fPos.y < 0)
	{ 
		world.SwitchScene(NORTH);
		player.SetPosition({ player.bbox.fPos.x, NATIVE_SCREEN_HEIGHT - 21 });
	}
}

