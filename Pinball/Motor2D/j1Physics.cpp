#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Physics.h"
#include "j1Scene.h"
#include "j1Map.h"


#define GRAVITY_X 0.0f
#define GRAVITY_Y -10

#ifdef _DEBUG
#pragma comment( lib, "Box2D/libx86/Debug/Box2D.lib" )
#else
#pragma comment( lib, "Box2D/libx86/Release/Box2D.lib" )
#endif

j1Physics::j1Physics() : j1Module()
{
	name.create("physics");

	world = NULL;
	mouse_joint = NULL;
	debug = true;

}

// Destructor
j1Physics::~j1Physics()
{
}

// Called before render is available
bool j1Physics::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool j1Physics::Start()
{
	LOG("Creating Physics 2D environment");

	start_time = 0;

	world = new b2World(b2Vec2(GRAVITY_X, -GRAVITY_Y));
	world->SetContactListener(this);

	b2BodyDef bd;
	ground = world->CreateBody(&bd);

	// Create timers ----------------

	App->map->CreateTimers();

	// ------------------------------

	return true;
}

// Called each loop iteration
bool j1Physics::PreUpdate()
{
	world->Step(1.0f / 60.0f, 128, 128);

	for (b2Contact* c = world->GetContactList(); c; c = c->GetNext())
	{
		if (c->GetFixtureA()->IsSensor() && c->IsTouching())
		{
			PhysBody* pb1 = (PhysBody*)c->GetFixtureA()->GetBody()->GetUserData();
			PhysBody* pb2 = (PhysBody*)c->GetFixtureA()->GetBody()->GetUserData();
			if (pb1 && pb2 && pb1->listener)
				pb1->listener->OnCollision(pb1, pb2);
		}
	}

	return true;
}

// Called each loop iteration
bool j1Physics::Update(float dt)
{
	for (int i = 0; i < App->map->timers.count(); i++)
	{
		App->map->timers[i]->UpdateTime();
	}
	
	TimerActions();

	return true;
}

