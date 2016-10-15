#ifndef __j1MAP_H__
#define __j1MAP_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"

class j1Map : public j1Module
{
public:

	j1Map();

	// Destructor
	virtual ~j1Map();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	bool Start();

	// Called each loop iteration
	void Draw();

	// Called before quitting
	bool CleanUp();

private:
	SDL_Texture*		background1;
	SDL_Rect			background1_rect;
};

#endif // __j1MAP_H__