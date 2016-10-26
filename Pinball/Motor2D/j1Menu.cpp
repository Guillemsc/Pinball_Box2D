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

j1Menu::j1Menu() : j1Module()
{
	name.create("menu");
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
	start = new MenuItem(App->tex->Load("menu/Play.png"), 0, 0, 1180, 746, -270, 390);
	button_normal = new MenuItem(App->tex->Load("menu/play_normal.png"), 0, 0, 164, 73, 238, 863);
	button_pressed = new MenuItem(App->tex->Load("menu/play_pressed.png"), 0, 0, 164, 73, 238, 863);
	rotating_background = new MenuItem(App->tex->Load("menu/rotating_background.png"), 0, 0, 1726, 971, -550, 300);
	logo = new MenuItem(App->tex->Load("menu/logo.png"), 0, 0, 350, 218, 140, 600);

	background.x = 0;
	background.y = -App->render->camera.y;
	background.w = 1000;
	background.h = 1000;


	start_button = new Button(240, 480, 160, 80);

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
	// Print background
	App->render->DrawQuad(background, 255, 255, 255, 255);
	degrees++;
	App->render->Blit(rotating_background->texture, rotating_background->pos.x, rotating_background->pos.y, &rotating_background->rect, 1.0f, (degrees * 0.5f));
	App->render->Blit(logo->texture, logo->pos.x, logo->pos.y, &logo->rect);

	start_button->Draw();

	// Button options
	if (start_button->MouseDown())
	{
		// Start timer
		App->scene_manager->loading->Start();
	}

	if (start_button->MouseOver())
	{
		App->render->Blit(button_pressed->texture, button_pressed->pos.x, button_pressed->pos.y, &button_pressed->rect);
	}
	else
	{
		App->render->Blit(button_normal->texture, button_normal->pos.x, button_normal->pos.y, &button_normal->rect);
	}

	// Debug camera
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

		if((App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN))
			App->scene_manager->loading->Start();
	}

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

void Button::Draw()
{
	if(App->scene_manager->camera_debug)
		App->render->DrawQuad(rect, 255, 0, 0, 1000, false);
}

bool Button::MouseDown()
{
	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
	{
		int mouse_x, mouse_y;
		App->input->GetMousePosition(mouse_x, mouse_y);
		mouse_x -= App->render->camera.x;
		mouse_y -= App->render->camera.y;

		if(mouse_x > rect.x && mouse_x < rect.x + rect.w)
		{
			if (mouse_y > rect.y && mouse_y < rect.y + rect.h)
			{
				return true;
			}
		}
	}
	return false;
}

bool Button::MouseOver()
{
	int mouse_x, mouse_y;
	App->input->GetMousePosition(mouse_x, mouse_y);
	mouse_x -= App->render->camera.x;
	mouse_y -= App->render->camera.y;

	if (mouse_x > rect.x && mouse_x < rect.x + rect.w)
	{
		if (mouse_y > rect.y && mouse_y < rect.y + rect.h)
		{
			return true;
		}
	}
	return false;
}


