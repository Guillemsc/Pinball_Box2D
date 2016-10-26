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

	// Lists
	p2List<PhysBody*>		balls;

public:
	struct Player
	{
		uint score;
		uint balls;
		uint max_score;
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

	class PrintPoints
	{
	public:
		PrintPoints(SDL_Texture* _texture100, int x, int y, int w, int h, SDL_Texture* _texture500, int x2, int y2, int w2, int h2, int _ball_index)
		{
			texture100 = _texture100;
			texture500 = _texture500;
			ball_index = _ball_index;

			rect100.x = x; rect100.y = y; rect100.w = w; rect100.h = h;
			rect500.x = x2; rect500.y = y2; rect500.w = w2; rect500.h = h2;
		}

		void New100()
		{
			Timer* t = new Timer(1.5);
			list100.add(t);
			list100[list100.count() - 1]->Start();

			iPoint print;
			print.x = METERS_TO_PIXELS(balls[ball_index - 1]->body->GetPosition().x);
			print.y = METERS_TO_PIXELS(balls[ball_index - 1]->body->GetPosition().y);
			points100.add(print);

			iPoint plus;
			plus.x = 0;
			plus.y = 0;
			points_plus100.add(plus);
		}

		void New500()
		{
			Timer* t = new Timer(2);
			list500.add(t);
			list500[list500.count() - 1]->Start();

			iPoint print;
			print.x = METERS_TO_PIXELS(balls[ball_index - 1]->body->GetPosition().x);
			print.y = METERS_TO_PIXELS(balls[ball_index - 1]->body->GetPosition().y);
			points500.add(print);

			iPoint plus;
			plus.x = 0;
			plus.y = 0;
			points_plus500.add(plus);
		}

	public:
		SDL_Texture* texture100 = nullptr;
		SDL_Rect rect100;
		SDL_Texture* texture500 = nullptr;
		SDL_Rect rect500;

		p2List<Timer*> list100;
		p2List<iPoint> points100;
		p2List<iPoint> points_plus100;

		p2List<Timer*> list500;
		p2List<iPoint> points500;
		p2List<iPoint> points_plus500;

		int ball_index;
		p2List<PhysBody*> balls;
	};

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
		bool collide_once = true;
		bool collide_for_bonus = false;

	private:
	};

	j1Map();

	// Destructor
	virtual ~j1Map();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	bool Start();

	// Functions
	void CreateColliders();
	void Draw();
	void CreateTimers();
	void PrintPuntuations();

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

	// Targets
	Sprite*					target;
	Sprite*					target2;
	Sprite*					target3;
	Sprite*					target4;
	Sprite*					target5;
	Sprite*					target6;
	Sprite*					target7;
	Sprite*					target8;

	SpriteSheetItem*		kawaii_blue;
	SpriteSheetItem*		kawaii_green;
	SpriteSheetItem*		kawaii_orange;
	SpriteSheetItem*		kawaii_red;
	SpriteSheetItem*		kawaii_violet;
	SpriteSheetItem*		kawaii_yellow;
	SpriteSheetItem*		kawaii_girl;
	SpriteSheetItem*		kawaii_guy;
	SpriteSheetItem*		kawaii_box1;
	SpriteSheetItem*		kawaii_box2;
	SpriteSheetItem*		little_button1;
	SpriteSheetItem*		little_button2;
	SpriteSheetItem*		little_button3;
	SpriteSheetItem*		little_button4;
	SpriteSheetItem*		little_button5;
	SpriteSheetItem*		little_button6;
	SpriteSheetItem*		little_button7;
	SpriteSheetItem*		little_button8;
	SpriteSheetItem*		small_bumper1;
	SpriteSheetItem*		small_bumper2;
	SpriteSheetItem*		small_bumper3;
	SpriteSheetItem*		small_bumper4;
	SpriteSheetItem*		small_bumper5;
	SpriteSheetItem*		small_bumper6;
	SpriteSheetItem*		small_bumper7;
	SpriteSheetItem*		big_bumper1;
	SpriteSheetItem*		big_bumper2;
	SpriteSheetItem*	    blue_arrow;

	// Kickers
	b2RevoluteJoint*		big_left_kicker_coll;
	b2RevoluteJoint*		big_right_kicker_coll;
	b2RevoluteJoint*		small_left_kicker_coll;
	b2RevoluteJoint*		small_right_kicker_coll;
	b2RevoluteJoint*		small_left_kicker_coll2;
	b2RevoluteJoint*		small_right_kicker_coll2;

	// Spring
	b2PrismaticJoint*		spring_coll;

	// Slingshot
	PhysBody* collider_slingshot1;
	PhysBody* collider_slingshot2;

	// Standalone detectors
	PhysBody*				ball_dead_detector = nullptr;
	PhysBody*				loop_detector = nullptr;

	// Music
	uint					kicker1_fx;
	uint					kicker2_fx;
	uint					spring_pull_fx;
	uint					spring_push_fx;
	uint					ball_hit_fx;
	uint					ball_lost_fx;
	uint					kawaii_sound1_fx;
	uint					kawaii_sound2_fx;
	uint					kawaii_sound3_fx;
	uint					kawaii_sound4_fx;
	uint					big_pixel1_fx;
	uint					big_pixel2_fx;
	uint					small_bumper_fx;
	uint					big_bumper_fx;
	uint					multiball_release_fx;
	uint					multiball_fx;
	uint					target_fx;
	uint					slingshot_fx;
	uint					piyo_bonus_fx;
	uint					meow_meow_happy_bonus_fx;
	uint					big_pixel_bonus_fx;
	uint					loop_bonus_fx;

	// Lists
	p2List<Timer*>			timers;

	// Player
	Player player;

	PrintPoints* print_points1;
};

#endif // __j1MAP_H__