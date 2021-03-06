#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1FileSystem.h"
#include "j1Textures.h"
#include "j1Map.h"
#include "SceneManager.h"
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

	print_points1 = new PrintPoints(App->tex->Load("menu/+100.png"), 0, 0, 46, 21, App->tex->Load("menu/+500.png"), 0, 0, 46, 21, 1);
	spritesheet = App->tex->Load("images/spritesheet.png");

	// Audio MUSIC
	App->audio->PlayMusic("audio/music/game_music.mp3");

	// Audio FX
	spring_pull_fx = App->audio->LoadFx("audio/fx/spring_pull_fx.wav");
	spring_push_fx = App->audio->LoadFx("audio/fx/spring_push_fx.wav");
	kicker1_fx = App->audio->LoadFx("audio/fx/kicker1_fx.wav");
	kicker2_fx = App->audio->LoadFx("audio/fx/kicker2_fx.wav");
	ball_hit_fx = App->audio->LoadFx("audio/fx/ball_hit_fx.wav");
	ball_lost_fx = App->audio->LoadFx("audio/fx/ball_lost_fx.wav");
	kawaii_sound1_fx = App->audio->LoadFx("audio/fx/kawaii_sound1_fx.wav");
	kawaii_sound2_fx = App->audio->LoadFx("audio/fx/kawaii_sound2_fx.wav");
	kawaii_sound3_fx = App->audio->LoadFx("audio/fx/kawaii_sound3_fx.wav");
	kawaii_sound4_fx = App->audio->LoadFx("audio/fx/kawaii_sound4_fx.wav");
	big_pixel1_fx = App->audio->LoadFx("audio/fx/big_pixel1_fx.wav");
	big_pixel2_fx = App->audio->LoadFx("audio/fx/big_pixel2_fx.wav");
	multiball_release_fx = App->audio->LoadFx("audio/fx/multiball_release_fx.wav");
	multiball_fx = App->audio->LoadFx("audio/fx/multiball_fx.wav");
	slingshot_fx = App->audio->LoadFx("audio/fx/slingshot_fx.wav");
	target_fx = App->audio->LoadFx("audio/fx/target_fx.wav");
	small_bumper_fx = App->audio->LoadFx("audio/fx/small_bumper_fx.wav");
	big_bumper_fx = App->audio->LoadFx("audio/fx/big_bumper_fx.wav");
	piyo_bonus_fx = App->audio->LoadFx("audio/fx/piyo_bonus_fx.wav");
	meow_meow_happy_bonus_fx = App->audio->LoadFx("audio/fx/meow_meow_happy_bonus_fx.wav");
	big_pixel_bonus_fx = App->audio->LoadFx("audio/fx/big_pixel_bonus_fx.wav");
	loop_bonus_fx = App->audio->LoadFx("audio/fx/loop_bonus_fx.wav");

	// Background 1
	bg1 = new Sprite(App->tex->Load("images/background1.png"), 
		0, 0, 630, 1178, 0, 0);

	// Background 2
	bg2 = new Sprite(App->tex->Load("images/background2.png"), 
		0, 0, 630, 1178, 0, 0);

	// Spring
	spring = new Sprite(App->tex->Load("images/spring.png"), 
		0, 0, 29, 180, 580, 900);

	// Balls
	ball = new Sprite(App->tex->Load("images/ball.png"),
		0, 0, 33, 32);

	ball1 = new Sprite(App->tex->Load("images/ball.png"),
		0, 0, 33, 32);

	ball2 = new Sprite(App->tex->Load("images/ball.png"),
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

	// Target
	target = new Sprite(App->tex->Load("images/target.png"),
		0, 0, 21, 45);
	target2 = new Sprite(App->tex->Load("images/target.png"),
		0, 0, 21, 45);
	target3 = new Sprite(App->tex->Load("images/target.png"),
		0, 0, 21, 45);
	target4 = new Sprite(App->tex->Load("images/target.png"),
		0, 0, 21, 45);
	target5 = new Sprite(App->tex->Load("images/target.png"),
		0, 0, 21, 45);
	target6 = new Sprite(App->tex->Load("images/target.png"),
		0, 0, 21, 45);
	target7 = new Sprite(App->tex->Load("images/target.png"),
		0, 0, 21, 45);
	target8 = new Sprite(App->tex->Load("images/target.png"),
		0, 0, 21, 45);

	// Spritesheet items ----------------

	// Kawaii blue
	kawaii_blue = new SpriteSheetItem(200, 890); 
		kawaii_blue->anim.PushBack({0, 130, 75, 65});
		kawaii_blue->anim.PushBack({76, 130, 75, 65});
		kawaii_blue->anim.speed = 0.00f ;
		s_s_i.add(kawaii_blue);

	// Kawaii green
	kawaii_green = new SpriteSheetItem(340, 890);
		kawaii_green->anim.PushBack({ 152, 0, 75, 65 });
		kawaii_green->anim.PushBack({ 228, 0, 75, 65 });
		kawaii_green->anim.speed = 0.00f;
		s_s_i.add(kawaii_green);

	// Kawaii orange
	kawaii_orange = new SpriteSheetItem(270, 990);
		kawaii_orange->anim.PushBack({ 152, 65, 75, 65 });
		kawaii_orange->anim.PushBack({ 228, 65, 75, 65 });
		kawaii_orange->anim.speed = 0.00f;
		s_s_i.add(kawaii_orange);

	// Kawaii red
	kawaii_red = new SpriteSheetItem(425, 695);
		kawaii_red->anim.PushBack({ 152, 130, 75, 65 });
		kawaii_red->anim.PushBack({ 228, 130, 75, 65 });
		kawaii_red->anim.speed = 0.00f;
		s_s_i.add(kawaii_red);

	// Kawaii violet
	kawaii_violet = new SpriteSheetItem(285, 670);
		kawaii_violet->anim.PushBack({ 0, 65, 75, 65 });
		kawaii_violet->anim.PushBack({ 76, 65, 75, 65 });
		kawaii_violet->anim.speed = 0.00f;
		s_s_i.add(kawaii_violet);

	// Kawaii yellow
	kawaii_yellow = new SpriteSheetItem(150, 695);
		kawaii_yellow->anim.PushBack({ 0, 0, 75, 65 });
		kawaii_yellow->anim.PushBack({ 76, 0, 75, 65 });
		kawaii_yellow->anim.speed = 0.00f;
		s_s_i.add(kawaii_yellow);

	// Kawaii girl
	kawaii_girl = new SpriteSheetItem(505, 890);
		kawaii_girl->anim.PushBack({ 309, 126, 66, 76 });
		kawaii_girl->anim.PushBack({ 375, 126, 66, 76 });
		kawaii_girl->anim.speed = 0.00f;
		s_s_i.add(kawaii_girl);

	// Kawaii guy
	kawaii_guy = new SpriteSheetItem(40, 910);
		kawaii_guy->anim.PushBack({ 309, 55, 65, 70 });
		kawaii_guy->anim.PushBack({ 375, 55, 65, 70 });
		kawaii_guy->anim.speed = 0.00f;
		s_s_i.add(kawaii_guy);

	// Kawaii box
	kawaii_box1 = new SpriteSheetItem(453, 130);
		kawaii_box1->anim.PushBack({ 313, 0, 58, 53 });
		kawaii_box1->anim.PushBack({ 377, 0, 58, 53 });
		kawaii_box1->anim.speed = 0.00f;
		s_s_i.add(kawaii_box1);

	kawaii_box2 = new SpriteSheetItem(318, 347);
		kawaii_box2->anim.PushBack({ 313, 0, 58, 53 });
		kawaii_box2->anim.PushBack({ 377, 0, 58, 53 });
		kawaii_box2->anim.speed = 0.00f;
		s_s_i.add(kawaii_box2);

	// Little button
	little_button1 = new SpriteSheetItem();
		little_button1->anim.PushBack({ 102, 334, 55, 55 });
		little_button1->anim.PushBack({ 8, 278, 52, 52 });
		little_button1->anim.speed = 0.00f;
		s_s_i.add(little_button1);

	little_button2 = new SpriteSheetItem();
		little_button2->anim.PushBack({ 102, 334, 55, 55 });
		little_button2->anim.PushBack({ 8, 278, 52, 52 });
		little_button2->anim.speed = 0.00f;
		s_s_i.add(little_button2);

	little_button3 = new SpriteSheetItem();
		little_button3->anim.PushBack({ 102, 334, 55, 55 });
		little_button3->anim.PushBack({ 8, 278, 52, 52 });
		little_button3->anim.speed = 0.00f;
		s_s_i.add(little_button3);

	little_button4 = new SpriteSheetItem();
		little_button4->anim.PushBack({ 102, 334, 55, 55 });
		little_button4->anim.PushBack({ 8, 278, 52, 52 });
		little_button4->anim.speed = 0.00f;
		s_s_i.add(little_button4);

	little_button5 = new SpriteSheetItem();
		little_button5->anim.PushBack({ 102, 334, 55, 55 });
		little_button5->anim.PushBack({ 8, 278, 52, 52 });
		little_button5->anim.speed = 0.00f;
		s_s_i.add(little_button5);

	little_button6 = new SpriteSheetItem();
		little_button6->anim.PushBack({ 102, 334, 55, 55 });
		little_button6->anim.PushBack({ 8, 278, 52, 52 });
		little_button6->anim.speed = 0.00f;
		s_s_i.add(little_button6);

	little_button7 = new SpriteSheetItem();
		little_button7->anim.PushBack({ 102, 334, 55, 55 });
		little_button7->anim.PushBack({ 8, 278, 52, 52 });
		little_button7->anim.speed = 0.00f;
		s_s_i.add(little_button7);

	little_button8 = new SpriteSheetItem();
		little_button8->anim.PushBack({ 102, 334, 55, 55 });
		little_button8->anim.PushBack({ 8, 278, 52, 52 });
		little_button8->anim.speed = 0.00f;
		s_s_i.add(little_button8);

	// Small Bumpers
	small_bumper1 = new SpriteSheetItem();
		small_bumper1->anim.PushBack({ 141, 225, 55, 55 });
		small_bumper1->anim.PushBack({ 577, 291, 70, 70 });
		small_bumper1->anim.speed = 0.00f;
		s_s_i.add(small_bumper1);


	small_bumper2 = new SpriteSheetItem();
		small_bumper2->anim.PushBack({ 141, 225, 55, 55 });
		small_bumper2->anim.PushBack({ 577, 291, 70, 70 });
		small_bumper2->anim.speed = 0.00f;
		s_s_i.add(small_bumper2);

	small_bumper3 = new SpriteSheetItem();
		small_bumper3->anim.PushBack({ 141, 225, 55, 55 });
		small_bumper3->anim.PushBack({ 577, 291, 70, 70 });
		small_bumper3->anim.speed = 0.00f;
		small_bumper3->anim.SetFrame(0);
		s_s_i.add(small_bumper3);

	small_bumper4 = new SpriteSheetItem();
		small_bumper4->anim.PushBack({ 141, 225, 55, 55 });
		small_bumper4->anim.PushBack({ 577, 291, 70, 70 });
		small_bumper4->anim.speed = 0.00f;
		s_s_i.add(small_bumper4);

	small_bumper5 = new SpriteSheetItem();
		small_bumper5->anim.PushBack({ 141, 225, 55, 55 });
		small_bumper5->anim.PushBack({ 577, 291, 70, 70 });
		small_bumper5->anim.speed = 0.00f;
		s_s_i.add(small_bumper5);

	small_bumper6 = new SpriteSheetItem();
		small_bumper6->anim.PushBack({ 141, 225, 55, 55 });
		small_bumper6->anim.PushBack({ 577, 291, 70, 70 });
		small_bumper6->anim.speed = 0.00f;
		s_s_i.add(small_bumper6);

	small_bumper7 = new SpriteSheetItem();
		small_bumper7->anim.PushBack({ 141, 225, 55, 55 });
		small_bumper7->anim.PushBack({ 577, 291, 70, 70 });
		small_bumper7->anim.speed = 0.00f;
		s_s_i.add(small_bumper7);

	// Blue Arrow (?)
	blue_arrow = new SpriteSheetItem();
		blue_arrow->anim.PushBack({ 7, 212, 48, 61 });
		blue_arrow->anim.PushBack({ 56, 212, 48, 61 });
		blue_arrow->anim.speed = 0.00f;
		s_s_i.add(blue_arrow);

	// Big bumpers
	big_bumper1 = new SpriteSheetItem();
		big_bumper1->anim.PushBack({ 202, 250, 104, 104 });
		big_bumper1->anim.PushBack({ 379, 258, 127, 127 });
		big_bumper1->anim.speed = 0.00f;
		s_s_i.add(big_bumper1);

	big_bumper2 = new SpriteSheetItem();
		big_bumper2->anim.PushBack({ 202, 250, 104, 104 });
		big_bumper2->anim.PushBack({ 379, 258, 127, 127 });
		big_bumper2->anim.speed = 0.00f;
		s_s_i.add(big_bumper2);

	// ----------------------------------

	return ret;
}

