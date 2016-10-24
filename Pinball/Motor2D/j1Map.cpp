#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1FileSystem.h"
#include "j1Textures.h"
#include "j1Map.h"
#include "j1Animation.h"
#include "j1Audio.h"
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
	
	// Music FX
	fx_coll = App->audio->LoadFx("pinball/bonus.wav");
	
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

	// Spritesheet items ----------------

	// Kawaii blue
	kawaii_blue = new SpriteSheetItem(200, 890); 
		kawaii_blue->anim.PushBack({0, 130, 75, 65});
		kawaii_blue->anim.PushBack({76, 130, 75, 65});
		kawaii_blue->anim.speed = 0.00f ;

	// Kawaii green
	kawaii_green = new SpriteSheetItem(340, 890);
		kawaii_green->anim.PushBack({ 152, 0, 75, 65 });
		kawaii_green->anim.PushBack({ 228, 0, 75, 65 });
		kawaii_green->anim.speed = 0.00f;

	// Kawaii orange
	kawaii_orange = new SpriteSheetItem(270, 990);
		kawaii_orange->anim.PushBack({ 152, 65, 75, 65 });
		kawaii_orange->anim.PushBack({ 228, 65, 75, 65 });
		kawaii_orange->anim.speed = 0.00f;

	// Kawaii red
	kawaii_red = new SpriteSheetItem(425, 695);
		kawaii_red->anim.PushBack({ 152, 130, 75, 65 });
		kawaii_red->anim.PushBack({ 228, 130, 75, 65 });
		kawaii_red->anim.speed = 0.00f;

	// Kawaii violet
	kawaii_violet = new SpriteSheetItem(285, 670);
		kawaii_violet->anim.PushBack({ 0, 65, 75, 65 });
		kawaii_violet->anim.PushBack({ 76, 65, 75, 65 });
		kawaii_violet->anim.speed = 0.00f;

	// Kawaii yellow
	kawaii_yellow = new SpriteSheetItem(150, 695);
		kawaii_yellow->anim.PushBack({ 0, 0, 75, 65 });
		kawaii_yellow->anim.PushBack({ 76, 0, 75, 65 });
		kawaii_yellow->anim.speed = 0.00f;

	// Kawaii girl
	kawaii_girl = new SpriteSheetItem(505, 890);
		kawaii_girl->anim.PushBack({ 309, 126, 66, 76 });
		kawaii_girl->anim.PushBack({ 376, 126, 66, 76 });
		kawaii_girl->anim.speed = 0.00f;

	// Kawaii guy
	kawaii_guy = new SpriteSheetItem(40, 910);
		kawaii_guy->anim.PushBack({ 309, 55, 65, 70 });
		kawaii_guy->anim.PushBack({ 375, 55, 65, 70 });
		kawaii_guy->anim.speed = 0.00f;

	// Little button
	little_button = new SpriteSheetItem();
		little_button->anim.PushBack({ 10, 281, 46, 46 });
		little_button->anim.PushBack({ 57, 278, 51, 52 });
		little_button->anim.speed = 0.00f;

	// Little button
	small_bumper = new SpriteSheetItem();
		small_bumper->anim.PushBack({ 105, 212, 24, 24 });
		small_bumper->anim.PushBack({ 144, 213, 59, 59 });
		small_bumper->anim.speed = 0.00f;

	// Blue Arrow
	blue_arrow = new SpriteSheetItem();
		blue_arrow->anim.PushBack({ 7, 212, 48, 61 });
		blue_arrow->anim.PushBack({ 56, 212, 48, 61 });
		blue_arrow->anim.speed = 0.00f;

	// Big bumpers
	big_bumper1 = new SpriteSheetItem();
		big_bumper1->anim.PushBack({ 209, 258, 104, 104 });
		big_bumper1->anim.PushBack({ 385, 266, 114, 113 });
		big_bumper1->anim.speed = 0;

	big_bumper2 = new SpriteSheetItem();
		big_bumper2->anim.PushBack({ 209, 258, 104, 104 });
		big_bumper2->anim.PushBack({ 385, 266, 114, 113 });
		big_bumper2->anim.speed = 0;

	// ----------------------------------

	return ret;
}

