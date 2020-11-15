#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModulePhysics.h"
#include "ModuleSceneIntro.h"
#include "p2Point.h"
#include "math.h"

#include "Box2D/Box2D/Box2D.h"

#ifdef _DEBUG
#pragma comment( lib, "Box2D/libx86/Debug/Box2D.lib" )
#else
#pragma comment( lib, "Box2D/libx86/Release/Box2D.lib" )
#endif

ModulePhysics::ModulePhysics(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	world = NULL;
	debug = true;
}

// Destructor
ModulePhysics::~ModulePhysics()
{
}

bool ModulePhysics::Start()
{
	LOG("Creating Physics 2D environment");

	world = new b2World(b2Vec2(GRAVITY_X, -GRAVITY_Y));
	world->SetContactListener(this);

	b2BodyDef bd;
	ground = world->CreateBody(&bd);

	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(0), PIXEL_TO_METERS(0));

	b2Body* backgroundBlue = world->CreateBody(&body);

	//Board create
	int Pinball_MainBoard_1_coords[80] = {
		119, 476,
		72, 430,
		72, 372,
		87, 357,
		80, 349,
		80, 283,
		72, 261,
		72, 175,
		87, 160,
		87, 126,
		72, 80,
		72, 66,
		78, 49,
		88, 36,
		99, 28,
		106, 25,
		196, 25,
		208, 30,
		219, 39,
		227, 52,
		231, 63,
		232, 69,
		232, 394,
		228, 396,
		228, 445,
		223, 445,
		223, 396,
		219, 394,
		219, 317,
		209, 317,
		209, 349,
		201, 357,
		215, 371,
		215, 431,
		169, 476,
		235, 476,
		235, 1,
		69, 1,
		69, 476,
		119, 476
	};

	b2ChainShape shape;
	b2Vec2* p = new b2Vec2[78 / 2];

	for (uint i = 0; i < 78 / 2; ++i)
	{
		p[i].x = PIXEL_TO_METERS(Pinball_MainBoard_1_coords[i * 2 + 0]);
		p[i].y = PIXEL_TO_METERS(Pinball_MainBoard_1_coords[i * 2 + 1]);
	}
	shape.CreateLoop(p, 78 / 2);

	b2FixtureDef fixture;
	fixture.shape = &shape;
	backgroundBlue->CreateFixture(&fixture);

	return true;
}

// 
update_status ModulePhysics::PreUpdate()
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

	return UPDATE_CONTINUE;
}

// 
update_status ModulePhysics::PostUpdate()
{
	// On space bar press, create a circle on mouse position


	if(App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	if(!debug)
		return UPDATE_CONTINUE;

	// Bonus code: this will iterate all objects in the world and draw the circles
	// You need to provide your own macro to translate meters to pixels
	for(b2Body* b = world->GetBodyList(); b; b = b->GetNext())
	{
		for(b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext())
		{
			switch(f->GetType())
			{
				// Draw circles ------------------------------------------------
				case b2Shape::e_circle:
				{
					b2CircleShape* shape = (b2CircleShape*)f->GetShape();
					b2Vec2 pos = f->GetBody()->GetPosition();
					App->renderer->DrawCircle(METERS_TO_PIXELS(pos.x)*SCREEN_SIZE, METERS_TO_PIXELS(pos.y)* SCREEN_SIZE, METERS_TO_PIXELS(shape->m_radius)* SCREEN_SIZE, 255, 255, 255);
				}
				break;

				// Draw polygons ------------------------------------------------
				case b2Shape::e_polygon:
				{
					b2PolygonShape* polygonShape = (b2PolygonShape*)f->GetShape();
					int32 count = polygonShape->GetVertexCount();
					b2Vec2 prev, v;

					for(int32 i = 0; i < count; ++i)
					{
						v = b->GetWorldPoint(polygonShape->GetVertex(i));
						if(i > 0)
							App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 100, 100);

						prev = v;
					}

					v = b->GetWorldPoint(polygonShape->GetVertex(0));
					App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 100, 100);
				}
				break;

				// Draw chains contour -------------------------------------------
				case b2Shape::e_chain:
				{
					b2ChainShape* shape = (b2ChainShape*)f->GetShape();
					b2Vec2 prev, v;

					for(int32 i = 0; i < shape->m_count; ++i)
					{
						v = b->GetWorldPoint(shape->m_vertices[i]);
						if(i > 0)
							App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 100, 255, 100);
						prev = v;
					}

					v = b->GetWorldPoint(shape->m_vertices[0]);
					App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 100, 255, 100);
				}
				break;

				// Draw a single segment(edge) ----------------------------------
				case b2Shape::e_edge:
				{
					b2EdgeShape* shape = (b2EdgeShape*)f->GetShape();
					b2Vec2 v1, v2;

					v1 = b->GetWorldPoint(shape->m_vertex0);
					v1 = b->GetWorldPoint(shape->m_vertex1);
					App->renderer->DrawLine(METERS_TO_PIXELS(v1.x), METERS_TO_PIXELS(v1.y), METERS_TO_PIXELS(v2.x), METERS_TO_PIXELS(v2.y), 100, 100, 255);
				}
				break;
			}
		}
	}

	return UPDATE_CONTINUE;
}