// Called each loop iteration
bool j1Physics::PostUpdate()
{
	bool ret = true;

	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	if (!debug)
		return true;

	// Bonus code: this will iterate all objects in the world and draw the circles
	// You need to provide your own macro to translate meters to pixels
	for (b2Body* b = world->GetBodyList(); b; b = b->GetNext())
	{
		for (b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext())
		{
			switch (f->GetType())
			{
				// Draw circles ------------------------------------------------
			case b2Shape::e_circle:
			{
				b2CircleShape* shape = (b2CircleShape*)f->GetShape();
				b2Vec2 pos = f->GetBody()->GetPosition();
				App->render->DrawCircle(METERS_TO_PIXELS(pos.x), METERS_TO_PIXELS(pos.y), METERS_TO_PIXELS(shape->m_radius), 255, 255, 255);
			}
			break;

			// Draw polygons ------------------------------------------------
			case b2Shape::e_polygon:
			{
				b2PolygonShape* polygonShape = (b2PolygonShape*)f->GetShape();
				int32 count = polygonShape->GetVertexCount();
				b2Vec2 prev, v;

				for (int32 i = 0; i < count; ++i)
				{
					v = b->GetWorldPoint(polygonShape->GetVertex(i));
					if (i > 0)
						App->render->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 100, 100);

					prev = v;
				}

				v = b->GetWorldPoint(polygonShape->GetVertex(0));
				App->render->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 100, 100);
			}
			break;

			// Draw chains contour -------------------------------------------
			case b2Shape::e_chain:
			{
				b2ChainShape* shape = (b2ChainShape*)f->GetShape();
				b2Vec2 prev, v;

				for (int32 i = 0; i < shape->m_count; ++i)
				{
					v = b->GetWorldPoint(shape->m_vertices[i]);
					if (i > 0)
						App->render->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 100, 255, 100);
					prev = v;
				}

				v = b->GetWorldPoint(shape->m_vertices[0]);
				App->render->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 100, 255, 100);
			}
			break;

			// Draw a single segment(edge) ----------------------------------
			case b2Shape::e_edge:
			{
				b2EdgeShape* shape = (b2EdgeShape*)f->GetShape();
				b2Vec2 v1, v2;

				v1 = b->GetWorldPoint(shape->m_vertex0);
				v1 = b->GetWorldPoint(shape->m_vertex1);
				App->render->DrawLine(METERS_TO_PIXELS(v1.x), METERS_TO_PIXELS(v1.y), METERS_TO_PIXELS(v2.x), METERS_TO_PIXELS(v2.y), 100, 100, 255, false);
			}
			break;
			}


			if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN) 
			{
				int mouse_x, mouse_y;
				App->input->GetMousePosition(mouse_x, mouse_y);
				mouse_x -= App->render->camera.x;
				mouse_y -= App->render->camera.y;
				b2Vec2 mouse(PIXEL_TO_METERS(mouse_x), PIXEL_TO_METERS(mouse_y));
				if (f->TestPoint(mouse)) 
				{
					selected = f->GetBody();
					break;
				}
			}
		}
	}

	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN && selected != nullptr) 
	{
		int mouse_x, mouse_y;
		App->input->GetMousePosition(mouse_x, mouse_y);
		mouse_x -= App->render->camera.x;
		mouse_y -= App->render->camera.y;
		b2Vec2 mouse(PIXEL_TO_METERS(mouse_x), PIXEL_TO_METERS(mouse_y));

		b2MouseJointDef def;
		def.bodyA = ground;
		def.bodyB = selected;
		def.target = mouse;
		def.dampingRatio = 0.5f;
		def.frequencyHz = 2.0f;
		def.maxForce = 100.0f * selected->GetMass();

		mouse_joint = (b2MouseJoint*)world->CreateJoint(&def);

	}

	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT && selected != nullptr) 
	{
		int mouse_x, mouse_y;
		App->input->GetMousePosition(mouse_x, mouse_y);
		mouse_x -= App->render->camera.x;
		mouse_y -= App->render->camera.y;
		b2Vec2 mouse(PIXEL_TO_METERS(mouse_x), PIXEL_TO_METERS(mouse_y));
		b2Vec2 mouse_pix(mouse_x, mouse_y);

		mouse_joint->SetTarget(mouse);
		App->render->DrawLine(mouse_pix.x, mouse_pix.y, METERS_TO_PIXELS(selected->GetPosition().x), METERS_TO_PIXELS(selected->GetPosition().y), 255, 0, 0, 255);
	}

	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP && selected != nullptr) 
	{
		world->DestroyJoint(mouse_joint);
		mouse_joint = nullptr;
		selected = nullptr;
	}

	return ret;
}

// Called before quitting
bool j1Physics::CleanUp()
{
	LOG("Destroying physics world");

	// Delete the whole physics world!
	delete world;

	for (int i = 0; i < App->map->timers.count(); i++)
	{
		App->map->timers[i]->Reset();
	}

	return true;
}

PhysBody* j1Physics::CreateCircle(int x, int y, int radius, float restitution, uint16 mask, uint16 category)
{
	b2BodyDef body;
	body.type = b2_dynamicBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2CircleShape shape;
	shape.m_radius = PIXEL_TO_METERS(radius);
	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.density = 0.5f;
	fixture.filter.maskBits = mask;
	fixture.filter.categoryBits = category;
	fixture.restitution = restitution; //0.4f;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = radius;

	return pbody;
}

PhysBody * j1Physics::CreateCircleSensor(int x, int y, int radius, uint16 mask, uint16 category)
{
	b2BodyDef body;
	body.type = b2_dynamicBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2CircleShape shape;
	shape.m_radius = PIXEL_TO_METERS(radius);
	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.density = 0.5f;
	fixture.filter.maskBits = mask;
	fixture.filter.categoryBits = category;
	fixture.restitution = 0.33f;
	fixture.isSensor = true;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = radius;

	return pbody;
}