void j1Map::CreateColliders()
{
	// Ball
	ball->pb = App->physics->CreateCircle(598, 990, 14, 0x0002, 0x0001); ball->pb->body->SetBullet(1);
	ball->pb->listener = App->physics;
	ball->pb->coll_name = collider_names::ball;
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
		532, 788,
		544, 800,
		560, 817,
		572, 840,
		580, 865,
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
		547, 761,
		526, 745,
		536, 728,
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

	int chain2[76] = {
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
	int small_kicker_left_points[22] = {
		9, 0,
		9, 27,
		12, 28,
		57, 22,
		62, 21,
		65, 18,
		66, 14,
		65, 10,
		62, 7,
		57, 6,
		12, 0
	};
	small_left_kicker_coll = App->physics->CreateRevoluteJoint(12.5, small_kicker_left_points, 20, 130, 554, 15, 15, 200, 150, 250, -90, 0x0003, 0x0002);
	
	int small_kicker_right_points[22] = {
		1, 11,
		3, 9,
		6, 7,
		50, 1,
		55, 1,
		55, 27,
		50, 27,
		6, 22,
		3, 20,
		1, 18,
		0, 15
	};

	// Spring
	int pos_x = 596; int pos_y = 1150;
	PhysBody* A = App->physics->CreateRectangle(pos_x, pos_y, 10, 100, 0x0003, 0x0002); A->body->SetType(b2_staticBody);
	PhysBody* B = App->physics->CreateRectangle(pos_x, pos_y, 50, 70, 0x0003, 0x0002); A->body->SetType(b2_staticBody);
	spring_coll = App->physics->CreatePrismaticJoint(A, B, b2Vec2(1,10), b2Vec2(1, -10), -40, -120, 248, 200);

	// Big bumper
	big_bumper1->pb = App->physics->CreateCircle(370, 576, 40, 0x0003, 0x0002); big_bumper1->pb->body->SetType(b2_kinematicBody);
	big_bumper1->pb->coll_name = collider_names::big_bumper_1;
	big_bumper2->pb = App->physics->CreateCircle(527, 440, 40, 0x0003, 0x0002); big_bumper2->pb->body->SetType(b2_kinematicBody);
	big_bumper2->pb->coll_name = collider_names::big_bumper_2;

	// Sensors -----------------

	kawaii_blue->pb = App->physics->CreateRectangleSensor(236, 923, 64, 64, 0x0003, 0x0002);
	kawaii_blue->pb->coll_name = collider_names::kawaii_blue;

	kawaii_red->pb = App->physics->CreateRectangleSensor(460, 725, 64, 64, 0x0003, 0x0002);
	kawaii_red->pb->coll_name = collider_names::kawaii_red;

	kawaii_violet->pb = App->physics->CreateRectangleSensor(320, 700, 64, 64, 0x0003, 0x0002);
	kawaii_violet->pb->coll_name = collider_names::kawaii_violet;

	kawaii_yellow->pb = App->physics->CreateRectangleSensor(186, 725, 64, 64, 0x0003, 0x0002);
	kawaii_yellow->pb->coll_name = collider_names::kawaii_yellow;

	kawaii_green->pb = App->physics->CreateRectangleSensor(375, 923, 64, 64, 0x0003, 0x0002);
	kawaii_green->pb->coll_name = collider_names::kawaii_green;

	kawaii_orange->pb = App->physics->CreateRectangleSensor(305, 1020, 64, 64, 0x0003, 0x0002);
	kawaii_orange->pb->coll_name = collider_names::kawaii_orange;

	// -------------------------
}

void j1Map::CreateTimers()
{
	kawaii_blue->timer = new Timer(0.2); timers.add(kawaii_blue->timer);
	kawaii_red->timer = new Timer(0.2); timers.add(kawaii_red->timer);
	kawaii_green->timer = new Timer(0.2); timers.add(kawaii_red->timer);
	kawaii_orange->timer = new Timer(0.2); timers.add(kawaii_orange->timer);
	kawaii_yellow->timer = new Timer(0.2); timers.add(kawaii_yellow->timer);
	kawaii_violet->timer = new Timer(0.2); timers.add(kawaii_violet->timer);
	kawaii_guy->timer = new Timer(0.2); timers.add(kawaii_guy->timer);
	kawaii_girl->timer = new Timer(0.2); timers.add(kawaii_girl->timer);

	big_bumper1->timer = new Timer(0.1); timers.add(big_bumper1->timer);
	big_bumper2->timer = new Timer(0.1); timers.add(big_bumper2->timer);
}

void j1Map::Draw()
{
	// Background --------------
	Blit(bg1->texture, bg1->pos.x, bg1->pos.y, &bg1->rect);
	Blit(bg2->texture, bg2->pos.x, bg2->pos.y, &bg2->rect);

	// Kawaiis
	Blit(kawaii_blue->texture, kawaii_blue->pos.x, kawaii_blue->pos.y, &kawaii_blue->anim.GetCurrentFrame());
	Blit(kawaii_red->texture, kawaii_red->pos.x, kawaii_red->pos.y, &kawaii_red->anim.GetCurrentFrame());
	Blit(kawaii_green->texture, kawaii_green->pos.x, kawaii_green->pos.y, &kawaii_green->anim.GetCurrentFrame());
	Blit(kawaii_orange->texture, kawaii_orange->pos.x, kawaii_orange->pos.y, &kawaii_orange->anim.GetCurrentFrame());
	Blit(kawaii_violet->texture, kawaii_violet->pos.x, kawaii_violet->pos.y, &kawaii_violet->anim.GetCurrentFrame());
	Blit(kawaii_yellow->texture, kawaii_yellow->pos.x, kawaii_yellow->pos.y, &kawaii_yellow->anim.GetCurrentFrame());
	Blit(kawaii_girl->texture, kawaii_girl->pos.x, kawaii_girl->pos.y, &kawaii_girl->anim.GetCurrentFrame(), 1, 20);
	Blit(kawaii_guy->texture, kawaii_guy->pos.x, kawaii_guy->pos.y, &kawaii_guy->anim.GetCurrentFrame(), 1, -20);


	// Big bumper
	Blit(big_bumper1->texture, METERS_TO_PIXELS(big_bumper1->pb->body->GetPosition().x) - 56, METERS_TO_PIXELS(big_bumper1->pb->body->GetPosition().y) - 57, &big_bumper1->anim.GetCurrentFrame());
	Blit(big_bumper2->texture, METERS_TO_PIXELS(big_bumper2->pb->body->GetPosition().x) - 56, METERS_TO_PIXELS(big_bumper2->pb->body->GetPosition().y) - 57, &big_bumper2->anim.GetCurrentFrame());

	// -------------------------

	// Spring
	Blit(spring->texture, spring->pos.x, METERS_TO_PIXELS(spring_coll->GetBodyB()->GetPosition().y) -35, &spring->rect);

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




