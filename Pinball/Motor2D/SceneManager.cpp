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
#include "SceneManager.h"

j1SceneManager::j1SceneManager() : j1Module()
{
	name.create("scene_manager");
}

// Destructor
j1SceneManager::~j1SceneManager()
{}

// Called before render is available
bool j1SceneManager::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool j1SceneManager::Start()
{
	one_time = true;
	loading = new Timer(1.5);

	background.x = 0;
	background.y = -App->render->camera.y;
	background.w = 1000;
	background.h = 1000;

	loading_image = new MenuItem(App->tex->Load("menu/loading.png"), 0, 0, 224, 34, 215, 750);

	return true;
}

// Called each loop iteration
bool j1SceneManager::PreUpdate()
{
	loading->UpdateTime();

	return true;
}

// Called each loop iteration
bool j1SceneManager::Update(float dt)
{
	// Title and puntuation
	if (App->scene->active)
	{
		p2SString title("%s	  [Balls: %d Points: %d MaxPuntuation: %d]", 
			App->title.GetString(), App->map->player.balls, App->map->player.points, App->map->player.max_points);
		App->win->SetTitle(title.GetString());
	}
	else
	{
		p2SString title("%s", App->title.GetString());
		App->win->SetTitle(title.GetString());
	}

	// Activate camera debug
	if (App->input->GetKey(SDL_SCANCODE_G) == KEY_DOWN)
		camera_debug = !camera_debug;

	// Loading timer
	if (!loading->IsTimeReached())
	{
		is_loading = true;
		App->render->DrawQuad(background, 30, 30, 30, 255);
		App->render->Blit(loading_image->texture, loading_image->pos.x, loading_image->pos.y, &loading_image->rect);

		if (one_time)
		{
			LoadScene();
			one_time = false;
		}
	}
	else
		is_loading = false;

	return true;
}


// Called each loop iteration
bool j1SceneManager::PostUpdate()
{
	bool ret = true;

	return ret;
}

// Called before quitting
bool j1SceneManager::CleanUp()
{
	LOG("Freeing scene");

	return true;
}

void j1SceneManager::LoadScene()
{
		App->physics->Enable();
		App->physics->active = true;
		App->scene->Enable();
		App->scene->active = true;

		App->menu->Disable();
		App->menu->active = false;
/*
		App->scene->Disable();
		App->scene->active = false;
		App->physics->Disable();
		App->physics->active = false;
*/
}
