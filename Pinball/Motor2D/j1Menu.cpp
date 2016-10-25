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
	menu = App->tex->Load("menu/Play.png");
	menu_rect.x = 0;
	menu_rect.y = 0;
	menu_rect.w = 1180;
	menu_rect.h = 746;

	start = new Button(250, 500, 150, 50);

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
	App->render->Blit(menu, -270, 390, &menu_rect);
	start->Draw();

	if (start->CheckMouse())
	{
		App->scene_manager->LoadScene();
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
	App->render->DrawQuad(rect, 100, 255, 100, 100, false);
}

bool Button::CheckMouse()
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
