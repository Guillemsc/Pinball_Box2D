#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1FileSystem.h"
#include "j1Textures.h"
#include "j1Map.h"
#include "j1Animation.h"
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
		0, 0, 630, 1178, 0, 0);

	// Background 2
	bg2 = new Sprite(App->tex->Load("images/background2.png"), 
		0, 0, 630, 1178, 0, 0);

	// Spring
	spring = new Sprite(App->tex->Load("images/spring.png"), 
		0, 0, 29, 180, 580, 900);

	// Ball
	ball = new Sprite(App->tex->Load("images/ball.png"),
		0, 0, 33, 32);

	// Kickers
	big_left_kicker = new Sprite(App->tex->Load("images/big_kicker_left.png"),
		0, 0, 92, 39);
	big_right_kicker = new Sprite(App->tex->Load("images/big_kicker_right.png"),
		1, 1, 92, 39);

	small_left_kicker = new Sprite(App->tex->Load("images/small_kicker_left.png"),
		0, 0, 67, 28);
	small_right_kicker = new Sprite(App->tex->Load("images/small_kicker_right.png"),
		1, 1, 67, 28);

	// Spritesheet items
	kawaii_blue = new SpriteSheetItem();
	kawaii_green = new SpriteSheetItem();
	kawaii_orange = new SpriteSheetItem();
	kawaii_red = new SpriteSheetItem();
	kawaii_violet = new SpriteSheetItem();
	kawaii_yellow = new SpriteSheetItem();
	kawaii_girl = new SpriteSheetItem();
	kawaii_guy = new SpriteSheetItem();
	little_button = new SpriteSheetItem();
	small_bumper = new SpriteSheetItem();
	blue_arrow = new SpriteSheetItem();

	return ret;
}

