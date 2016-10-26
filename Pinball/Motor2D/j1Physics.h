#ifndef __j1PHYSICS_H__
#define __j1PHYSICS_H__

#include "j1Module.h"
#include "Box2D/Box2D/Box2D.h"

#define PIXELS_PER_METER 50.0f // if touched change METER_PER_PIXEL too
#define METER_PER_PIXEL 0.02f // this is 1 / PIXELS_PER_METER !

#define METERS_TO_PIXELS(m) ((int) floor(PIXELS_PER_METER * m))
#define PIXEL_TO_METERS(p)  ((float) METER_PER_PIXEL * p)

#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f

struct SDL_Texture;

enum collider_names
{
	ball,
	kawaii_blue,
	kawaii_red,
	kawaii_violet,
	kawaii_green,
	kawaii_orange,
	kawaii_yellow,
	kawaii_girl,
	kawaii_guy,
	kawaii_box1,
	kawaii_box2,
	big_bumper_1,
	big_bumper_2,
	target,
	target2,
	target3,
	target4,
	target5,
	target6,
	target7, 
	target8,
	small_bumper_1,
	small_bumper_2,
	small_bumper_3,
	small_bumper_4,
	small_bumper_5,
	small_bumper_6,
	small_bumper_7,

	ball_dead_detector,
	loop_detector,

	wall,
	slingshot
};

class PhysBody
{
public:
	PhysBody() : listener(NULL), body(NULL)
	{}

	void GetPosition(int& x, int &y) const;
	float GetRotation() const;
	bool Contains(int x, int y) const;
	int RayCast(int x1, int y1, int x2, int y2, float& normal_x, float& normal_y) const;

public:
	int width, height;
	b2Body* body;
	j1Module* listener;
	collider_names coll_name;
};


class j1Physics : public j1Module, public b2ContactListener
{
public:

	j1Physics();

	// Destructor
	~j1Physics();

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

	PhysBody* CreateCircle(int x, int y, int radius, float restitution, uint16 mask, uint16 category);
	PhysBody* CreateCircleSensor(int x, int y, int radius, uint16 mask, uint16 category);
	PhysBody* CreateRectangle(int x, int y, int width, int height, uint16 mask = 0x0001, uint16 category = 0x0001);
	PhysBody* CreateRectangleSensor(int x, int y, int width, int height, uint16 mask, uint16 category);
	PhysBody* CreateChain(int x, int y, int* points, int size, uint16 mask, uint16 category, float restitution = 0.0f);
	b2PrismaticJoint* CreatePrismaticJoint(PhysBody* bodyA, PhysBody* bodyB, b2Vec2 ancorA, b2Vec2 ancorB, int max, int min, int maxMotor, int motorSpeed);
	b2RevoluteJoint* CreateRevoluteJoint(int radius, int* vects, int size, int posx, int posy, int desplacementx, int desplacementy, int upper_angle, int lower_angle, int max_torque, int speed, uint16 mask, uint16 category);
	void TimerActions();

	void SetGround(b2Body* body) 
	{
		ground = body;
	}

	// b2ContactListener ---
	void BeginContact(b2Contact* contact);

	virtual void OnCollision(PhysBody* bodyA, PhysBody* bodyB);

private:
	bool debug;
	b2MouseJoint* mouse_joint;
	b2Body* ground;
	b2Body* selected = nullptr;

public:

	// Time
	Uint32 start_time;

	// World
	b2World* world;
};

#endif // __j1PHYSICS_H__