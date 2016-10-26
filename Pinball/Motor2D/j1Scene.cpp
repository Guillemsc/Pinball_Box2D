#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "j1Physics.h"
#include "SceneManager.h"

j1Scene::j1Scene() : j1Module()
{
	name.create("scene");
}

// Destructor
j1Scene::~j1Scene()
{}

// Called before render is available
bool j1Scene::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{

	one_start = true;

	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{

	if (one_start)
	{
		// Load game ------------------

		App->LoadGame("save_game.xml");

		// ----------------------------

		// Create colliders -----------

		App->map->CreateColliders();

		// ----------------------------

		App->map->player.score = 0;
		App->map->player.balls = 3;

		one_start = false;
	}

	// Camera ---------------------------------------------------------------------

	// Scroll camera
	if (!App->scene_manager->camera_debug)
		ScrollCamera();

	// Camera debug
	if (App->scene_manager->camera_debug)
	{
		if ((App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT))
			App->render->camera.y += 5;

		if ((App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT))
			App->render->camera.y -= 5;

		if ((App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT))
			App->render->camera.x += 5;

		if ((App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT))
			App->render->camera.x -= 5;
	}

	// ----------------------------------------------------------------------------

	// Kickers --------------------------------------------------------------------


	// Left kickers
	if ((App->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN && !App->scene_manager->is_loading && !App->scene_manager->game_over)) {
		App->map->big_left_kicker_coll->SetMotorSpeed(400);
		App->map->small_left_kicker_coll->SetMotorSpeed(200);
		App->map->small_left_kicker_coll2->SetMotorSpeed(200);
		App->audio->PlayFx(App->map->kicker1_fx);
	}
	else if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && !App->scene_manager->is_loading && !App->scene_manager->game_over) {
		App->map->big_left_kicker_coll->SetMotorSpeed(400);
		App->map->small_left_kicker_coll->SetMotorSpeed(200);
		App->map->small_left_kicker_coll2->SetMotorSpeed(200);
	}
	else if ((App->input->GetKey(SDL_SCANCODE_A) == KEY_UP) && !App->scene_manager->is_loading && !App->scene_manager->game_over) {
		App->audio->PlayFx(App->map->kicker2_fx);
	}
	else {
		App->map->big_left_kicker_coll->SetMotorSpeed(-200);
		App->map->small_left_kicker_coll->SetMotorSpeed(-10);
		App->map->small_left_kicker_coll2->SetMotorSpeed(-10);
	}

	// Right kickers
	if ((App->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN) && !App->scene_manager->is_loading && !App->scene_manager->game_over) {
		App->map->big_right_kicker_coll->SetMotorSpeed(-400);
		App->map->small_right_kicker_coll->SetMotorSpeed(-200);
		App->map->small_right_kicker_coll2->SetMotorSpeed(-200);
		App->audio->PlayFx(App->map->kicker1_fx);
	}
	else if ((App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) && !App->scene_manager->is_loading && !App->scene_manager->game_over) {
		App->map->big_right_kicker_coll->SetMotorSpeed(-400);
		App->map->small_right_kicker_coll->SetMotorSpeed(-200);
		App->map->small_right_kicker_coll2->SetMotorSpeed(-200);
	}
	else if ((App->input->GetKey(SDL_SCANCODE_D) == KEY_UP) && !App->scene_manager->is_loading && !App->scene_manager->game_over) {
		App->audio->PlayFx(App->map->kicker2_fx);
	}
	else {
		App->map->big_right_kicker_coll->SetMotorSpeed(200);
		App->map->small_right_kicker_coll->SetMotorSpeed(10);
		App->map->small_right_kicker_coll2->SetMotorSpeed(10);
	}
	// ----------------------------------------------------------------------------

	// Spring ----------

	if ((App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN) && !App->scene_manager->is_loading && !App->scene_manager->game_over) {
		App->audio->PlayFx(App->map->spring_pull_fx);
		App->map->spring_coll->SetMotorSpeed(2);
	}
	else if ((App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) && !App->scene_manager->is_loading && !App->scene_manager->game_over)
		App->map->spring_coll->SetMotorSpeed(6);
	else if ((App->input->GetKey(SDL_SCANCODE_S) == KEY_UP) && !App->scene_manager->is_loading && !App->scene_manager->game_over) {
		App->audio->PlayFx(App->map->spring_push_fx);
	}
	else {
		App->map->spring_coll->SetMotorSpeed(-500);
	}

	// ----------------

	// Create a new ball
	if ((App->input->GetKey(SDL_SCANCODE_N) == KEY_DOWN) && App->scene_manager->camera_debug)
	{
		resetball = true;
	}

	// Draw -----------

	App->map->Draw();

	// ----------------

	ResetBall(resetball);

	return true;
}


// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;

	if(App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}

void j1Scene::ScrollCamera()
{
	App->render->camera.x = 0;

	if (-App->map->ball->pb->body->GetPosition().y > PIXEL_TO_METERS(-590) )
	{
		if(-App->map->ball->pb->body->GetPosition().y < PIXEL_TO_METERS(-200))
		{
			App->render->camera.y = METERS_TO_PIXELS(-App->map->ball->pb->body->GetPosition().y) + 205;
		}
	}
}

void j1Scene::ResetBall(bool& reset)
{
	if (reset) 
	{
		App->audio->PlayFx(App->map->ball_lost_fx);
		App->map->ball->pb->body->SetTransform(b2Vec2(PIXEL_TO_METERS(598), PIXEL_TO_METERS(990)), 0);
		App->map->ball->pb->body->SetAngularVelocity(0);
		reset = false;
	}
}


