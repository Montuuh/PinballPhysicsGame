#pragma once
#include "Module.h"
#include "p2List.h"
#include "Globals.h"

class PhysBody;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update();
	bool CleanUp();
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);

public:

	SDL_Texture* circle;
	SDL_Texture* box;
	SDL_Texture* rick;

	SDL_Texture* backgroundBlue;
	SDL_Texture* sprites;
	SDL_Texture* gameOver;

	p2List<PhysBody*> boardItems;

	PhysBody* sensor100points;
	PhysBody* sensor500points1;
	PhysBody* sensor500points2;
	PhysBody* sensor1000points;
	PhysBody* sensorreset;
	bool sensor100pointstriggered = false;

	PhysBody* smallWall;
	
	int realTime = 0;
	int currentTime = 0;
	bool springAnimation = false;
	SDL_Rect spring1;
	SDL_Rect spring2;
	SDL_Rect spring3;
	SDL_Rect spring4;
	SDL_Rect spring5;
	PhysBody* springBouncer;
	PhysBody* bouncerJoint;

	bool newball = false;
	bool reset = false;

	


	p2List<PhysBody*> circles;

	p2List<PhysBody*> rightPaddles;
	p2List<PhysBody*> leftPaddles;

	p2List_item<PhysBody*>* L;


	uint points = 0;
	uint maxPoints = 0;
	int numBalls = 3;




};
