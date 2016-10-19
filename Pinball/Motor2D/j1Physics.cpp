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

	world = new b2World(b2Vec2(GRAVITY_X, -GRAVITY_Y));
	world->SetContactListener(this);

	return true;
}

// Called each loop iteration
bool j1Physics::PreUpdate()
{
	world->Step(1.0f / 60.0f, 6, 2);

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
				App->render->DrawLine(METERS_TO_PIXELS(v1.x), METERS_TO_PIXELS(v1.y), METERS_TO_PIXELS(v2.x), METERS_TO_PIXELS(v2.y), 100, 100, 255);
			}
			break;
			}
		}
	}

	return ret;
}

// Called before quitting
bool j1Physics::CleanUp()
{
	LOG("Destroying physics world");

	// Delete the whole physics world!
	delete world;

	return true;
}

PhysBody * j1Physics::CreateCircle(int x, int y, int radius, uint16 mask, uint16 category)
{
	b2BodyDef body;
	body.type = b2_dynamicBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2CircleShape shape;
	shape.m_radius = PIXEL_TO_METERS(radius);
	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.density = 1.0f;
	fixture.filter.maskBits = mask;
	fixture.filter.categoryBits = category;

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

PhysBody * j1Physics::CreateRectangleSensor(int x, int y, int width, int height)
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

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = width;
	pbody->height = height;

	return pbody;
}

PhysBody * j1Physics::CreateChain(int x, int y, int * points, int size, uint16 mask, uint16 category)
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
