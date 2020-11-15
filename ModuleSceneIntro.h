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

public:

	SDL_Texture* circle;
	SDL_Texture* box;
	SDL_Texture* rick;
	/*PhysBody* bouncer;*/

	SDL_Texture* backgroundBlue;
	SDL_Texture* sprites;

	p2List<PhysBody*> boardItems;

	PhysBody* smallWall;
	PhysBody* springBouncer;
	PhysBody* bouncerJoint;
	
	int realTime = 0;
	int currentTime = 0;
	bool springAnimation = false;
	SDL_Rect spring1;
	SDL_Rect spring2;
	SDL_Rect spring3;
	SDL_Rect spring4;
	SDL_Rect spring5;

	


	p2List<PhysBody*> circles;

	p2List<PhysBody*> rightPaddles;
	p2List<PhysBody*> leftPaddles;

	p2List_item<PhysBody*>* L;


	uint points = 0;
	uint maxPoints = 0;
	int numBalls = 1;




};