PhysBody* j1Physics::CreateRectangle(int x, int y, int width, int height, uint16 mask, uint16 category)
{
	b2BodyDef body;
	body.type = b2_dynamicBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);
	b2PolygonShape box;
	box.SetAsBox(PIXEL_TO_METERS(width) * 0.5f, PIXEL_TO_METERS(height) * 0.5f);

	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.density = 1.0f;
	fixture.filter.maskBits = mask;
	fixture.filter.categoryBits = category;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = width * 0.5f;
	pbody->height = height * 0.5f;

	return pbody;
}

PhysBody * j1Physics::CreateRectangleSensor(int x, int y, int width, int height, uint16 mask, uint16 category)
{
	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2PolygonShape box;
	box.SetAsBox(PIXEL_TO_METERS(width) * 0.5f, PIXEL_TO_METERS(height) * 0.5f);

	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.density = 1.0f;
	fixture.isSensor = true;
	fixture.filter.maskBits = mask;
	fixture.filter.categoryBits = category;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = width;
	pbody->height = height;

	return pbody;
}

PhysBody * j1Physics::CreateChain(int x, int y, int * points, int size, uint16 mask, uint16 category, float restitution)
{
	b2BodyDef body;
	body.type = b2_dynamicBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2ChainShape shape;
	b2Vec2* p = new b2Vec2[size / 2];

	for (uint i = 0; i < size / 2; ++i)
	{
		p[i].x = PIXEL_TO_METERS(points[i * 2 + 0]);
		p[i].y = PIXEL_TO_METERS(points[i * 2 + 1]);
	}

	shape.CreateLoop(p, size / 2);

	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.filter.maskBits = mask;
	fixture.filter.categoryBits = category;
	fixture.restitution = restitution;

	b->CreateFixture(&fixture);

	delete p;

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = 0;

	return pbody;
}

b2PrismaticJoint* j1Physics::CreatePrismaticJoint(PhysBody* bodyA, PhysBody* bodyB, b2Vec2 ancorA, b2Vec2 ancorB, int max, int min, int maxMotor, int motorSpeed)
{
	b2PrismaticJointDef prismaticJointDef;
	prismaticJointDef.bodyA = bodyA->body;
	prismaticJointDef.bodyB = bodyB->body;
	prismaticJointDef.collideConnected = false;
	prismaticJointDef.localAxisA.Set(0, 1);
	prismaticJointDef.localAnchorA.Set(PIXEL_TO_METERS(ancorA.x), PIXEL_TO_METERS(ancorA.y));
	prismaticJointDef.localAnchorB.Set(PIXEL_TO_METERS(ancorB.x), PIXEL_TO_METERS(ancorB.y));

	prismaticJointDef.enableLimit = true;
	prismaticJointDef.lowerTranslation = PIXEL_TO_METERS(min);
	prismaticJointDef.upperTranslation = PIXEL_TO_METERS(max);
	prismaticJointDef.type = e_prismaticJoint;

	prismaticJointDef.enableMotor = true;
	prismaticJointDef.motorSpeed = motorSpeed * DEGTORAD;
	prismaticJointDef.maxMotorForce = maxMotor;

	b2PrismaticJoint* joint = (b2PrismaticJoint*)world->CreateJoint(&prismaticJointDef);

	return joint;
}

