#ifndef __j1MENU_H__
#define __j1MENU_H__

#include "j1Module.h"
#include "j1Render.h"

struct SDL_Texture;

class MenuItem
{
public:
	MenuItem()
	{
	}

	MenuItem(SDL_Texture* _texture, int _x, int _y, int _w, int _h, int _posx, int _posy)
	{
		texture = _texture;
		rect.x = _x; rect.y = _y; rect.w = _w; rect.h = _h;
		pos.x = _posx;
		pos.y = _posy;
	}

	SDL_Texture* texture;
	SDL_Rect rect;
	iPoint pos;
};

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
	bool MouseDown();
	bool MouseOver();
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

	void Unload(SDL_Texture* tex);

public:

	MenuItem*		start;
	MenuItem*		button_normal;
	MenuItem*		button_pressed;
	MenuItem*		rotating_background;
	MenuItem*		logo;

	Button*		    start_button;

private:	
	float			degrees = 0;
	SDL_Rect		background;
};

#endif // __j1MENU_H__