void j1Map::CreateColliders()
{
	// Ball
	ball->pb = App->physics->CreateCircle(600, 980, 11, 0x0002, 0x0001);
	balls.add(ball->pb);

	// Background standalone colliders
	int chain1[154] = {
		263, 121,
		254, 98,
		233, 64,
		198, 44,
		162, 36,
		128, 38,
		101, 47,
		82, 62,
		61, 87,
		52, 114,
		48, 155,
		49, 666,
		57, 686,
		65, 701,
		75, 712,
		81, 722,
		81, 834,
		73, 842,
		39, 842,
		41, 1082,
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
		548, 799,
		560, 817,
		574, 839,
		581, 865,
		581, 1177,
		538, 1251,
		535, 1318,
		631, 1343,
		685, 1310,
		696, 1239,
		646, 1196,
		610, 1177,
		610, 915,
		610, 863,
		600, 830,
		585, 800,
		565, 778,
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
		270, 138,
		266, 132
	};
	PhysBody* colliderb1 = App->physics->CreateChain(0, 0, chain1, 154, 0x0001, 0x0002);
	colliderb1->body->SetType(b2_staticBody);

	int chain2 [76] = {
		216, 138,
		213, 113,
		199, 88,
		175, 69,
		159, 63,
		139, 64,
		123, 69,
		111, 78,
		96, 92,
		86, 110,
		81, 125,
		80, 143,
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
	PhysBody* colliderb2 = App->physics->CreateChain(0, 0, chain2, 76, 0x0001, 0x0002);
	colliderb2->body->SetType(b2_staticBody);

	int chain3[10] = {
		127, 888,
		149, 882,
		207, 983,
		194, 998,
		132, 968
	};
	PhysBody* colliderb3 = App->physics->CreateChain(0, 0, chain3, 10, 0x0001, 0x0002);
	colliderb3->body->SetType(b2_staticBody);

	int chain4[10] = {
		463, 882,
		404, 985,
		418, 998,
		478, 967,
		485, 887
	};
	PhysBody* colliderb4 = App->physics->CreateChain(0, 0, chain4, 10, 0x0001, 0x0002);
	colliderb4->body->SetType(b2_staticBody);

	int chain5[26] = {
		84, 992,
		89, 994,
		92, 999,
		103, 1022,
		216, 1087,
		198, 1119,
		94, 1064,
		85, 1053,
		81, 1044,
		77, 1033,
		76, 1020,
		77, 1005,
		78, 998
	};
	PhysBody* colliderb5 = App->physics->CreateChain(0, 0, chain5, 26, 0x0001, 0x0002);
	colliderb5->body->SetType(b2_staticBody);

	int chain6[28] = {
		518, 992,
		523, 990,
		528, 993,
		530, 1002,
		532, 1012,
		531, 1023,
		530, 1034,
		526, 1045,
		517, 1058,
		412, 1117,
		392, 1085,
		506, 1022,
		511, 1011,
		515, 1000
	};
	PhysBody* colliderb6 = App->physics->CreateChain(0, 0, chain6, 28, 0x0001, 0x0002);
	colliderb6->body->SetType(b2_staticBody);

	// Big Kickers
	int big_kicker_left_points[24] = {
		77, 0,
		9, 8,
		5, 11,
		2, 15,
		2, 20,
		3, 25,
		5, 28,
		76, 38,
		86, 34,
		92, 23,
		91, 12,
		84, 3
	};

	big_left_kicker_coll = App->physics->CreateRevoluteJoint(15, big_kicker_left_points, 24, 225, 1107, 70, 20, 200, 150, 250, -90, 0x0003, 0x0002);

	int big_kicker_right_points[24] = {
		13, 1,
		84, 8,
		88, 11,
		91, 17,
		91, 22,
		88, 26,
		84, 29,
		14, 38,
		6, 33,
		0, 25,
		0, 14,
		4, 7
	};
	big_right_kicker_coll = App->physics->CreateRevoluteJoint(15, big_kicker_right_points, 24, 387, 1107, 20, 20, -150, -200, 250, 90, 0x0003, 0x0002);

	// Small Kickers
	int small_kicker_left_points[38] = {
		67, 15,
		66, 12,
		64, 9,
		61, 7,
		57, 6,
		15, 1,
		11, 1,
		7, 2,
		4, 4,
		1, 8,
		0, 12,
		0, 17,
		2, 21,
		6, 25,
		11, 27,
		16, 27,
		59, 22,
		64, 20,
		66, 17
	};

	int small_kicker_right[32] = {
		66, 15,
		66, 11,
		63, 6,
		60, 2,
		54, 1,
		44, 2,
		4, 7,
		1, 10,
		0, 14,
		1, 18,
		5, 21,
		15, 23,
		53, 27,
		60, 26,
		64, 22,
		66, 18
	};

	// Spring
	int pos_x = 596; int pos_y = 1150;
	PhysBody* A = App->physics->CreateRectangle(pos_x, pos_y, 10, 100, 0x0003, 0x0002); A->body->SetType(b2_staticBody);
	PhysBody* B = App->physics->CreateRectangle(pos_x, pos_y, 50, 20, 0x0003, 0x0002); A->body->SetType(b2_staticBody);
	spring_coll = App->physics->CreatePrismaticJoint(A, B, b2Vec2(1, 1), b2Vec2(1, 1), -40, -120, 200, 200);
}

void j1Map::Draw()
{
	// Background
	Blit(bg1->texture, bg1->pos.x, bg1->pos.y, &bg1->rect);
	Blit(bg2->texture, bg2->pos.x, bg2->pos.y, &bg2->rect);

	// Spring
	Blit(spring->texture, spring->pos.x, METERS_TO_PIXELS(spring_coll->GetBodyB()->GetPosition().y) -10, &spring->rect);

	// Ball
	Blit(ball->texture, METERS_TO_PIXELS(ball->pb->body->GetPosition().x - 14), METERS_TO_PIXELS(ball->pb->body->GetPosition().y - 14), &ball->rect);

	// Kickers
	Blit(big_left_kicker->texture, 206, 1090, &big_left_kicker->rect, 1, (-big_left_kicker_coll->GetJointAngle() * RADTODEG) + 180, 18, 20);
	Blit(big_right_kicker->texture, 315, 1090, &big_right_kicker->rect, 1, (-big_right_kicker_coll->GetJointAngle() * RADTODEG) + 180, 73, 20);
	
}

// Called before quitting
bool j1Map::CleanUp()
{
	LOG("Unloading map");

	return true;
}

void j1Map::Blit(SDL_Texture * texture, int x, int y, const SDL_Rect* section, float speed, double angle, int pivot_x, int pivot_y)
{
	App->render->Blit(texture, x, y, section, speed, angle, pivot_x, pivot_y);
}