void j1Map::CreateColliders()
{
	// Balls
	ball->pb = App->physics->CreateCircle(598, 990, 14, 0.4f, 0x0002, 0x0001); ball->pb->body->SetBullet(1);
	ball->pb->listener = App->physics;
	ball->pb->coll_name = collider_names::ball;
	print_points1->balls.add(ball->pb);

	/*
	ball1->pb = App->physics->CreateCircle(598, 990, 14, 0.4f, 0x0002, 0x0001); ball->pb->body->SetBullet(1); 
	ball1->pb->listener = App->physics;
	ball1->pb->coll_name = collider_names::ball;
	print_points1->balls.add(ball->pb);
	*/


	// Background standalone colliders

	int chain1[154] = {
		263, 121,
		259, 98,
		236, 62,
		203, 43,
		164, 31,
		125, 31,
		98, 44,
		79, 60,
		60, 84,
		49, 114,
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
	colliderb1->coll_name = collider_names::wall;

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
	colliderb2->coll_name = collider_names::wall;

	int chain3[10] = {
		127, 888,
		149, 882,
		207, 983,
		194, 998,
		132, 968
	};
	PhysBody* colliderb3 = App->physics->CreateChain(0, 0, chain3, 10, 0x0001, 0x0002);
	colliderb3->body->SetType(b2_staticBody);
	colliderb3->coll_name = collider_names::wall;
	
	int chainA[4] = {
		149, 882,
		207, 983
	};
	collider_slingshot1 = App->physics->CreateChain(0, 0, chainA, 4, 0x0001, 0x0002, 1.25f);
	collider_slingshot1->body->SetType(b2_staticBody);
	collider_slingshot1->coll_name = collider_names::slingshot;

	int chain4[10] = {
		463, 882,
		404, 985,
		418, 998,
		478, 967,
		485, 887
	};
	PhysBody* colliderb4 = App->physics->CreateChain(0, 0, chain4, 10, 0x0001, 0x0002);
	colliderb4->body->SetType(b2_staticBody);
	colliderb4->coll_name = collider_names::wall;

	int chainB[4] = {
		463, 882,
		404, 985
	};
	collider_slingshot2 = App->physics->CreateChain(0, 0, chainB, 4, 0x0001, 0x0002, 1.25f);
	collider_slingshot2->body->SetType(b2_staticBody);
	collider_slingshot2->coll_name = collider_names::slingshot;

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
	colliderb5->coll_name = collider_names::wall;

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
	colliderb6->coll_name = collider_names::wall;

	// Big Kickers
	int big_kicker_left_points[16] = {
		90, 4,
		9, 8,
		5, 11,
		2, 15,
		2, 20,
		3, 24,
		8, 30,
		97, 32
	};
	PhysBody* b1 = App->physics->CreateCircle(225, 1107, 15, 0.00f, 0x0003, 0x0002); b1->body->SetType(b2_staticBody);
	big_left_kicker_coll = App->physics->CreateRevoluteJoint(b1, big_kicker_left_points, 16, 225, 1107, 70, 20, 200, 150, 250, -90, 0x0003, 0x0002);

	PhysBody* b2 = App->physics->CreateCircle(387, 1107, 15, 0.00f, 0x0003, 0x0002); b2->body->SetType(b2_staticBody);
	int big_kicker_right_points[16] = {
		5, 1,
		84, 8,
		88, 11,
		91, 17,
		91, 22,
		88, 26,
		84, 29,
		0, 36
	};
	big_right_kicker_coll = App->physics->CreateRevoluteJoint(b2, big_kicker_right_points, 16, 387, 1107, 20, 20, -150, -200, 250, 90, 0x0003, 0x0002);

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
	PhysBody* b3 = App->physics->CreateCircle(130, 554, 12.5, 0.00f, 0x0003, 0x0002); b3->body->SetType(b2_staticBody);
	small_left_kicker_coll = App->physics->CreateRevoluteJoint(b3, small_kicker_left_points, 20, 130, 554, 15, 15, 360, 300, 200, -90, 0x0003, 0x0002);
	
	PhysBody* b4 = App->physics->CreateCircle(412, 258, 12.5, 0.00f, 0x0003, 0x0002); b4->body->SetType(b2_staticBody);
	small_left_kicker_coll2 = App->physics->CreateRevoluteJoint(b4, small_kicker_left_points, 20, 412, 258, 15, 15, 15, -25, 200, -90, 0x0003, 0x0002);

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
	PhysBody* b5 = App->physics->CreateCircle(570, 690, 12.5, 0.00f, 0x0003, 0x0002); b5->body->SetType(b2_staticBody);
	small_right_kicker_coll = App->physics->CreateRevoluteJoint(b5, small_kicker_right_points, 20, 570, 690, 50, 15, 55, 5, 200, -90, 0x0003, 0x0002);
	
	PhysBody* b6 = App->physics->CreateCircle(550, 258, 12.5, 0.00f, 0x0003, 0x0002); b6->body->SetType(b2_staticBody);
	small_right_kicker_coll2 = App->physics->CreateRevoluteJoint(b6, small_kicker_right_points, 20, 550, 258, 50, 15, 30, -10, 200, -90, 0x0003, 0x0002);

	// Spring
	int pos_x = 596; int pos_y = 1150;
	PhysBody* A = App->physics->CreateRectangle(pos_x, pos_y, 10, 100, 0x0003, 0x0002); A->body->SetType(b2_staticBody);
	PhysBody* B = App->physics->CreateRectangle(pos_x, pos_y, 50, 70, 0x0003, 0x0002); A->body->SetType(b2_staticBody);
	spring_coll = App->physics->CreatePrismaticJoint(A, B, b2Vec2(1,10), b2Vec2(1, -10), -40, -120, 350, 200);

	// Big bumpers
	big_bumper1->pb = App->physics->CreateCircle(370, 576, 40, 0.7f, 0x0003, 0x0002); big_bumper1->pb->body->SetType(b2_kinematicBody);
	big_bumper1->pb->coll_name = collider_names::big_bumper_1;
	big_bumper2->pb = App->physics->CreateCircle(527, 440, 40, 0.7f, 0x0003, 0x0002); big_bumper2->pb->body->SetType(b2_kinematicBody);
	big_bumper2->pb->coll_name = collider_names::big_bumper_2;

	// Small bumpers
	small_bumper1->pb = App->physics->CreateCircle(540, 880, 17, 0.8f, 0x0003, 0x0002); small_bumper1->pb->body->SetType(b2_kinematicBody);
	small_bumper1->pb->coll_name = collider_names::small_bumper_1;

	small_bumper2->pb = App->physics->CreateCircle(130, 350, 17, 0.8f, 0x0003, 0x0002); small_bumper2->pb->body->SetType(b2_kinematicBody);
	small_bumper2->pb->coll_name = collider_names::small_bumper_2;

	small_bumper3->pb = App->physics->CreateCircle(300, 180, 17, 0.8f, 0x0003, 0x0002); small_bumper3->pb->body->SetType(b2_kinematicBody);
	small_bumper3->pb->coll_name = collider_names::small_bumper_3;

	small_bumper4->pb = App->physics->CreateCircle(405, 160, 17, 0.8f, 0x0003, 0x0002); small_bumper4->pb->body->SetType(b2_kinematicBody);
	small_bumper4->pb->coll_name = collider_names::small_bumper_4;

	small_bumper5->pb = App->physics->CreateCircle(540, 160, 17, 0.8f, 0x0003, 0x0002); small_bumper5->pb->body->SetType(b2_kinematicBody);
	small_bumper5->pb->coll_name = collider_names::small_bumper_5;

	small_bumper6->pb = App->physics->CreateCircle(580, 285, 17, 0.8f, 0x0003, 0x0002); small_bumper6->pb->body->SetType(b2_kinematicBody);
	small_bumper6->pb->coll_name = collider_names::small_bumper_6;

	small_bumper7->pb = App->physics->CreateCircle(480, 100, 17, 0.8f, 0x0003, 0x0002); small_bumper7->pb->body->SetType(b2_kinematicBody);
	small_bumper7->pb->coll_name = collider_names::small_bumper_7;

	// Targets
	target->pb = App->physics->CreateCircle(123, 622, 20, 1.25f, 0x0003, 0x0002); target->pb->body->SetType(b2_staticBody);
	target->pb->coll_name = collider_names::target;
	target2->pb = App->physics->CreateCircle(102, 448, 18, 1.25f, 0x0003, 0x0002); target2->pb->body->SetType(b2_staticBody);
	target2->pb->coll_name = collider_names::target2;
	target3->pb = App->physics->CreateCircle(102, 398, 18, 1.25f, 0x0003, 0x0002); target3->pb->body->SetType(b2_staticBody);
	target3->pb->coll_name = collider_names::target3;
	target4->pb = App->physics->CreateCircle(613, 573, 18, 1.25f, 0x0003, 0x0002); target4->pb->body->SetType(b2_staticBody);
	target4->pb->coll_name = collider_names::target4;
	target5->pb = App->physics->CreateCircle(613, 350, 18, 1.25f, 0x0003, 0x0002); target5->pb->body->SetType(b2_staticBody);
	target5->pb->coll_name = collider_names::target5;
	target6->pb = App->physics->CreateCircle(118, 272, 18, 1.25f, 0x0003, 0x0002); target6->pb->body->SetType(b2_staticBody);
	target6->pb->coll_name = collider_names::target6; 
	target7->pb = App->physics->CreateCircle(570, 65, 18, 1.25f, 0x0003, 0x0002); target7->pb->body->SetType(b2_staticBody);
	target7->pb->coll_name = collider_names::target7;
	target8->pb = App->physics->CreateCircle(405, 63, 18, 1.25f, 0x0003, 0x0002); target8->pb->body->SetType(b2_staticBody);
	target8->pb->coll_name = collider_names::target8;

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

	kawaii_girl->pb = App->physics->CreateRectangleSensor(525, 940, 50, 64, 0x0003, 0x0002);
	kawaii_girl->pb->coll_name = collider_names::kawaii_girl;

	kawaii_guy->pb = App->physics->CreateRectangleSensor(83, 935, 50, 64, 0x0003, 0x0002);
	kawaii_guy->pb->coll_name = collider_names::kawaii_guy;

	kawaii_box1->pb = App->physics->CreateRectangleSensor(480, 160, 50, 50, 0x0003, 0x0002);
	kawaii_box1->pb->coll_name = collider_names::kawaii_box1;

	kawaii_box2->pb = App->physics->CreateRectangleSensor(345, 375, 50, 50, 0x0003, 0x0002);
	kawaii_box2->pb->coll_name = collider_names::kawaii_box2;

	ball_dead_detector = App->physics->CreateRectangleSensor(300, 1250, 300, 30, 0x0003, 0x0002);
	ball_dead_detector->coll_name = collider_names::ball_dead_detector;

	loop_detector = App->physics->CreateRectangleSensor(77, 705, 75, 25, 0x0003, 0x0002);
	loop_detector->coll_name = collider_names::loop_detector;

	little_button1->pb = App->physics->CreateCircleSensor(150, 665, 20, 0x0003, 0x0002); little_button1->pb->body->SetType(b2_kinematicBody);
	little_button2->pb = App->physics->CreateCircleSensor(550, 580, 20, 0x0003, 0x0002); little_button2->pb->body->SetType(b2_kinematicBody);
	little_button3->pb = App->physics->CreateCircleSensor(145, 460, 20, 0x0003, 0x0002); little_button3->pb->body->SetType(b2_kinematicBody);
	little_button4->pb = App->physics->CreateCircleSensor(145, 405, 20, 0x0003, 0x0002); little_button4->pb->body->SetType(b2_kinematicBody);	
	little_button5->pb = App->physics->CreateCircleSensor(550, 350, 20, 0x0003, 0x0002); little_button5->pb->body->SetType(b2_kinematicBody);	
	little_button6->pb = App->physics->CreateCircleSensor(420, 112, 20, 0x0003, 0x0002); little_button6->pb->body->SetType(b2_kinematicBody);
	little_button7->pb = App->physics->CreateCircleSensor(530, 112, 20, 0x0003, 0x0002); little_button7->pb->body->SetType(b2_kinematicBody);
	little_button8->pb = App->physics->CreateCircleSensor(160, 300, 20, 0x0003, 0x0002); little_button8->pb->body->SetType(b2_kinematicBody);
	
	
	// -------------------------
}

void j1Map::CreateTimers()
{
	kawaii_blue->timer = new Timer(0.2); timers.add(kawaii_blue->timer);
	kawaii_red->timer = new Timer(0.2); timers.add(kawaii_red->timer);
	kawaii_green->timer = new Timer(0.2); timers.add(kawaii_green->timer);
	kawaii_orange->timer = new Timer(0.2); timers.add(kawaii_orange->timer);
	kawaii_yellow->timer = new Timer(0.2); timers.add(kawaii_yellow->timer);
	kawaii_violet->timer = new Timer(0.2); timers.add(kawaii_violet->timer);
	kawaii_guy->timer = new Timer(0.2); timers.add(kawaii_guy->timer);
	kawaii_girl->timer = new Timer(0.2); timers.add(kawaii_girl->timer);
	kawaii_box1->timer = new Timer(0.2); timers.add(kawaii_box1->timer);
	kawaii_box2->timer = new Timer(0.2); timers.add(kawaii_box2->timer);

	big_bumper1->timer = new Timer(0.3); timers.add(big_bumper1->timer);
	big_bumper2->timer = new Timer(0.3); timers.add(big_bumper2->timer);

	small_bumper1->timer = new Timer(0.3); timers.add(small_bumper1->timer);
	small_bumper2->timer = new Timer(0.3); timers.add(small_bumper2->timer);
	small_bumper3->timer = new Timer(0.3); timers.add(small_bumper3->timer);
	small_bumper4->timer = new Timer(0.3); timers.add(small_bumper4->timer);
	small_bumper5->timer = new Timer(0.3); timers.add(small_bumper5->timer);
	small_bumper6->timer = new Timer(0.3); timers.add(small_bumper6->timer);
	small_bumper7->timer = new Timer(0.3); timers.add(small_bumper7->timer);

	
}

void j1Map::Draw()
{
	// Background --------------
	Blit(bg1->texture, bg1->pos.x, bg1->pos.y, &bg1->rect);
	Blit(bg2->texture, bg2->pos.x, bg2->pos.y, &bg2->rect);

	// Kawaiis
	Blit(spritesheet, kawaii_blue->pos.x, kawaii_blue->pos.y, &kawaii_blue->anim.GetCurrentFrame());
	Blit(spritesheet, kawaii_red->pos.x, kawaii_red->pos.y, &kawaii_red->anim.GetCurrentFrame());
	Blit(spritesheet, kawaii_green->pos.x, kawaii_green->pos.y, &kawaii_green->anim.GetCurrentFrame());
	Blit(spritesheet, kawaii_orange->pos.x, kawaii_orange->pos.y, &kawaii_orange->anim.GetCurrentFrame());
	Blit(spritesheet, kawaii_violet->pos.x, kawaii_violet->pos.y, &kawaii_violet->anim.GetCurrentFrame());
	Blit(spritesheet, kawaii_yellow->pos.x, kawaii_yellow->pos.y, &kawaii_yellow->anim.GetCurrentFrame());
	Blit(spritesheet, kawaii_girl->pos.x, kawaii_girl->pos.y, &kawaii_girl->anim.GetCurrentFrame(), 1, 20);
	Blit(spritesheet, kawaii_guy->pos.x, kawaii_guy->pos.y, &kawaii_guy->anim.GetCurrentFrame(), 1, -20);
	Blit(spritesheet, kawaii_box1->pos.x, kawaii_box1->pos.y, &kawaii_box1->anim.GetCurrentFrame());
	Blit(spritesheet, kawaii_box2->pos.x, kawaii_box2->pos.y, &kawaii_box2->anim.GetCurrentFrame());

	// Big bumpers
	Blit(spritesheet, METERS_TO_PIXELS(big_bumper1->pb->body->GetPosition().x) - 62, METERS_TO_PIXELS(big_bumper1->pb->body->GetPosition().y) - 63, &big_bumper1->anim.GetCurrentFrame());
	Blit(spritesheet, METERS_TO_PIXELS(big_bumper2->pb->body->GetPosition().x) - 62, METERS_TO_PIXELS(big_bumper2->pb->body->GetPosition().y) - 63, &big_bumper2->anim.GetCurrentFrame());

	// Small bumpers
	Blit(spritesheet, METERS_TO_PIXELS(small_bumper1->pb->body->GetPosition().x) - 36, METERS_TO_PIXELS(small_bumper1->pb->body->GetPosition().y) - 38, &small_bumper1->anim.GetCurrentFrame());
	Blit(spritesheet, METERS_TO_PIXELS(small_bumper2->pb->body->GetPosition().x) - 36, METERS_TO_PIXELS(small_bumper2->pb->body->GetPosition().y) - 38, &small_bumper2->anim.GetCurrentFrame());
	Blit(spritesheet, METERS_TO_PIXELS(small_bumper3->pb->body->GetPosition().x) - 36, METERS_TO_PIXELS(small_bumper3->pb->body->GetPosition().y) - 38, &small_bumper3->anim.GetCurrentFrame());
	Blit(spritesheet, METERS_TO_PIXELS(small_bumper4->pb->body->GetPosition().x) - 36, METERS_TO_PIXELS(small_bumper4->pb->body->GetPosition().y) - 38, &small_bumper4->anim.GetCurrentFrame());
	Blit(spritesheet, METERS_TO_PIXELS(small_bumper5->pb->body->GetPosition().x) - 36, METERS_TO_PIXELS(small_bumper5->pb->body->GetPosition().y) - 38, &small_bumper5->anim.GetCurrentFrame());
	Blit(spritesheet, METERS_TO_PIXELS(small_bumper6->pb->body->GetPosition().x) - 36, METERS_TO_PIXELS(small_bumper6->pb->body->GetPosition().y) - 38, &small_bumper6->anim.GetCurrentFrame());
	Blit(spritesheet, METERS_TO_PIXELS(small_bumper7->pb->body->GetPosition().x) - 36, METERS_TO_PIXELS(small_bumper7->pb->body->GetPosition().y) - 38, &small_bumper7->anim.GetCurrentFrame());

	// Small Buttons
	Blit(spritesheet, METERS_TO_PIXELS(little_button1->pb->body->GetPosition().x) - 26, METERS_TO_PIXELS(little_button1->pb->body->GetPosition().y) - 26, &little_button1->anim.GetCurrentFrame());
	Blit(spritesheet, METERS_TO_PIXELS(little_button2->pb->body->GetPosition().x) - 26, METERS_TO_PIXELS(little_button2->pb->body->GetPosition().y) - 26, &little_button2->anim.GetCurrentFrame());
	Blit(spritesheet, METERS_TO_PIXELS(little_button3->pb->body->GetPosition().x) - 26, METERS_TO_PIXELS(little_button3->pb->body->GetPosition().y) - 26, &little_button3->anim.GetCurrentFrame());
	Blit(spritesheet, METERS_TO_PIXELS(little_button4->pb->body->GetPosition().x) - 26, METERS_TO_PIXELS(little_button4->pb->body->GetPosition().y) - 26, &little_button4->anim.GetCurrentFrame());
	Blit(spritesheet, METERS_TO_PIXELS(little_button5->pb->body->GetPosition().x) - 26, METERS_TO_PIXELS(little_button5->pb->body->GetPosition().y) - 26, &little_button5->anim.GetCurrentFrame());
	Blit(spritesheet, METERS_TO_PIXELS(little_button6->pb->body->GetPosition().x) - 26, METERS_TO_PIXELS(little_button6->pb->body->GetPosition().y) - 26, &little_button6->anim.GetCurrentFrame());
	Blit(spritesheet, METERS_TO_PIXELS(little_button7->pb->body->GetPosition().x) - 26, METERS_TO_PIXELS(little_button7->pb->body->GetPosition().y) - 26, &little_button7->anim.GetCurrentFrame());
	Blit(spritesheet, METERS_TO_PIXELS(little_button8->pb->body->GetPosition().x) - 26, METERS_TO_PIXELS(little_button8->pb->body->GetPosition().y) - 26, &little_button8->anim.GetCurrentFrame());

	// Targets
	Blit(target->texture, 140, 610, &target->rect, 1, 45);
	Blit(target2->texture, 102, 425, &target2->rect, 1);
	Blit(target3->texture, 102, 375, &target3->rect, 1);
	Blit(target4->texture, 613, 595, &target4->rect, 1, 180);
	Blit(target5->texture, 613, 373, &target5->rect, 1, 180);
	Blit(target6->texture, 123, 250, &target6->rect, 1, 20);
	Blit(target7->texture, 585, 80, &target7->rect, 1, 130);
	Blit(target8->texture, 420, 45, &target8->rect, 1, 50);
	// -------------------------

	// Spring
	Blit(spring->texture, spring->pos.x, METERS_TO_PIXELS(spring_coll->GetBodyB()->GetPosition().y) -35, &spring->rect);

	// Ball
	Blit(ball->texture, METERS_TO_PIXELS(ball->pb->body->GetPosition().x - 14), METERS_TO_PIXELS(ball->pb->body->GetPosition().y - 14), &ball->rect);

	// Kickers
	Blit(big_left_kicker->texture, 206, 1090, &big_left_kicker->rect, 1, (-big_left_kicker_coll->GetJointAngle() * RADTODEG) + 180, 18, 20);
	Blit(big_right_kicker->texture, 315, 1090, &big_right_kicker->rect, 1, (-big_right_kicker_coll->GetJointAngle() * RADTODEG) + 180, 73, 20);
	Blit(small_left_kicker->texture, 115, 540, &small_left_kicker->rect, 1, (-small_left_kicker_coll->GetJointAngle() * RADTODEG), 13, 15);
	Blit(small_right_kicker->texture, 520, 675, &small_right_kicker->rect, 1, (-small_right_kicker_coll->GetJointAngle() * RADTODEG), 54, 15);
	Blit(small_left_kicker->texture, 400, 242, &small_left_kicker->rect, 1, (-small_left_kicker_coll2->GetJointAngle() * RADTODEG), 13, 15);
	Blit(small_right_kicker->texture, 497, 243, &small_right_kicker->rect, 1, (-small_right_kicker_coll2->GetJointAngle() * RADTODEG), 54, 15);

}

void j1Map::PrintPuntuations()
{
	// Puntuation print -------------------

	// 100
	for (int i = 0; i < print_points1->list100.count();)
	{
		print_points1->list100[i]->UpdateTime();

		if (!print_points1->list100[i]->IsTimeReached())
		{
			print_points1->points_plus100[i].y++;
			App->render->Blit(print_points1->texture100, print_points1->points100[i].x + print_points1->points_plus100[i].x, print_points1->points100[i].y - print_points1->points_plus100[i].y, &print_points1->rect100);
			i++;
		}
		else if (print_points1->list100[i]->IsTimeReached() && !print_points1->list100[i]->stop_time)
		{
			print_points1->list100.del(print_points1->list100.At(i));
			print_points1->points100.del(print_points1->points100.At(i));
			print_points1->points_plus100.del(print_points1->points_plus100.At(i));
		}
	}

	// 500
	for (int i = 0; i < print_points1->list500.count();)
	{
		print_points1->list500[i]->UpdateTime();

		if (!print_points1->list500[i]->IsTimeReached())
		{
			print_points1->points_plus500[i].y++;
			App->render->Blit(print_points1->texture500, print_points1->points500[i].x + print_points1->points_plus500[i].x, print_points1->points500[i].y - print_points1->points_plus500[i].y, &print_points1->rect500);
			i++;
		}
		else if (print_points1->list500[i]->IsTimeReached() && !print_points1->list500[i]->stop_time);
		{
			print_points1->list500.del(print_points1->list500.At(i));
			print_points1->points500.del(print_points1->points500.At(i));
			print_points1->points_plus500.del(print_points1->points_plus500.At(i));
		}
	}

	// ------------------------------------
}


// Called before quitting
bool j1Map::CleanUp()
{
	LOG("Unloading map");

	Unload(spritesheet);
	Unload(ball->texture);
	Unload(ball1->texture);
	Unload(ball2->texture);
	Unload(bg1->texture);
	Unload(bg2->texture);
	Unload(spring->texture);
	Unload(big_left_kicker->texture);
	Unload(big_right_kicker->texture);
	Unload(big_right_kicker->texture);
	Unload(small_left_kicker->texture);
	Unload(small_right_kicker->texture);
	Unload(target->texture);
	Unload(target2->texture);
	Unload(target3->texture);
	Unload(target4->texture);
	Unload(target5->texture);
	Unload(target6->texture);
	Unload(target7->texture);
	Unload(target8->texture);

	return true;
}
void j1Map::ResetAnimations()
{
	for(int i = 0; i<s_s_i.count(); i++)
	{
		s_s_i[i]->ResetFrame();

		if(s_s_i[i]->timer != nullptr)
			s_s_i[i]->ResetTimer();
	}
	App->scene_manager->piyo_bonus = false; //+150.000 points
	App->scene_manager->piyo_bonus_sum = true;
	App->scene_manager->meow_meow_happy_bonus = false; //+200.000 points
	App->scene_manager->meow_meow_happy_bonus_sum = true;
	App->scene_manager->big_pixel_bonus = false; //+200.000 points
	App->scene_manager->big_pixel_bonus_sum = true;
	App->scene_manager->loop_bonus = false;
	App->scene_manager->loop_bonus_sum = true;
}

void j1Map::Blit(SDL_Texture * texture, int x, int y, const SDL_Rect* section, float speed, double angle, int pivot_x, int pivot_y)
{
	App->render->Blit(texture, x, y, section, speed, angle, pivot_x, pivot_y);
}

void j1Map::Unload(SDL_Texture* tex)
{
	App->tex->UnLoad(tex);
}




