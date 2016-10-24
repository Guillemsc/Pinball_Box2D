#ifndef __j1MAP_H__
#define __j1MAP_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"
#include "j1Animation.h"
#include "j1App.h"
#include "j1Physics.h"

class App;
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

	class Timer;
	class SpriteSheetItem 
	{
	public:

		SpriteSheetItem()
		{
			pos.x = 0;
			pos.y = 0;
		};

		SpriteSheetItem(int x, int y)
		{
			pos.x = x; 
			pos.y = y;
		};

		~SpriteSheetItem() 
		{};

	public:
		SDL_Texture* texture = App->tex->Load("images/spritesheet.png");
		Animation anim;
		iPoint pos;
		PhysBody* pb = nullptr;
		Timer* timer;

	private:
	};

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

	j1Map();

	// Destructor
	virtual ~j1Map();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	bool Start();

	// Called each loop iteration
	void CreateColliders();
	void Draw();
	void CreateTimers();

	// Called before quitting
	bool CleanUp();

	void Blit(SDL_Texture * texture, int x, int y, const SDL_Rect* section, float speed = 1.0f, double angle = 0.0f, int pivot_x = 0, int pivot_y = 0);
private:

public:

	// Sprites
	Sprite*					bg1;
	Sprite*					bg2;
	Sprite*					spring;	
	Sprite*					ball;
	Sprite*					big_left_kicker;
	Sprite*					big_right_kicker;
	Sprite*					small_left_kicker;
	Sprite*					small_right_kicker;

	SpriteSheetItem*		kawaii_blue;
	SpriteSheetItem*		kawaii_green;
	SpriteSheetItem*		kawaii_orange;
	SpriteSheetItem*		kawaii_red;
	SpriteSheetItem*		kawaii_violet;
	SpriteSheetItem*		kawaii_yellow;
	SpriteSheetItem*		kawaii_girl;
	SpriteSheetItem*		kawaii_guy;
	SpriteSheetItem*		little_button;
	SpriteSheetItem*		small_bumper;
	SpriteSheetItem*		big_bumper1;
	SpriteSheetItem*		big_bumper2;
	SpriteSheetItem*	    blue_arrow;

	// Kickers
	b2RevoluteJoint*		big_left_kicker_coll;
	b2RevoluteJoint*		big_right_kicker_coll;
	b2RevoluteJoint*		small_left_kicker_coll;
	b2RevoluteJoint*		small_right_kicker_coll;

	// Spring
	b2PrismaticJoint*		spring_coll;

	// Music
	uint					kicker1_fx;
	uint					kicker2_fx;
	uint					spring_pull_fx;
	uint					spring_push_fx;
	uint					ball_hit_fx;
	uint					kawaii_sound1_fx;
	uint					kawaii_sound2_fx;

	// Lists
	p2List<PhysBody*>		balls;
	p2List<Timer*>			timers;

};

#endif // __j1MAP_H__