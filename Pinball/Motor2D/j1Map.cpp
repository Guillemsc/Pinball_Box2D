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
		0, 0, 29, 180);

	// Ball
	ball = new Sprite(App->tex->Load("images/ball.png"),
		0, 0, 33, 32);


	return ret;
}

void j1Map::CreateColliders()
{
	ball->pb = App->physics->CreateCircle(100, 50, 12);
	spring->pb = App->physics->CreateRectangle(597, 950, 30, 20); spring->pb->body->SetType(b2_staticBody);

	// Background standalone colliders
	int chain1 [152] = {
		261, 128,
		254, 98,
		233, 64,
		198, 44,
		162, 36,
		127, 38,
		102, 48,
		82, 62,
		68, 82,
		54, 115,
		52, 162,
		47, 670,
		57, 686,
		65, 701,
		75, 712,
		81, 722,
		81, 834,
		73, 842,
		39, 842,
		40, 1079,
		186, 1157,
		210, 1179,
		225, 1300,
		383, 1298,
		402, 1177,
		423, 1157,
		562, 1083,
		563, 875,
		554, 843,
		532, 810,
		504, 796,
		511, 775,
		535, 785,
		547, 797,
		560, 813,
		574, 838,
		581, 865,
		584, 1178,
		538, 1251,
		535, 1318,
		631, 1343,
		685, 1310,
		696, 1239,
		646, 1196,
		606, 1178,
		608, 915,
		608, 869,
		600, 837,
		584, 804,
		562, 777,
		544, 764,
		521, 754,
		533, 728,
		556, 735,
		580, 689,
		562, 677,
		611, 592,
		612, 228,
		605, 240,
		561, 262,
		546, 242,
		593, 214,
		604, 198,
		610, 181,
		610, 95,
		534, 33,
		430, 33,
		353, 100,
		352, 177,
		366, 212,
		411, 242,
		401, 262,
		351, 235,
		340, 203,
		317, 163,
		270, 138
	};
	PhysBody* colliderb1 = App->physics->CreateChain(0, 0, chain1, 152);
	colliderb1->body->SetType(b2_staticBody);

	int chain2 [76] = {
		216, 138,
		213, 113,
		199, 88,
		175, 69,
		159, 63,
		140, 62,
		123, 68,
		110, 78,
		95, 93,
		83, 110,
		80, 127,
		79, 144,
		89, 669,
		94, 682,
		102, 692,
		113, 713,
		116, 728,
		117, 782,
		117, 840,
		121, 845,
		128, 845,
		132, 842,
		132, 718,
		121, 690,
		104, 670,
		102, 652,
		140, 597,
		116, 553,
		135, 536,
		100, 471,
		97, 311,
		128, 232,
		134, 220,
		188, 187,
		184, 176,
		185, 166,
		198, 141,
		204, 142
	};
	PhysBody* colliderb2 = App->physics->CreateChain(0, 0, chain2, 76);
	colliderb2->body->SetType(b2_staticBody);
	
}

void j1Map::Draw()
{
	Blit(bg1->texture, bg1->pos.x, bg1->pos.y, &bg1->rect);
	Blit(bg2->texture, bg2->pos.x, bg2->pos.y, &bg2->rect);
	Blit(spring->texture, METERS_TO_PIXELS(spring->pb->body->GetPosition().x - 15), METERS_TO_PIXELS(spring->pb->body->GetPosition().y), &spring->rect);
	Blit(ball->texture, METERS_TO_PIXELS(ball->pb->body->GetPosition().x - 14), METERS_TO_PIXELS(ball->pb->body->GetPosition().y - 14), &ball->rect);
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




