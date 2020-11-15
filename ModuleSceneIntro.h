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
	void MapReset(bool totalreset);

public:

	SDL_Texture* circle;
	SDL_Texture* box;
	SDL_Texture* rick;

	SDL_Texture* backgroundBlue;
	SDL_Texture* sprites;

	SDL_Texture* gameOver;
	bool game_over;

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

	//Cards
	PhysBody* card10;
	PhysBody* cardJ;
	PhysBody* cardQ;
	PhysBody* cardK;
	PhysBody* cardA;
	bool sensorcard10 = false;
	bool sensorcardJ = false;
	bool sensorcardQ = false;
	bool sensorcardK = false;
	bool sensorcardA = false;
	bool flippedCards = false;
	bool loadOrangeMap = false;

	PhysBody* pinkball1;
	PhysBody* pinkball2;
	PhysBody* pinkball3;
	PhysBody* pinkball4;
	PhysBody* sensorpinkball1;
	PhysBody* sensorpinkball2;
	PhysBody* sensorpinkball3;
	PhysBody* sensorpinkball4;
	SDL_Rect pinkball_big;
	SDL_Rect pinkball_small;
	bool sensorpinkball1striggered = false;
	bool sensorpinkball2striggered = false;
	bool sensorpinkball3striggered = false;
	bool sensorpinkball4striggered = false;

	SDL_Rect coinrect;
	PhysBody* sensorcoin1;
	PhysBody* sensorcoin2;
	PhysBody* sensorcoin3;
	PhysBody* sensorcoin4;
	PhysBody* sensorcoin5;
	PhysBody* sensorcoin6;
	PhysBody* sensorcoin7;
	PhysBody* sensorcoin8;
	bool sensorcoin1triggered = false;
	bool sensorcoin2triggered = false;
	bool sensorcoin3triggered = false;
	bool sensorcoin4triggered = false;
	bool sensorcoin5triggered = false;
	bool sensorcoin6triggered = false;
	bool sensorcoin7triggered = false;
	bool sensorcoin8triggered = false;

	PhysBody* sensorcanon1upper;
	PhysBody* sensorcanon1lower;
	bool first_joint_time = false;
	bool temp_joint_created = false;
	bool sensorcanon1uppertriggered = false;
	bool sensorcanon1lowertriggered = false;
	int canoncurrenttime = 0;

	p2List<PhysBody*> circles;

	p2List<PhysBody*> rightPaddles;
	p2List<PhysBody*> leftPaddles;

	p2List_item<PhysBody*>* L;


	uint points = 0;
	uint maxPoints = 0;
	int numBalls = 3;

	uint music;
	uint paddle_fx;
	uint Bouncing_Balls_fx;
	uint Canon_Enter_fx;
	uint Card_fx;
	uint Chicken_Platform_fx;
	uint Coins_Tickets_ToDoor_fx;
	uint Save_Spring_fx;
	uint Start_fx;
	uint Points_Lateral_fx;



};
