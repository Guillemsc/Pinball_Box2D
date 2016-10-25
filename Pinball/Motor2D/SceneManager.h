#ifndef __j1SCENEMANAGER_H__
#define __j1SCENEMANAGER_H__

#include "j1Module.h"

struct SDL_Texture;

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

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	void LoadScene();

public:
	bool camera_debug = false;

private:
};

#endif // __j1SCENEMANAGER_H__