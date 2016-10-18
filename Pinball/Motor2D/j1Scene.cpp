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
	App->map->CreateColliders();

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
	if (App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
		App->LoadGame("save_game.xml");

	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
		App->SaveGame("save_game.xml");

	if (App->input->GetKey(SDL_SCANCODE_G) == KEY_DOWN)
		camera_debug = !camera_debug;

	if (camera_debug) 
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

	// Left kicker
	if ((App->input->GetKey(SDL_SCANCODE_Q) == KEY_DOWN))
		App->map->left_kicker->SetMotorSpeed(300);
	else if ((App->input->GetKey(SDL_SCANCODE_Q) == KEY_REPEAT))
		App->map->left_kicker->SetMotorSpeed(300);
	else
		App->map->left_kicker->SetMotorSpeed(-200);

	// Right kicker
	if ((App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN))
		App->map->right_kicker->SetMotorSpeed(-300);
	else if ((App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT))
		App->map->right_kicker->SetMotorSpeed(-300);
	else
		App->map->right_kicker->SetMotorSpeed(200);

	App->map->Draw();

	if(!camera_debug)
		ScrollCamera();

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
