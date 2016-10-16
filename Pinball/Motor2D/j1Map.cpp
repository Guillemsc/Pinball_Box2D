#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1FileSystem.h"
#include "j1Textures.h"
#include "j1Map.h"
#include <math.h>

j1Map::j1Map() : j1Module()
{
	name.create("map");
}

// Destructor
j1Map::~j1Map()
{}

// Called before render is available
bool j1Map::Awake(pugi::xml_node& config)
{
	bool ret = true;


	return ret;
}

bool j1Map::Start()
{
	bool ret = true;
	
	// Background 1
	background1 = App->tex->Load("images/background1.png");
	background1_rect.x = 0;
	background1_rect.y = 0;
	background1_rect.w = 630;
	background1_rect.h = 1080;

	// Background 2
	background2 = App->tex->Load("images/background2.png");
	background2_rect.x = 0;
	background2_rect.y = 0;
	background2_rect.w = 630;
	background2_rect.h = 1080;

	return ret;
}

void j1Map::Draw()
{
	App->render->Blit(background1, 0, 0, &background1_rect);
	App->render->Blit(background2, 0, 0, &background2_rect);
}

// Called before quitting
bool j1Map::CleanUp()
{
	LOG("Unloading map");
	//RELEASE(background1);

	return true;
}




