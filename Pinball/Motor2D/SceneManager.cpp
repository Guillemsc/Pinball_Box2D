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

bool j1SceneManager::Load(pugi::xml_node &data)
{
	App->map->player.max_score = data.attribute("max_score").as_int();
	
	return true;
}

bool j1SceneManager::Save(pugi::xml_node &data) const
{
	if (data.attribute("max_score").as_int() < App->map->player.max_score)
		data.append_attribute("max_score") = App->map->player.max_score;
		
	return true;
}

// Called before the first frame
bool j1SceneManager::Start()
{
	//App->LoadGame("save_game.xml");

	one_time = true;
	loading = new Timer(1.5);
	game_over = false;
	is_loading = false;;
	App->map->player.balls = 3;
	App->map->player.score = 0;
	App->map->player.max_score = 0;


	background.x = 0;
	background.y = -App->render->camera.y;
	background.w = 1000;
	background.h = 1000;

	loading_image = new MenuItem(App->tex->Load("menu/loading.png"), 0, 0, 224, 34, 215, 750);
	button_normal = new MenuItem(App->tex->Load("menu/play_again_button_blue.png"), 0, 0, 320, 76, 160, 850);
	button_pressed = new MenuItem(App->tex->Load("menu/play_again_button_orange.png"), 0, 0, 320, 76, 160, 850);
	game_over_text = new MenuItem(App->tex->Load("menu/game_over.png"), 0, 0, 620, 152, 0, 700);
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
	if (timer->IsTimeReached() && !timer->stop_time) {
		piyo_bonus = false;
		timer->stop_time = true;
	}
	if (timer2->IsTimeReached() && !timer2->stop_time) {
		meow_meow_happy_bonus = false;
		timer2->stop_time = true;
	}
	if (timer3->IsTimeReached() && !timer3->stop_time) {
		big_pixel_bonus = false;
		timer3->stop_time = true;
	}
	if (timer4->IsTimeReached() && !timer4->stop_time) {
		loop_bonus = false;
		timer4->stop_time = true;
	}
	// Title and puntuation
	if (App->scene->active)
	{
		p2SString title("%s	  [Balls: %d Score: %d Max Score: %d]", 
			App->title.GetString(), App->map->player.balls, App->map->player.score, App->map->player.max_score);
		App->win->SetTitle(title.GetString());
		if (piyo_bonus) {
			timer->UpdateTime();
			p2SString title2("\t\tPIYO BONUS +150000");
			if (piyo_bonus_sum) { // So it sums once
				timer->Start();
				App->map->player.score += 150000;
				piyo_bonus_sum = false;
			}
			App->win->SetTitle(title2.GetString());
		}
		if (meow_meow_happy_bonus) {
			timer2->UpdateTime();
			p2SString title3("\t\tMEOW MEOW HAPPY BONUS +200000");
			if (meow_meow_happy_bonus_sum) { // So it sums once
				timer2->Start();
				App->map->player.score += 200000;
				meow_meow_happy_bonus_sum = false;
			}
			App->win->SetTitle(title3.GetString());
		}
		if (big_pixel_bonus) {
			timer3->UpdateTime();
			p2SString title4("\t\tBIG PIXEL BONUS +200000");
			if (big_pixel_bonus_sum) { // So it sums once
				timer3->Start();
				App->map->player.score += 200000;
				big_pixel_bonus_sum = false;
			}
			App->win->SetTitle(title4.GetString());
		}
		if (loop_bonus) {
			timer4->UpdateTime();
			p2SString title5("\t\tLOOP BONUS +50000");
			if (loop_bonus_sum) { // So it sums once
				timer4->Start();
				App->audio->PlayFx(App->map->loop_bonus_fx);
				App->map->player.score += 50000;
				loop_bonus_sum = false;
			}
			App->win->SetTitle(title5.GetString());
		}
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

	// Game over ---------------
	if(App->map->player.balls == 0 && App->scene->IsEnabled() && !is_loading)
	{
		App->render->DrawQuad(background, 30, 30, 30, 200);
	
		// Max Score update
		if (App->map->player.score > App->map->player.max_score)
		{
			App->map->player.max_score = App->map->player.score;
		}

		game_over = true;

		// Button control
		if(play_again->MouseOver() && game_over)
		{
			App->render->Blit(button_pressed->texture, button_pressed->pos.x, button_pressed->pos.y, &button_pressed->rect);
		}
		else
		{
			App->render->Blit(button_normal->texture, button_normal->pos.x, button_normal->pos.y, &button_normal->rect);
		}

		App->render->Blit(game_over_text->texture, game_over_text->pos.x, game_over_text->pos.y, &game_over_text->rect);

		// Play again
		if(play_again->MouseDown() && game_over)
		{
			game_over = false;
			one_time = true;
			App->map->ResetAnimations();
			App->map->player.balls = 3;
			App->map->player.score = 0;
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
	LOG("Freeing scene manager");

	Unload(loading_image->texture);
	Unload(button_normal->texture);
	Unload(button_pressed->texture);
	Unload(game_over_text->texture);

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

void j1SceneManager::Unload(SDL_Texture * tex)
{
	App->tex->UnLoad(tex);
}
