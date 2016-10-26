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
	button_normal = new MenuItem(App->tex->Load("menu/play_again_button_blue.png"), 0, 0, 320, 76, 160, 850);
	button_pressed = new MenuItem(App->tex->Load("menu/play_again_button_orange.png"), 0, 0, 320, 76, 160, 850);
	play_again = new Button(160, 470, 320, 80);

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
		p2SString title("%s	  [Balls: %d Score: %d Max Score: %d]", 
			App->title.GetString(), App->map->player.balls, App->map->player.score, App->map->player.max_score);
		App->win->SetTitle(title.GetString());
	}
	else
	{
		p2SString title("%s", App->title.GetString());
		App->win->SetTitle(title.GetString());
	}

	// Activate camera debug
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
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

	// Game over
	if(App->map->player.balls == 0 && App->scene->IsEnabled())
	{
		App->render->DrawQuad(background, 30, 30, 30, 200);
	
		game_over = true;
		if(play_again->MouseOver())
		{
			App->render->Blit(button_pressed->texture, button_pressed->pos.x, button_pressed->pos.y, &button_pressed->rect);
		}
		else
		{
			App->render->Blit(button_normal->texture, button_normal->pos.x, button_normal->pos.y, &button_normal->rect);
		}
		if(play_again->MouseDown())
		{
			game_over = false;
			one_time = true;
			App->map->player.balls = 3;
			loading->Start();
			is_loading = true;
		}
		play_again->Draw();
	}

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
