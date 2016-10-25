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
#include "j1Menu.h"
#include "j1Scene.h"

j1Menu::j1Menu() : j1Module()
{
	name.create("scene");
}

// Destructor
j1Menu::~j1Menu()
{}

// Called before render is available
bool j1Menu::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool j1Menu::Start()
{

	return true;
}

// Called each loop iteration
bool j1Menu::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool j1Menu::Update(float dt)
{
	/*
	if ((App->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN))
	{
		App->physics->Enable();
		App->physics->active = true;
		App->scene->Enable();
		App->scene->active = true;
	}
	if ((App->input->GetKey(SDL_SCANCODE_O) == KEY_DOWN))
	{
		App->scene->Disable();
		App->scene->active = false;
		App->physics->Disable();
		App->physics->active = false;
	}
	*/
	return true;
}


// Called each loop iteration
bool j1Menu::PostUpdate()
{
	bool ret = true;

	return ret;
}

// Called before quitting
bool j1Menu::CleanUp()
{
	LOG("Freeing scene");

	return true;
}