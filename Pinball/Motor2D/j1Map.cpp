#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1FileSystem.h"
#include "j1Textures.h"
#include "j1Map.h"
#include <math.h>
#include "j1Physics.h"

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
	bg1 = new Sprite(App->tex->Load("images/background1.png"), 
		0, 0, 630, 1080, 0, 0);

	// Background 2
	bg2 = new Sprite(App->tex->Load("images/background2.png"), 
		0, 0, 630, 1080, 0, 0);

	// Spring
	spring = new Sprite(App->tex->Load("images/spring.png"),
		0, 0, 29, 180, 581, 950);


	return ret;
}

void j1Map::CreateColliders()
{
	App->physics->CreateCircle(50, 50, 10);
}

void j1Map::Draw()
{
	App->render->Blit(bg1->texture, bg1->pos.x, bg1->pos.y, &bg1->rect);
	App->render->Blit(bg2->texture, bg2->pos.x, bg2->pos.y, &bg2->rect);
	App->render->Blit(spring->texture, spring->pos.x, spring->pos.y, &spring->rect);
}

// Called before quitting
bool j1Map::CleanUp()
{
	LOG("Unloading map");
	//RELEASE(background1);

	return true;
}