// Called before quitting
bool ModulePhysics::CleanUp()
{
	LOG("Destroying physics world");

	// Delete the whole physics world!
	delete world;

	return true;
}

//Create Ball
PhysBody* ModulePhysics::CreateCircle(int x, int y, float radius, b2BodyType type, float restitution)
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

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = radius;

	return pbody;
}

//Function to create rectangles
PhysBody* ModulePhysics::CreateRectangle(int x, int y, int width, int height, float angle, b2BodyType type, float restitution)
{
	b2BodyDef body;
	body.type = type;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));
	body.angle = angle;

	b2Body* b = world->CreateBody(&body);
	b2PolygonShape box;
	box.SetAsBox(PIXEL_TO_METERS(width) * 0.5f, PIXEL_TO_METERS(height) * 0.5f);

	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.density = 1.0f;
	fixture.restitution = restitution;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = width * 0.5f;
	pbody->height = height * 0.5f;

	return pbody;
}

PhysBody* ModulePhysics::CreateRectangleSensor(int x, int y, int width, int height, float angle)
{
	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));
	body.angle = angle;

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

//Function to create chains
PhysBody* ModulePhysics::CreateChain(int x, int y, int* points, int size, float rest)
{
	b2BodyDef body;
	body.type = b2_staticBody;
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
	fixture.restitution = rest;
	b->CreateFixture(&fixture);

	delete p;

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = 0;

	return pbody;
}

// Create Paddle Left
PhysBody* ModulePhysics::CreatePaddleLeft(int x, int y, float angd, float angu)
{
	//Circle
	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2CircleShape shape;
	shape.m_radius = PIXEL_TO_METERS(0);
	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.density = 1.0f;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = 0;

	//Polygon
	b2BodyDef body2;
	body2.type = b2_dynamicBody;
	body2.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b2 = world->CreateBody(&body2);

	b2PolygonShape shape2;

	int points[16] =
	{
		1, 10,
		1, 6,
		3, 3,
		33, 6,
		33, 8,
		12, 11,
		6, 12,
		2, 11
	};

	b2Vec2* p = new b2Vec2[8];

	for (uint i = 0; i < 8; ++i)
	{
		p[i].x = PIXEL_TO_METERS(points[i * 2 + 0]);
		p[i].y = PIXEL_TO_METERS(points[i * 2 + 1]);
	}
	shape2.Set(p, 8);
	delete p;

	b2FixtureDef fixture2;
	fixture2.shape = &shape2;
	fixture2.density = 2.0f;

	b2->CreateFixture(&fixture2);

	PhysBody* pbody2 = new PhysBody();
	pbody->body = b2;
	b2->SetUserData(pbody2);
	pbody->width = pbody->height = 0;

	//REVOLUTION JOINT
	b2RevoluteJointDef revoluteJointDef;
	revoluteJointDef.bodyA = b;
	revoluteJointDef.bodyB = b2;
	revoluteJointDef.localAnchorA.Set(PIXEL_TO_METERS(0), PIXEL_TO_METERS(0));
	revoluteJointDef.localAnchorB.Set(PIXEL_TO_METERS(4), PIXEL_TO_METERS(6));
	revoluteJointDef.referenceAngle = 0;
	revoluteJointDef.collideConnected = false;
	revoluteJointDef.enableMotor = false;
	revoluteJointDef.maxMotorTorque = 500;
	revoluteJointDef.motorSpeed = -1000 * DEGTORAD;
	revoluteJointDef.enableLimit = true;
	revoluteJointDef.lowerAngle = angu;
	revoluteJointDef.upperAngle = angd;

	paddleLeftList.add((b2RevoluteJoint*)world->CreateJoint(&revoluteJointDef));
	return pbody;
}
//MOVE PADDLE LEFT
void ModulePhysics::PaddleMoveLeft()
{
	p2List_item<b2RevoluteJoint*>* temp = paddleLeftList.getFirst();
	while (temp != nullptr)
	{
		temp->data->EnableMotor(true);
		temp = temp->next;
	}
}
//STOP PADDLE LEFT
void ModulePhysics::PaddleStopLeft()
{
	p2List_item<b2RevoluteJoint*>* temp = paddleLeftList.getFirst();
	while (temp != nullptr)
	{
		temp->data->EnableMotor(false);
		temp = temp->next;
	}
}