b2RevoluteJoint* j1Physics::CreateRevoluteJoint(int radius, int* vects, int size, int posx, int posy, int desplacementx, int desplacementy, int upper_angle, int lower_angle, int max_torque, int speed, uint16 mask, uint16 category)
{
//body and fixture defs - the common parts
  b2BodyDef bodyDef;
  b2FixtureDef fixtureDef;
  fixtureDef.density = 1;
  fixtureDef.filter.maskBits = mask;
  fixtureDef.filter.categoryBits = category;
  
  //two shapes
  b2PolygonShape poligonShape;

  b2Vec2* vect = new b2Vec2[size / 2];
  for (uint i = 0; i < size/2; ++i)
  {
	  vect[i].x = PIXEL_TO_METERS(vects[i * 2 + 0]);
	  vect[i].y = PIXEL_TO_METERS(vects[i * 2 + 1]);
  }

  poligonShape.Set(vect, size/2);


  b2CircleShape circleShape;
  circleShape.m_radius = PIXEL_TO_METERS(radius);
  
  //make a box
  bodyDef.position.Set(PIXEL_TO_METERS(posx), PIXEL_TO_METERS(posy));
  fixtureDef.shape = &poligonShape;
  bodyDef.type = b2_dynamicBody;
  b2Body* m_bodyA = world->CreateBody( &bodyDef );
  m_bodyA->CreateFixture( &fixtureDef );
  
  //and a circle
  bodyDef.position.Set(PIXEL_TO_METERS(posx), PIXEL_TO_METERS(posy));
  fixtureDef.shape = &circleShape;
  bodyDef.type = b2_staticBody;
  b2Body* m_bodyB = world->CreateBody( &bodyDef );
  m_bodyB->CreateFixture( &fixtureDef );

  b2RevoluteJointDef revoluteJointDef;
  revoluteJointDef.bodyA = m_bodyA;
  revoluteJointDef.bodyB = m_bodyB;
  revoluteJointDef.collideConnected = false;
  revoluteJointDef.type = e_revoluteJoint;
  revoluteJointDef.localAnchorA.Set(PIXEL_TO_METERS(desplacementx), PIXEL_TO_METERS(desplacementy));

  if (lower_angle != NULL && upper_angle != NULL) 
  {
	  revoluteJointDef.enableLimit = true;
	  revoluteJointDef.lowerAngle = lower_angle * DEGTORAD;
	  revoluteJointDef.upperAngle = upper_angle * DEGTORAD;
  }
  else
	  revoluteJointDef.enableLimit = false;

  if (max_torque != 0)
  {
	  revoluteJointDef.enableMotor = true;
	  revoluteJointDef.maxMotorTorque = max_torque;
	  revoluteJointDef.motorSpeed = speed * DEGTORAD; //90 degrees per second
  }
  else
	  revoluteJointDef.enableMotor = false;


  b2RevoluteJoint* m_joint = (b2RevoluteJoint*)world->CreateJoint(&revoluteJointDef);
	
	return m_joint;
}

void j1Physics::BeginContact(b2Contact * contact)
{
	PhysBody* physA = (PhysBody*)contact->GetFixtureA()->GetBody()->GetUserData();
	PhysBody* physB = (PhysBody*)contact->GetFixtureB()->GetBody()->GetUserData();

	if (physA && physA->listener != NULL)
		physA->listener->OnCollision(physA, physB);

	if (physB && physB->listener != NULL)
		physB->listener->OnCollision(physB, physA);
}

