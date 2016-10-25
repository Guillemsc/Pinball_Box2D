#ifndef __j1MENU_H__
#define __j1MENU_H__

#include "j1Module.h"
#include "j1Render.h"

struct SDL_Texture;

class Button
{
public:
	Button(int _x, int _y, int _w, int _h)
	{
		rect.x = _x;
		rect.y = _y - App->render->camera.y;
		rect.w = _w;
		rect.h = _h;
	};

	SDL_Rect rect;

	void Draw();
	bool CheckMouse();
};

class j1Menu : public j1Module
{
public:

	j1Menu();

	// Destructor
	virtual ~j1Menu();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

public:

	SDL_Texture* menu;
	SDL_Rect menu_rect;

	Button* start;

private:
};

#endif // __j1MENU_H__