//Create Paddle Right
PhysBody* ModulePhysics::CreatePaddleRight(int x, int y, float angd, float angu)
{
	//Circle
	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2CircleShape shape;
	shape.m_radius = PIXEL_TO_METERS(0);
	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.density = 1.0f;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = 50;

	//Polygon
	b2BodyDef body2;
	body2.type = b2_dynamicBody;
	body2.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b2 = world->CreateBody(&body2);

	b2PolygonShape shape2;

	int points[14] =
	{
		-3, 2,
		27, 3,
		29, 4,
		29, 8,
		27, 12,
		21, 12,
		-3, 3
	};

	b2Vec2* p = new b2Vec2[7];

	for (uint i = 0; i < 7; ++i)
	{
		p[i].x = PIXEL_TO_METERS(points[i * 2 + 0]);
		p[i].y = PIXEL_TO_METERS(points[i * 2 + 1]);
	}
	shape2.Set(p, 7);
	delete p;

	b2FixtureDef fixture2;
	fixture2.shape = &shape2;
	fixture2.density = 2.0f;

	b2->CreateFixture(&fixture2);

	PhysBody* pbody2 = new PhysBody();
	pbody->body = b2;
	b2->SetUserData(pbody2);
	pbody->width = pbody->height = 0;

	//REVOLUTION JOINT
	b2RevoluteJointDef revoluteJointDef;
	revoluteJointDef.bodyA = b;
	revoluteJointDef.bodyB = b2;
	revoluteJointDef.localAnchorA.Set(PIXEL_TO_METERS(0), PIXEL_TO_METERS(0));
	revoluteJointDef.localAnchorB.Set(PIXEL_TO_METERS(25), PIXEL_TO_METERS(6));
	revoluteJointDef.referenceAngle = -120 * DEGTORAD;
	revoluteJointDef.collideConnected = false;
	revoluteJointDef.enableMotor = false;
	revoluteJointDef.maxMotorTorque = 500;
	revoluteJointDef.motorSpeed = 1000 * DEGTORAD;
	revoluteJointDef.enableLimit = true;
	revoluteJointDef.lowerAngle = angu;
	revoluteJointDef.upperAngle = angd;
	
	paddleRightList.add((b2RevoluteJoint*)world->CreateJoint(&revoluteJointDef));
	return pbody;
}
//MOVE PADDLE RIGHT
void ModulePhysics::PaddleMoveRight()
{
	p2List_item<b2RevoluteJoint*>* temp = paddleRightList.getFirst();
	while (temp != nullptr)
	{
		temp->data->EnableMotor(true);
		temp = temp->next;
	}
}
//STOP PADDLE RIGHT
void ModulePhysics::PaddleStopRight()
{
	p2List_item<b2RevoluteJoint*>* temp = paddleRightList.getFirst();
	while (temp != nullptr)
	{
		temp->data->EnableMotor(false);
		temp = temp->next;
	}
}

//PhysBody functions
void PhysBody::GetPosition(int& x, int& y) const
{
	b2Vec2 pos = body->GetPosition();
	x = METERS_TO_PIXELS(pos.x) - (width);
	y = METERS_TO_PIXELS(pos.y) - (height);
}
float PhysBody::GetRotation() const
{
	return RADTODEG * body->GetAngle();
}

b2DistanceJointDef* ModulePhysics::CreateLineJoint(b2Body* bodyA, b2Body* bodyB, p2Point<float> Local_Anchor_A, p2Point<float> Local_Anchor_B, float frequency, float damping)
{
	b2DistanceJointDef DistanceJoinDef;

	DistanceJoinDef.bodyA = bodyA;
	DistanceJoinDef.bodyB = bodyB;

	DistanceJoinDef.localAnchorA.Set(Local_Anchor_A.x, Local_Anchor_A.y);
	DistanceJoinDef.localAnchorB.Set(Local_Anchor_B.x, Local_Anchor_B.y);

	DistanceJoinDef.dampingRatio = damping;
	DistanceJoinDef.frequencyHz = frequency;

	world->CreateJoint(&DistanceJoinDef);
	b2DistanceJointDef* dis_joint = (b2DistanceJointDef*)world->CreateJoint(&DistanceJoinDef);
	return dis_joint;
}


void ModulePhysics::BeginContact(b2Contact* contact)
{
	PhysBody* physA = (PhysBody*)contact->GetFixtureA()->GetBody()->GetUserData();
	PhysBody* physB = (PhysBody*)contact->GetFixtureB()->GetBody()->GetUserData();

	if (physA && physA->listener != NULL)
		physA->listener->OnCollision(physA, physB);

	if (physB && physB->listener != NULL)
		physB->listener->OnCollision(physB, physA);
}