void j1Physics::OnCollision(PhysBody * bodyA, PhysBody * bodyB)
{
	if (bodyA != nullptr && bodyB != nullptr && bodyA->coll_name == ball) 
	{
		switch(bodyB->coll_name)
		{
		case kawaii_blue:
			if (App->map->kawaii_blue->collide_once) {
				App->audio->PlayFx(App->map->kawaii_sound2_fx);
				App->map->kawaii_blue->collide_once = false;
				App->map->kawaii_blue->collide_for_bonus = true;
			}
			App->map->kawaii_blue->timer->Start();
			App->map->player.score += 100;
			App->map->print_points1->New100();
			break;
		case kawaii_red:
			if (App->map->kawaii_red->collide_once) {
				App->audio->PlayFx(App->map->kawaii_sound1_fx);
				App->map->kawaii_red->collide_once = false;
				App->map->kawaii_red->collide_for_bonus = true;
			}
			App->map->kawaii_red->timer->Start();
			App->map->player.score += 100;
			App->map->print_points1->New100();
			break;
		case kawaii_violet:
			if (App->map->kawaii_violet->collide_once) {
				App->audio->PlayFx(App->map->kawaii_sound1_fx);
				App->map->kawaii_violet->collide_once = false;
				App->map->kawaii_violet->collide_for_bonus = true;
			}
			App->map->kawaii_violet->timer->Start();
			App->map->player.score += 100;
			App->map->print_points1->New100();
			break;
		case kawaii_yellow:
			if (App->map->kawaii_yellow->collide_once) {
				App->audio->PlayFx(App->map->kawaii_sound1_fx);
				App->map->kawaii_yellow->collide_once = false;
				App->map->kawaii_yellow->collide_for_bonus = true;
			}
			App->map->kawaii_yellow->timer->Start();
			App->map->player.score += 100;
			App->map->print_points1->New100();
			break;
		case kawaii_green:
			if (App->map->kawaii_green->collide_once) {
				App->audio->PlayFx(App->map->kawaii_sound2_fx);
				App->map->kawaii_green->collide_once = false;
				App->map->kawaii_green->collide_for_bonus = true;
			}
			App->map->kawaii_green->timer->Start();
			App->map->player.score += 100;
			App->map->print_points1->New100();
			break;
		case kawaii_orange:
			if (App->map->kawaii_orange->collide_once) {
				App->audio->PlayFx(App->map->kawaii_sound2_fx);
				App->map->kawaii_orange->collide_once = false;
				App->map->kawaii_orange->collide_for_bonus = true;
			}
			App->map->kawaii_orange->timer->Start();
			App->map->player.score += 100;
			App->map->print_points1->New100();
			break;
		case kawaii_box1:
			if (App->map->kawaii_box1->collide_once) {
				App->audio->PlayFx(App->map->kawaii_sound2_fx);
				App->map->kawaii_box1->collide_once = false;
			}
			App->map->kawaii_box1->timer->Start();
			App->map->player.score += 100;
			App->map->print_points1->New100();
			break;
		case kawaii_box2:
			if (App->map->kawaii_box2->collide_once) {
				App->audio->PlayFx(App->map->kawaii_sound2_fx);
				App->map->kawaii_box2->collide_once = false;
			}
			App->map->kawaii_box2->timer->Start();
			App->map->player.score += 100;
			App->map->print_points1->New100();
			break;
		case big_bumper_1:
			App->map->big_bumper1->timer->Start();
			App->audio->PlayFx(App->map->big_bumper_fx);
			App->map->player.score += 100;
			App->map->print_points1->New100();
			break;
		case big_bumper_2:
			App->map->big_bumper2->timer->Start();
			App->audio->PlayFx(App->map->big_bumper_fx);
			App->map->player.score += 100;
			App->map->print_points1->New100();
			break;
		case small_bumper_1:
			App->map->small_bumper1->timer->Start();
			App->audio->PlayFx(App->map->small_bumper_fx);
			App->map->player.score += 100;
			App->map->print_points1->New100();
			break;
		case small_bumper_2:
			App->map->small_bumper2->timer->Start();
			App->audio->PlayFx(App->map->small_bumper_fx);
			App->map->player.score += 100;
			App->map->print_points1->New100();
			break;
		case small_bumper_3:
			App->map->small_bumper3->timer->Start();
			App->audio->PlayFx(App->map->small_bumper_fx);
			App->map->player.score += 100;
			App->map->print_points1->New100();
			break;
		case small_bumper_4:
			App->map->small_bumper4->timer->Start();
			App->audio->PlayFx(App->map->small_bumper_fx);
			App->map->player.score += 100;
			App->map->print_points1->New100();
			break;
		case small_bumper_5:
			App->map->small_bumper5->timer->Start();
			App->audio->PlayFx(App->map->small_bumper_fx);
			App->map->player.score += 100;
			App->map->print_points1->New100();
			break;
		case small_bumper_6:
			App->map->small_bumper6->timer->Start();
			App->audio->PlayFx(App->map->small_bumper_fx);
			App->map->player.score += 100;
			App->map->print_points1->New100();
			break;
		case small_bumper_7:
			App->map->small_bumper7->timer->Start();
			App->audio->PlayFx(App->map->small_bumper_fx);
			App->map->player.score += 100;
			App->map->print_points1->New100();
			break;
		case kawaii_girl:
			if (App->map->kawaii_girl->collide_once) {
				App->audio->PlayFx(App->map->kawaii_sound4_fx);
				App->map->kawaii_girl->collide_once = false;
				App->map->kawaii_girl->collide_for_bonus = true;
			}
			App->map->kawaii_girl->timer->Start();
			App->map->player.score += 100;
			App->map->print_points1->New100();
			break;
		case kawaii_guy:
			if (App->map->kawaii_guy->collide_once) {
				App->audio->PlayFx(App->map->kawaii_sound3_fx);
				App->map->kawaii_guy->collide_once = false;
				App->map->kawaii_guy->collide_for_bonus = true;
			}
			App->map->kawaii_guy->timer->Start();
			App->map->player.score += 100;
			App->map->print_points1->New100();
			break;
		case ball_dead_detector:
			App->scene->resetball = true;

			if(App->map->player.balls > 0)
				App->map->player.balls--;
			break;
		case wall:
			App->audio->PlayFx(App->map->ball_hit_fx);
			break;
		case slingshot:
			App->audio->PlayFx(App->map->slingshot_fx);
			App->map->player.score += 100;
			App->map->print_points1->New100();
			break;
		case target:
			App->audio->PlayFx(App->map->target_fx);
			App->map->little_button1->anim.SetFrame(1);
			App->map->player.score += 500;
			App->map->print_points1->New500();
			break;
		case target2:
			App->audio->PlayFx(App->map->target_fx);
			App->map->little_button3->anim.SetFrame(1);
			App->map->player.score += 500;
			App->map->print_points1->New500();
			break;
		case target3:
			App->audio->PlayFx(App->map->target_fx);
			App->map->little_button4->anim.SetFrame(1);
			App->map->player.score += 500;
			App->map->print_points1->New500();
			break;
		case target4:
			App->audio->PlayFx(App->map->target_fx);
			App->map->little_button2->anim.SetFrame(1);
			App->map->player.score += 500;
			App->map->print_points1->New500();
			break;
		case target5:
			App->audio->PlayFx(App->map->target_fx);
			App->map->little_button5->anim.SetFrame(1);
			App->map->player.score += 500;
			App->map->print_points1->New500();
			break;
		case target6:
			App->audio->PlayFx(App->map->target_fx);
			App->map->little_button8->anim.SetFrame(1);
			App->map->player.score += 500;
			App->map->print_points1->New500();
			break;
		case target7:
			App->audio->PlayFx(App->map->target_fx);
			App->map->little_button7->anim.SetFrame(1);
			App->map->player.score += 500;
			App->map->print_points1->New500();
			break;
		case target8:
			App->audio->PlayFx(App->map->target_fx);
			App->map->little_button6->anim.SetFrame(1);
			App->map->player.score += 500;
			App->map->print_points1->New500();
			break;
		default:
			break;
		}
	}
}

