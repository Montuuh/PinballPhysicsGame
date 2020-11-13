#pragma once
#include "Module.h"
#include "Globals.h"
#include "Application.h"

#include "Box2D/Box2D/Box2D.h"



#define GRAVITY_X 0.0f
#define GRAVITY_Y -7.0f

#define PIXELS_PER_METER 50.0f // if touched change METER_PER_PIXEL too
#define METER_PER_PIXEL 0.02f // this is 1 / PIXELS_PER_METER !

#define METERS_TO_PIXELS(m) ((int) floor(PIXELS_PER_METER * m))
#define PIXEL_TO_METERS(p)  ((float) METER_PER_PIXEL * p)

class b2World;
class b2Body;

// Small class to return to other modules to track position and rotation of physics bodies
class PhysBody
{
public:
	PhysBody() : listener(NULL), body(NULL)
	{}

	void GetPosition(int& x, int& y) const;
	float GetRotation() const;
	bool Contains(int x, int y) const;
	int RayCast(int x1, int y1, int x2, int y2, float& normal_x, float& normal_y) const;

public:
	int width, height;
	b2Body* body;
	Module* listener;
};

class ModulePhysics : public Module
{
public:
	ModulePhysics(Application* app, bool start_enabled = true);
	~ModulePhysics();

	bool Start();
	update_status PreUpdate();
	update_status PostUpdate();
	bool CleanUp();

	// TODO 4: Move body creation to 3 functions to create circles, rectangles and chains
	void SpawnCircle(float radius, float density, float restitution, float friction);
	void SpawnRectangle(float width, float height, float density, float restitution, float friction);
	//void SpawnChain(int points[], const int *arrSize, float density, float restitution, float friction);

	//Functions to create paddles
	PhysBody* CreatePaddleLeft(int x, int y, float angd, float angu);
	PhysBody* CreatePaddleRight(int x, int y, float angd, float angu);

	void PaddleMoveLeft();
	void PaddleStopLeft();
	void PaddleMoveRight();
	void PaddleStopRight();

private:

	bool debug;
	b2World* world;
	b2Body* ground;

	p2List<b2RevoluteJoint*> paddleLeftList;
	p2List<b2RevoluteJoint*> paddleRightList;
};


inline void ModulePhysics::SpawnCircle(float radius, float density, float restitution, float friction)
{
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(PIXEL_TO_METERS(App->input->GetMouseX()), PIXEL_TO_METERS(App->input->GetMouseY()));

	b2Body* circle = world->CreateBody(&bodyDef);

	b2CircleShape circleShape;
	circleShape.m_radius = PIXEL_TO_METERS(radius) * 0.5f;

	b2FixtureDef fixture;
	fixture.shape = &circleShape;
	fixture.density = density;
	fixture.restitution = restitution;
	fixture.friction = friction;

	circle->CreateFixture(&fixture);
}

inline void ModulePhysics::SpawnRectangle(float width, float height, float density, float restitution, float friction)
{
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(PIXEL_TO_METERS(App->input->GetMouseX()), PIXEL_TO_METERS(App->input->GetMouseY()));

	b2Body* polygon = world->CreateBody(&bodyDef);

	b2PolygonShape polygonShape;
	polygonShape.SetAsBox(PIXEL_TO_METERS(width / 2), PIXEL_TO_METERS(height / 2));

	b2FixtureDef fixture;
	fixture.shape = &polygonShape;
	fixture.density = density;
	fixture.restitution = restitution;
	fixture.friction = friction;

	polygon->CreateFixture(&fixture);
}

//void ModulePhysics::SpawnChain(int points[], const int *arrSize, float density, float restitution, float friction)
//{
//	b2BodyDef bodyDef;
//	bodyDef.type = b2_dynamicBody;
//	bodyDef.position.Set(PIXEL_TO_METERS(App->input->GetMouseX()), PIXEL_TO_METERS(App->input->GetMouseY()));
//
//	b2Body* chain = world->CreateBody(&bodyDef);
//
//	arrSize = (const int*)(sizeof(points) / sizeof(*points));
//
//	b2Vec2 vecBody[arrSize];
//	for (int i = 0; i < *arrSize / 2; i++)
//	{
//		vecBody[i].Set(PIXEL_TO_METERS(points[2 * i]), PIXEL_TO_METERS(points[2 * i + 1]));
//	}
//
//	b2ChainShape chainBody;
//	chainBody.CreateLoop(vecBody, *arrSize / 2);
//
//	b2FixtureDef fixture;
//	fixture.shape = &chainBody;
//	fixture.density = density;
//	fixture.restitution = restitution;
//	fixture.friction = friction;
//
//	chain->CreateFixture(&fixture);
//
//}