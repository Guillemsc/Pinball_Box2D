#ifndef __j1MAP_H__
#define __j1MAP_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"

class j1Map : public j1Module
{
public:

	class Sprite
	{
	public:
		Sprite()
		{
		}

		Sprite(SDL_Texture* _texture, int _x, int _y, int _w, int _h, int _posx, int _posy)
		{
			texture = _texture;
			rect.x = _x; rect.y = _y; rect.w = _w; rect.h = _h;
			pos.x = _posx;
			pos.y = _posy;
		}

		Sprite(SDL_Texture* _texture, int _x, int _y, int _w, int _h)
		{
			texture = _texture;
			rect.x = _x; rect.y = _y; rect.w = _w; rect.h = _h;
		}

		Sprite(SDL_Texture* _texture, SDL_Rect _rect, int _posx, int _posy)
		{
			texture = _texture;
			rect = _rect;
			pos.x = _posx;
			pos.y = _posy;
		}

		~Sprite()
		{}

		SDL_Texture* texture;
		SDL_Rect rect;
		iPoint pos;
		PhysBody* pb = nullptr;

	private:
	};

	j1Map();

	// Destructor
	virtual ~j1Map();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	bool Start();

	// Called each loop iteration
	void CreateColliders();
	void Draw();

	// Called before quitting
	bool CleanUp();

	void Blit(SDL_Texture * texture, int x, int y, const SDL_Rect* section, float speed = 1.0f, double angle = 0.0f, int pivot_x = 0, int pivot_y = 0);

public:
	Sprite*		 bg1;
	Sprite*		 bg2;
	Sprite*		 spring;	
	Sprite*		 ball;
	Sprite*		 left_kicker;
	Sprite*		 right_kicker;

	// Kickers
	b2RevoluteJoint* left_kicker_coll;
	b2RevoluteJoint* right_kicker_coll;

	// Spring
	b2PrismaticJoint* spring_coll;

	p2List<PhysBody*> balls;


};

#endif // __j1MAP_H__