void j1Physics::TimerActions()
{
	// Timer actions ----------------------------------------------------------------------------------


	// Kawaii_blue
	if (App->map->kawaii_blue->timer->IsTimeReached())	{
		if (App->map->kawaii_blue->timer->stop_time) { 
			App->map->kawaii_blue->anim.SetFrame(0); 
		}
		else {
			App->map->kawaii_blue->anim.SetFrame(1);
			App->map->kawaii_blue->anim.speed = 0;
		}
	}
	else{
		App->map->kawaii_blue->anim.speed = 0.5f;
	}

	// Kawaii_green
	if (App->map->kawaii_green->timer->IsTimeReached()) {
		if (App->map->kawaii_green->timer->stop_time) {
			App->map->kawaii_green->anim.SetFrame(0);
		}
		else {
			App->map->kawaii_green->anim.SetFrame(1);
			App->map->kawaii_green->anim.speed = 0;
		}
	}
	else {
		App->map->kawaii_green->anim.speed = 0.5f;
	}

	// Kawaii_red
	if (App->map->kawaii_red->timer->IsTimeReached()) {
		if (App->map->kawaii_red->timer->stop_time) {
			App->map->kawaii_red->anim.SetFrame(0);
		}
		else {
			App->map->kawaii_red->anim.SetFrame(1);
			App->map->kawaii_red->anim.speed = 0;
		}
	}
	else {
		App->map->kawaii_red->anim.speed = 0.5f;
	}

	// Kawaii_orange
	if (App->map->kawaii_orange->timer->IsTimeReached()) {
		if (App->map->kawaii_orange->timer->stop_time) {
			App->map->kawaii_orange->anim.SetFrame(0);
		}
		else {
			App->map->kawaii_orange->anim.SetFrame(1);
			App->map->kawaii_orange->anim.speed = 0;
		}
	}
	else {
		App->map->kawaii_orange->anim.speed = 0.5f;
	}

	// Kawaii_violet
	if (App->map->kawaii_violet->timer->IsTimeReached()) {
		if (App->map->kawaii_violet->timer->stop_time) {
			App->map->kawaii_violet->anim.SetFrame(0);
		}
		else {
			App->map->kawaii_violet->anim.SetFrame(1);
			App->map->kawaii_violet->anim.speed = 0;
		}
	}
	else {
		App->map->kawaii_violet->anim.speed = 0.5f;
	}

	// Kawaii_yellow
	if (App->map->kawaii_yellow->timer->IsTimeReached()) {
		if (App->map->kawaii_yellow->timer->stop_time) {
			App->map->kawaii_yellow->anim.SetFrame(0);
		}
		else {
			App->map->kawaii_yellow->anim.SetFrame(1);
			App->map->kawaii_yellow->anim.speed = 0;
		}
	}
	else {
		App->map->kawaii_yellow->anim.speed = 0.5f;
	}

	// Kawaii_guy
	if (App->map->kawaii_guy->timer->IsTimeReached()) {
		if (App->map->kawaii_guy->timer->stop_time) {
			App->map->kawaii_guy->anim.SetFrame(0);
		}
		else {
			App->map->kawaii_guy->anim.SetFrame(1);
			App->map->kawaii_guy->anim.speed = 0;
		}
	}
	else {
		App->map->kawaii_guy->anim.speed = 0.5f;
	}

	// Kawaii_girl
	if (App->map->kawaii_girl->timer->IsTimeReached()) {
		if (App->map->kawaii_girl->timer->stop_time) {
			App->map->kawaii_girl->anim.SetFrame(0);
		}
		else {
			App->map->kawaii_girl->anim.SetFrame(1);
			App->map->kawaii_girl->anim.speed = 0;
		}
	}
	else {
		App->map->kawaii_girl->anim.speed = 0.5f;
	}

	// Kawaii_box 1
	if (App->map->kawaii_box1->timer->IsTimeReached()) {
		if (App->map->kawaii_box1->timer->stop_time) {
			App->map->kawaii_box1->anim.SetFrame(0);
		}
		else {
			App->map->kawaii_box1->anim.SetFrame(1);
			App->map->kawaii_box1->anim.speed = 0;
		}
	}
	else {
		App->map->kawaii_box1->anim.speed = 0.5f;
	}

	// Kawaii_box 2
	if (App->map->kawaii_box2->timer->IsTimeReached()) {
		if (App->map->kawaii_box2->timer->stop_time) {
			App->map->kawaii_box2->anim.SetFrame(0);
		}
		else {
			App->map->kawaii_box2->anim.SetFrame(1);
			App->map->kawaii_box2->anim.speed = 0;
		}
	}
	else {
		App->map->kawaii_box2->anim.speed = 0.5f;
	}


	// Big bumper 1
	if (App->map->big_bumper1->timer->IsTimeReached()) {
		if (!App->map->big_bumper1->timer->stop_time) {
			App->map->big_bumper1->anim.SetFrame(0);
		}
	}
	else
	{
		App->map->big_bumper1->anim.SetFrame(1);
	}

	// Big bumper 2
	if (App->map->big_bumper2->timer->IsTimeReached()) {
		if (!App->map->big_bumper2->timer->stop_time) {
			App->map->big_bumper2->anim.SetFrame(0);
		}
	}
	else
	{
		App->map->big_bumper2->anim.SetFrame(1);
	}

	// Small bumper 1
	if (App->map->small_bumper1->timer->IsTimeReached()) {
		if (!App->map->small_bumper1->timer->stop_time) {
			App->map->small_bumper1->anim.SetFrame(0);
		}
	}
	else
	{
		App->map->small_bumper1->anim.SetFrame(1);
	}

	// Small bumper 2
	if (App->map->small_bumper2->timer->IsTimeReached()) {
		if (!App->map->small_bumper2->timer->stop_time) {
			App->map->small_bumper2->anim.SetFrame(0);
		}
	}
	else
	{
		App->map->small_bumper2->anim.SetFrame(1);
	}

	// Small bumper 3
	if (App->map->small_bumper3->timer->IsTimeReached()) {
		if (!App->map->small_bumper3->timer->stop_time) {
			App->map->small_bumper3->anim.SetFrame(0);
		}
	}
	else
	{
		App->map->small_bumper3->anim.SetFrame(1);
	}

	// Small bumper 4
	if (App->map->small_bumper4->timer->IsTimeReached()) {
		if (!App->map->small_bumper4->timer->stop_time) {
			App->map->small_bumper4->anim.SetFrame(0);
		}
	}
	else
	{
		App->map->small_bumper4->anim.SetFrame(1);
	}

	// Small bumper 5
	if (App->map->small_bumper5->timer->IsTimeReached()) {
		if (!App->map->small_bumper5->timer->stop_time) {
			App->map->small_bumper5->anim.SetFrame(0);
		}
	}
	else
	{
		App->map->small_bumper5->anim.SetFrame(1);
	}

	// Small bumper 6
	if (App->map->small_bumper6->timer->IsTimeReached()) {
		if (!App->map->small_bumper6->timer->stop_time) {
			App->map->small_bumper6->anim.SetFrame(0);
		}
	}
	else
	{
		App->map->small_bumper6->anim.SetFrame(1);
	}

	// Small bumper 7
	if (App->map->small_bumper7->timer->IsTimeReached()) {
		if (!App->map->small_bumper7->timer->stop_time) {
			App->map->small_bumper7->anim.SetFrame(0);
		}
	}
	else
	{
		App->map->small_bumper7->anim.SetFrame(1);
	}

	// ------------------------------------------------------------------------------------------------
}

