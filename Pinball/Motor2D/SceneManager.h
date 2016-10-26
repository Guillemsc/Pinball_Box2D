#ifndef __j1SCENEMANAGER_H__
#define __j1SCENEMANAGER_H__

#include "j1Module.h"
#include "j1Menu.h"

struct SDL_Texture;

class MenuItem;
class Timer
{
public:
	Timer(float _time_to_wait) : time_to_wait(_time_to_wait * 0.5f * 1000.0f)
	{
		now = 0.0f;
		stop_time = true;
		actual = 0.0f;
	}

	~Timer()
	{};


	void UpdateTime()
	{
		if (!stop_time)
			now = SDL_GetTicks();
		else
			now = time_to_wait + 10;
	}

	bool IsTimeReached()
	{
		if (now - actual >= time_to_wait)
			return true;
		return false;
	}

	void Reset()
	{
		stop_time = true;
	}

	void Start()
	{
		stop_time = false;
		actual = SDL_GetTicks();
	}

private:

public:

public:
	float now;
	float time_to_wait;
	float actual;
	bool stop_time;
};

class j1SceneManager : public j1Module
{
public:

	j1SceneManager();

	// Destructor
	virtual ~j1SceneManager();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Load / Save
	bool Load(pugi::xml_node& node);
	bool Save(pugi::xml_node& node) const;

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	void LoadScene();

public:
	bool			camera_debug = true;
	Timer*			loading;
	bool			is_loading;
	bool			game_over;

private:
	bool			one_time;
	SDL_Rect		background;

	MenuItem*		loading_image;
	MenuItem*		button_normal;
	MenuItem*		button_pressed;
	MenuItem*		game_over_text;
	Button*			play_again;
};

#endif // __j1SCENEMANAGER_H__