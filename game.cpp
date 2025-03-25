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
#include "Dog.h"
#include "World.h"
#include "Projectile.h"
#include "Soldiers.h"

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
	Enemy::target		= &player;
	Projectile::player	= &player; 

	world.scenes		= new Scene[17];
	world.columns		= 4;
	world.rows			= 5;
	world.sceneCount	= 17;

	world.scenes[0]		= Scene("assets/scenes/scene1/scene.scene");  
	world.scenes[1]		= Scene("assets/scenes/scene2/scene.scene");  
	world.scenes[2]		= Scene("assets/scenes/scene3/scene.scene");  
	world.scenes[3]		= Scene("assets/scenes/scene4/scene.scene");  
	world.scenes[4]		= Scene("assets/scenes/scene5/scene.scene");  
	world.scenes[5]		= Scene("assets/scenes/scene6/scene.scene");  
	world.scenes[6]		= Scene("assets/scenes/scene7/scene.scene");  
	world.scenes[7]		= Scene("assets/scenes/scene8/scene.scene");  
	world.scenes[8]		= Scene("assets/scenes/scene9/scene.scene");  
	world.scenes[9]		= Scene("assets/scenes/scene10/scene.scene");  
	world.scenes[10]	= Scene("assets/scenes/scene11/scene.scene");  
	world.scenes[11]	= Scene("assets/scenes/scene12/scene.scene");  
	world.scenes[12]	= Scene("assets/scenes/scene13/scene.scene");  
	world.scenes[13]	= Scene("assets/scenes/scene14/scene.scene");  
	world.scenes[14]	= Scene("assets/scenes/scene15/scene.scene");  
	world.scenes[15]	= Scene("assets/scenes/scene16/scene.scene");  
	world.scenes[16]	= Scene("assets/scenes/scene17/scene.scene");  

	world.InitWorld();

	Soldier::SetType(Soldier::types::GRAY);    

	alertTheme.setLooping(true); 
	mainTheme.setLooping(true);
	currentTheme = &mainTheme;
	currentTheme->play(); 
}

void Game::Tick(float dt)  
{
	screen8->Clear(100);     
	player.Update(dt);

	if (world.currentScene->tilemap) world.currentScene->tilemap->Render(screen8); 

	Soldiers::Update(dt);
	Soldiers::Render(screen8); 
	player.Render(screen8);
	Projectile::UpdatePool(dt); 
	Projectile::RenderPool(screen8);

	HandlePlayerLeaveScreen();

	// performance report:
	screen8->Print(std::to_string(int(1.0f / (dt / 1000.0f))).c_str(), 0, 0, 181);
}

void Game::SetTheme(Audio::Sound* sound)
{
	currentTheme->stop();
	currentTheme = sound;
	currentTheme->replay();  
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

	if (player.bbox.fPos.y > NATIVE_SCREEN_HEIGHT - 1)
	{
		world.SwitchScene(SOUTH);
		player.SetPosition({ player.bbox.fPos.x, 0.0f });
	}
	else if (player.bbox.fPos.y < 0)
	{ 
		world.SwitchScene(NORTH);
		player.SetPosition({ player.bbox.fPos.x, NATIVE_SCREEN_HEIGHT - 1 });
	}
}