void PhysBody::GetPosition(int & x, int & y) const
{
	b2Vec2 pos = body->GetPosition();
	x = METERS_TO_PIXELS(pos.x) - (width);
	y = METERS_TO_PIXELS(pos.y) - (height);
}

float PhysBody::GetRotation() const
{
	return RADTODEG * body->GetAngle();
}

bool PhysBody::Contains(int x, int y) const
{
	b2Vec2 p(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	const b2Fixture* fixture = body->GetFixtureList();

	while (fixture != NULL)
	{
		if (fixture->GetShape()->TestPoint(body->GetTransform(), p) == true)
			return true;
		fixture = fixture->GetNext();
	}

	return false;
}

int PhysBody::RayCast(int x1, int y1, int x2, int y2, float & normal_x, float & normal_y) const
{
	int ret = -1;

	b2RayCastInput input;
	b2RayCastOutput output;

	input.p1.Set(PIXEL_TO_METERS(x1), PIXEL_TO_METERS(y1));
	input.p2.Set(PIXEL_TO_METERS(x2), PIXEL_TO_METERS(y2));
	input.maxFraction = 1.0f;

	const b2Fixture* fixture = body->GetFixtureList();

	while (fixture != NULL)
	{
		if (fixture->GetShape()->RayCast(&output, input, body->GetTransform(), 0) == true)
		{
			// do we want the normal ?

			float fx = x2 - x1;
			float fy = y2 - y1;
			float dist = sqrtf((fx*fx) + (fy*fy));

			normal_x = output.normal.x;
			normal_y = output.normal.y;

			return output.fraction * dist;
		}
		fixture = fixture->GetNext();
	}

	return ret;
}

