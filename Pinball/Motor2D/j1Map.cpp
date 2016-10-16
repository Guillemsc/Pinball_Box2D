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

	// Ball
	ball = new Sprite(App->tex->Load("images/ball.png"),
		0, 0, 33, 32, 50, 50);


	return ret;
}

void j1Map::CreateColliders()
{
	ball_body = App->physics->CreateCircle(50, 50, 50);
	PhysBody* a = App->physics->CreateRectangle(40, 300, 50, 50);
	a->body->SetType(b2_kinematicBody);
}

void j1Map::Draw()
{
	Blit(bg1->texture, bg1->pos.x, bg1->pos.y, &bg1->rect);
	Blit(bg2->texture, bg2->pos.x, bg2->pos.y, &bg2->rect);
	Blit(spring->texture, spring->pos.x, spring->pos.y, &spring->rect);
	Blit(ball->texture, ball_body->body->GetPosition().x , ball_body->body->GetPosition().y, &ball->rect);
}

// Called before quitting
bool j1Map::CleanUp()
{
	LOG("Unloading map");

	return true;
}

void j1Map::Blit(SDL_Texture * texture, int x, int y, const SDL_Rect* section)
{
	App->render->Blit(texture, x, y, section);
}




