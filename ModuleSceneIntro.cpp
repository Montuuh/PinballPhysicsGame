#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "ModuleWindow.h"


#include "SDL/include/SDL.h"


ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	circle = box = rick = NULL;
}

ModuleSceneIntro::~ModuleSceneIntro()
{}


// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->renderer->camera.x = 0;
	App->renderer->camera.y = 0;

	backgroundBlue = App->textures->Load("Assets/backgroundBlue.png");
	sprites = App->textures->Load("Assets/spriteSheet.png");
	gameOver = App->textures->Load("Assets/gameOver.png");

	smallWall = App->physics->CreateRectangle(214, 305, 9, 24, 0, b2_staticBody, 0);

	//Sensors
	sensor100points = App->physics->CreateRectangleSensor(114, 80, 12, 4, 172);
	sensor500points1 = App->physics->CreateRectangleSensor(160, 60, 12, 4, 0);
	sensor500points2 = App->physics->CreateRectangleSensor(127, 60, 12, 4, 0);
	sensor1000points = App->physics->CreateRectangleSensor(144, 60, 12, 4, 0);

	sensorreset = App->physics->CreateRectangleSensor(145, SCREEN_HEIGHT, 50, 4, 0);

	sensorcoin1 = App->physics->CreateRectangleSensor(89, 103, 5, 5, 0);
	sensorcoin2 = App->physics->CreateRectangleSensor(86, 95, 5, 5, 0);
	sensorcoin3 = App->physics->CreateRectangleSensor(83, 87, 5, 5, 0);
	sensorcoin4 = App->physics->CreateRectangleSensor(80, 78, 5, 5, 0);
	sensorcoin5 = App->physics->CreateRectangleSensor(80, 68, 5, 5, 0);
	sensorcoin6 = App->physics->CreateRectangleSensor(82, 58, 5, 5, 0);
	sensorcoin7 = App->physics->CreateRectangleSensor(88, 50, 5, 5, 0);
	sensorcoin8 = App->physics->CreateRectangleSensor(93, 43, 5, 5, 0);

	//Card physBody definitions
	card10 = App->physics->CreateRectangleSensor(113, 291, 12, 4, 0);
	cardJ = App->physics->CreateRectangleSensor(129, 291, 12, 4, 0);
	cardQ = App->physics->CreateRectangleSensor(145, 291, 12, 4, 0);
	cardK = App->physics->CreateRectangleSensor(161, 291, 12, 4, 0);
	cardA = App->physics->CreateRectangleSensor(177, 291, 12, 4, 0);

	//Spring coords
	spring1.x = 102; 
	spring1.y = 129;
	spring1.h = 44;
	spring1.w = 9;
	
	spring2.x = 115;
	spring2.y = 129;
	spring2.h = 44;
	spring2.w = 9;

	spring3.x = 127;
	spring3.y = 129;
	spring3.h = 44;
	spring3.w = 10;

	spring4.x = 140;
	spring4.y = 129;
	spring4.h = 44;
	spring4.w = 9;

	spring5.x = 152;
	spring5.y = 129;
	spring5.h = 44;
	spring5.w = 9;

	coinrect.x = 152;
	coinrect.y = 368;
	coinrect.h = coinrect.w = 5;

	//Initial ball
	circles.add(App->physics->CreateCircle(225, 390, 5.5, b2_dynamicBody, 0));
	circles.getFirst()->data->body->SetBullet(true);
	circles.getLast()->data->listener = this;


	//Paddles coords
	leftPaddles.add(App->physics->CreatePaddleLeft(108, 429, 40 * DEGTORAD, -30 * DEGTORAD));
	leftPaddles.add(App->physics->CreatePaddleLeft(109, 201, 40 * DEGTORAD, -30 * DEGTORAD));
	rightPaddles.add(App->physics->CreatePaddleRight(180, 430, 150 * DEGTORAD, 80 * DEGTORAD));
	rightPaddles.add(App->physics->CreatePaddleRight(180, 203, 150 * DEGTORAD, 80 * DEGTORAD));

	springBouncer = App->physics->CreateRectangle(227, 425, 2, 9, 0, b2_dynamicBody, 0);
	bouncerJoint = App->physics->CreateRectangle(227, 450, 10, 0, 0, b2_staticBody, 0);
	App->physics->CreateLineJoint(springBouncer->body, bouncerJoint->body, p2Point<float>(0, 0), p2Point<float>(0, 0), 30.0f, 0.0f);

	// Pink balls coords and size
	pinkball_big.h = 24;
	pinkball_big.w = 22;
	pinkball_small.h = 18;
	pinkball_small.w = 16;

	sensorpinkball1 = App->physics->CreateCircleSensor(144, 100, 11, b2_staticBody, 25, 1);
	pinkball1 = App->physics->CreateCircle(144, 100, 9, b2_staticBody, 1);
	sensorpinkball2 = App->physics->CreateCircleSensor(121, 321, 11, b2_staticBody, 25, 1);
	pinkball2 = App->physics->CreateCircle(121, 321, 9, b2_staticBody, 1);
	sensorpinkball3 = App->physics->CreateCircleSensor(144, 353, 11, b2_staticBody, 25, 1);
	pinkball3 = App->physics->CreateCircle(144, 353, 9, b2_staticBody, 1);
	sensorpinkball4 = App->physics->CreateCircleSensor(169, 321, 11, b2_staticBody, 25, 1);
	pinkball4 = App->physics->CreateCircle(169, 321, 9, b2_staticBody, 1);

	//Cannon coords
	sensorcanon1upper = App->physics->CreateCircleSensor(208, 180, 7, b2_staticBody, 1, 0);
	sensorcanon1lower = App->physics->CreateCircleSensor(208, 269, 7, b2_staticBody, 1, 0);

	//MAP COLLIDERS
	int Green_Upper_Right_Wall_coordinates[20] = 
	{
		116, 88,
		121, 84,
		124, 84,
		130, 88,
		130, 124,
		128, 127,
		125, 127,
		123, 124,
		123, 96,
		116, 88
	};
	boardItems.add(App->physics->CreateChain(69, 0, Green_Upper_Right_Wall_coordinates, 18, 0));

	int Upper_Left_Map_Collider_coordinates[28] = 
	{
		37, 43,
		51, 57,
		51, 71,
		35, 88,
		35, 101,
		32, 103,
		29, 103,
		24, 94,
		20, 82,
		20, 63,
		23, 56,
		27, 49,
		32, 45,
		37, 43
	};
	boardItems.add(App->physics->CreateChain(69, 0, Upper_Left_Map_Collider_coordinates, 26, 0));

	int Upper_Right_Map_Collider_coordinates[74] = 
	{
		149, 293,
		149, 72,
		146, 62,
		141, 52,
		136, 46,
		125, 41,
		115, 41,
		99, 57,
		99, 71,
		107, 71,
		112, 67,
		117, 65,
		128, 65,
		136, 68,
		141, 75,
		144, 83,
		144, 131,
		141, 139,
		137, 144,
		132, 147,
		132, 160,
		146, 174,
		146, 184,
		142, 188,
		135, 188,
		127, 181,
		116, 192,
		116, 200,
		123, 207,
		123, 245,
		130, 260,
		143, 260,
		146, 265,
		146, 272,
		140, 285,
		140, 293,
		149, 293
	};
	boardItems.add(App->physics->CreateChain(69, 0, Upper_Right_Map_Collider_coordinates, 72, 0));

	int Upper_Left_Paddle_Map_Collider_coordinates[20] = 
	{
		23, 181,
		35, 192,
		35, 200,
		27, 208,
		26, 257,
		24, 260,
		21, 260,
		17, 253,
		17, 187,
		23, 181
	};
	boardItems.add(App->physics->CreateChain(69, 0, Upper_Left_Paddle_Map_Collider_coordinates, 18, 0));

	int Bottom_Left_Paddle_Map_Collider_coordinates[22] = 
	{
		19, 381,
		19, 409,
		23, 414,
		30, 419,
		35, 423,
		35, 426,
		29, 421,
		22, 416,
		17, 410,
		17, 381,
		19, 381
	};
	boardItems.add(App->physics->CreateChain(69, 0, Bottom_Left_Paddle_Map_Collider_coordinates, 20, 0));

	int Bottom_Right_Paddle_Map_Collider_coordinates[22] = 
	{
		133, 381,
		133, 409,
		130, 414,
		123, 420,
		115, 426,
		115, 423,
		122, 418,
		128, 413,
		131, 409,
		131, 381,
		133, 381
	};
	boardItems.add(App->physics->CreateChain(69, 0, Bottom_Right_Paddle_Map_Collider_coordinates, 20, 0));

	//BOARD ITEM COLLIDERS
	int Pink_Right_Bouncer_coordinates[16] = 
	{
		118, 382,
		118, 401,
		115, 405,
		110, 409,
		107, 407,
		116, 383,
		117, 381,
		118, 382
	};
	boardItems.add(App->physics->CreateChain(69, 0, Pink_Right_Bouncer_coordinates, 14, 0.5));

	int Pink_Left_Bouncer_coordinates[14] = 
	{
		34, 381,
		42, 406,
		41, 409,
		36, 406,
		33, 401,
		33, 382,
		34, 381
	};
	boardItems.add(App->physics->CreateChain(69, 0, Pink_Left_Bouncer_coordinates, 12, 0.5));

	int Top_Separator_1_coordinates[14] = 
	{
		68, 57,
		68, 58,
		68, 65,
		68, 66,
		67, 65,
		67, 58,
		68, 57
	};
	boardItems.add(App->physics->CreateChain(69, 0, Top_Separator_1_coordinates, 12, 0));

	int Top_Separator_2_coordinates[14] = 
	{
		84, 57,
		85, 58,
		85, 65,
		84, 66,
		83, 65,
		83, 58,
		84, 57
	};
	boardItems.add(App->physics->CreateChain(68, 0, Top_Separator_2_coordinates, 12, 0));

	int Middle_Separator_1_coordinates[14] = 
	{
		36, 285,
		37, 286,
		37, 294,
		36, 296,
		35, 294,
		35, 286,
		36, 285
	};
	boardItems.add(App->physics->CreateChain(69, 0, Middle_Separator_1_coordinates, 12, 0));

	int Middle_Separator_2_coordinates[14] = 
	{
		52, 285,
		53, 286,
		53, 295,
		52, 296,
		51, 295,
		51, 286,
		52, 285
	};
	boardItems.add(App->physics->CreateChain(68, 0, Middle_Separator_2_coordinates, 12, 0));

	int Middle_Separator_3_coordinates[14] = 
	{
		68, 285,
		69, 286,
		69, 295,
		68, 296,
		67, 295,
		67, 286,
		68, 285
	};
	boardItems.add(App->physics->CreateChain(69, 0, Middle_Separator_3_coordinates, 12, 0));

	int Middle_Separator_4_coordinates[14] = 
	{
		84, 285,
		85, 286,
		85, 295,
		84, 296,
		83, 295,
		83, 286,
		84, 285
	};
	boardItems.add(App->physics->CreateChain(69, 0, Middle_Separator_4_coordinates, 12, 0));

	int Middle_Separator_5_coordinates[14] = 
	{
		100, 285,
		101, 286,
		101, 295,
		100, 296,
		99, 295,
		99, 286,
		100, 285
	};
	boardItems.add(App->physics->CreateChain(68, 0, Middle_Separator_5_coordinates, 12, 0));

	int Middle_Separator_6_coordinates[14] = 
	{
		116, 285,
		117, 286,
		117, 295,
		116, 296,
		115, 295,
		115, 286,
		116, 285
	};
	boardItems.add(App->physics->CreateChain(69, 0, Middle_Separator_6_coordinates, 12, 0));

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
	realTime = SDL_GetTicks();

	App->renderer->Blit(backgroundBlue, 0, 0);

	// Spring Animation
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		currentTime = realTime;
	}
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_IDLE)
	{
		springAnimation = false;
	}
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT)
	{
		int animation_time = 450;
		springAnimation = true;

		if (realTime < currentTime + (animation_time / 4)) 
		{
			App->renderer->Blit(sprites, 220, 401, &spring1);
		}
		if ((realTime > currentTime + (animation_time / 4)) && (realTime < currentTime + 2 * (animation_time / 4))) 
		{
            App->renderer->Blit(sprites, 220, 401, &spring2);
		}
    	if ((realTime > currentTime + 2 * (animation_time / 4)) && (realTime < currentTime + 3 * (animation_time / 4))) 
		{
			App->renderer->Blit(sprites, 219, 401, &spring3);
		}
		if ((realTime > currentTime + 3 * (animation_time / 4)) && (realTime < currentTime + animation_time)) 
		{
			App->renderer->Blit(sprites, 220, 401, &spring4);
		}
		if (realTime > currentTime + animation_time) 
		{
			App->renderer->Blit(sprites, 220, 401, &spring5);
		}
	}

	//coins
	if (sensorcoin1triggered == false) 
	{
		App->renderer->Blit(sprites, 87, 101, &coinrect);
	}
	else 
	{
		sensorcoin1->body->SetActive(false);
	}
	if (sensorcoin2triggered == false) 
	{
		App->renderer->Blit(sprites, 84, 93, &coinrect);
	}
	else 
	{
		sensorcoin2->body->SetActive(false);
	}
	if (sensorcoin3triggered == false) {
		App->renderer->Blit(sprites, 81, 85, &coinrect);
	}
	else 
	{
		sensorcoin3->body->SetActive(false);
	}
	if (sensorcoin4triggered == false) 
	{
		App->renderer->Blit(sprites, 78, 76, &coinrect);
	}
	else 
	{
		sensorcoin4->body->SetActive(false);
	}
	if (sensorcoin5triggered == false) 
	{
		App->renderer->Blit(sprites, 78, 66, &coinrect);
	}
	else 
	{
		sensorcoin5->body->SetActive(false);
	}
	if (sensorcoin6triggered == false) {
		App->renderer->Blit(sprites, 80, 56, &coinrect);
	}
	else 
	{
		sensorcoin6->body->SetActive(false);
	}
	if (sensorcoin7triggered == false) {
		App->renderer->Blit(sprites, 86, 48, &coinrect);
	}
	else 
	{
		sensorcoin7->body->SetActive(false);
	}
	if (sensorcoin8triggered == false) {
		App->renderer->Blit(sprites, 91, 41, &coinrect);
	}
	else 
	{
		sensorcoin8->body->SetActive(false);
	}

	//Card coords
	SDL_Rect cardDefaultRect;
	cardDefaultRect.h = 24;
	cardDefaultRect.w = 14;
	cardDefaultRect.x = 76;
	cardDefaultRect.y = 88;

	SDL_Rect cardRect;
	cardRect.h = 24;
	cardRect.w = 14;
	cardRect.x = 91;
	cardRect.y = 88;

	if (sensorcard10 == false)
	{
		App->renderer->Blit(sprites, 105, 261, &cardDefaultRect);
	}
	else
	{
		App->renderer->Blit(sprites, 105, 261, &cardRect);
	}
	cardRect.x += 15;
	if (sensorcardJ == false)
	{
		App->renderer->Blit(sprites, 121, 261, &cardDefaultRect);
	}
	else
	{
		App->renderer->Blit(sprites, 121, 261, &cardRect);
	}
	cardRect.x += 15;
	if (sensorcardQ == false)
	{
		App->renderer->Blit(sprites, 137, 261, &cardDefaultRect);
	}
	else
	{
		App->renderer->Blit(sprites, 137, 261, &cardRect);
	}
	cardRect.x += 15;
	if (sensorcardK == false)
	{
		App->renderer->Blit(sprites, 153, 261, &cardDefaultRect);
	}
	else
	{
		App->renderer->Blit(sprites, 153, 261, &cardRect);
	}
	cardRect.x += 15;
	if (sensorcardA == false)
	{
		App->renderer->Blit(sprites, 169, 261, &cardDefaultRect);
	}
	else
	{
		App->renderer->Blit(sprites, 169, 261, &cardRect);
	}
	if ((sensorcard10 == true) &&
		(sensorcardJ == true) &&
		(sensorcardQ == true) &&
		(sensorcardK == true) &&
		(sensorcardA == true))
	{
		flippedCards = true;
	}
	else
	{
		flippedCards = false;
	}
	if (flippedCards == true)
	{
		if (loadOrangeMap == false)
		{
			backgroundBlue = App->textures->Load("Assets/backgroundOrange.png");
			points += 5000;
			loadOrangeMap = true;
		}
	}

	//Spring bouncer
	static float Push = 0.0f;
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT)
	{

		Push += 40.0f;
		springBouncer->body->ApplyForceToCenter(b2Vec2(0, (Push)), true);
	}
	else
		Push = 0.0f;

	b2Vec2 pos = springBouncer->body->GetPosition();

	if (springAnimation==false)
	{
		App->renderer->Blit(sprites, 220, 401, &spring1);
	}

	//Paddle movement
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN)
	{
		App->physics->PaddleMoveLeft();
	}
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_UP)
	{
		App->physics->PaddleStopLeft();
	}
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN)
	{
		App->physics->PaddleMoveRight();
	}
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_UP)
	{
		App->physics->PaddleStopRight();
	}
	//Ball rendering
	p2List_item<PhysBody*>*  c = circles.getFirst();

	while (c != NULL) 
	{
		int x, y;
		c->data->GetPosition(x, y);
		SDL_Rect ballrect;
		ballrect.x = 94;
		ballrect.y = 342;
		ballrect.h = 12;
		ballrect.w = 11;
		App->renderer->Blit(sprites, x, y, &ballrect, 1.0f);
		c = c->next;
	}

	//Paddles rendering
	L = leftPaddles.getFirst();
	while (L != NULL)
	{
		int x, y;
		L->data->GetPosition(x, y);
		SDL_Rect PaddleLeftRect;
		PaddleLeftRect.x = 42;
		PaddleLeftRect.y = 88;
		PaddleLeftRect.h = 12;
		PaddleLeftRect.w = 34;
		App->renderer->Blit(sprites, x - 1, y + 3, &PaddleLeftRect, 1.0f, L->data->GetRotation(), 0, 0);
		L = L->next;
	}
	L = rightPaddles.getFirst();
	while (L != NULL)
	{
		int x, y;
		L->data->GetPosition(x, y);
		SDL_Rect PaddleRightRect;
		PaddleRightRect.x = 42;
		PaddleRightRect.y = 99;
		PaddleRightRect.h = 12;
		PaddleRightRect.w = 34;
		App->renderer->Blit(sprites, x - 3, y + 3, &PaddleRightRect, 1.0f, L->data->GetRotation(), 0, 0);
		L = L->next;
	}

	// Ball bouncer rendering
	int i = 0;
	int j = 0;
	pinkball_big.x = 102;
	pinkball_big.y = 178;
	pinkball_small.x = 127;
	pinkball_small.y = 184;
	if (sensorpinkball1striggered == false) 
	{
		sensorpinkball1->GetPosition(i, j);
		App->renderer->Blit(sprites, i, j, &pinkball_big);
	}
	else 
	{
		pinkball1->GetPosition(i, j);
		App->renderer->Blit(sprites, i + 1, j, &pinkball_small);
		sensorpinkball1striggered = false;
	}
	if (sensorpinkball2striggered == false) 
	{
		sensorpinkball2->GetPosition(i, j);
		App->renderer->Blit(sprites, i, j, &pinkball_big);
	}
	else 
	{
		pinkball2->GetPosition(i, j);
		App->renderer->Blit(sprites, i + 1, j, &pinkball_small);
		sensorpinkball2striggered = false;
	}
	if (sensorpinkball4striggered == false) 
	{
		sensorpinkball4->GetPosition(i, j);
		App->renderer->Blit(sprites, i, j, &pinkball_big);
	}
	else 
	{
		pinkball4->GetPosition(i, j);
		App->renderer->Blit(sprites, i + 1, j, &pinkball_small);
		sensorpinkball4striggered = false;
	}
	pinkball_big.x = 58;
	pinkball_small.x = 83;
	if (sensorpinkball3striggered == false) 
	{
		sensorpinkball3->GetPosition(i, j);
		App->renderer->Blit(sprites, i, j, &pinkball_big);
	}
	else {
		pinkball3->GetPosition(i, j);
		App->renderer->Blit(sprites, i + 1, j, &pinkball_small);
		sensorpinkball3striggered = false;
	}

	// Cannons logic and rendering
	//canons
	if (sensorcanon1uppertriggered == true) {
		if (temp_joint_created == false) {
			canoncurrenttime = realTime;
			App->physics->CreateTemporaryJoint();
			temp_joint_created = true;
		}
		if ((realTime > canoncurrenttime + 1000) && (first_joint_time == false)) {
			App->physics->DeleteTemporaryJoint();
			sensorcanon1upper->body->SetActive(false);
			circles.getLast()->data->body->ApplyForceToCenter(b2Vec2(-15.0f, -16.5f), true);
			//App->audio->PlayFx(Start_fx);
			first_joint_time = true;
		}
		if (realTime > canoncurrenttime + 1500) {
			temp_joint_created = false;
			sensorcanon1upper->body->SetActive(true);
			sensorcanon1uppertriggered = false;
			first_joint_time = false;
		}
	}
	if (sensorcanon1lowertriggered == true) {
		b2Vec2 CanonPos;
		CanonPos.x = 4.16;
		CanonPos.y = 3.59;
		circles.getLast()->data->body->SetTransform(CanonPos, 0);
		sensorcanon1lowertriggered = false;
	}

	// Some scancodes
	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{ 
		circles.add(App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 5.5, b2_dynamicBody, 0));
		circles.getLast()->data->listener = this;
	}
	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)
	{
		points = 0;
		numBalls = 3;
		reset = true;
	}

	// Reset all
	circles.getLast();
	if ((newball == true) || (reset == true))
	{
		if ((numBalls > 0) || (reset == true))
		{
			MapReset(true);
		}
		newball = false;
	}

	// Create new ball
	if (newball == true)
	{
		circles.add(App->physics->CreateCircle(225, 390, 5.5, b2_dynamicBody, 0.0f));
		circles.getLast()->data->listener = this;
		newball = false;
	}
	
	if (points > maxPoints) 
	{
		maxPoints = points;
	}

	if (numBalls <= 0)
		game_over = true;

	if (game_over)
		App->renderer->Blit(gameOver, 110, 233);

	return UPDATE_CONTINUE;
}

// Function to reset the map
void ModuleSceneIntro::MapReset(bool totalreset) {
	if (totalreset == true) {
		b2Vec2 BallInitialPos;
		BallInitialPos.x = 4.55;
		BallInitialPos.y = 7.8;
		circles.getLast()->data->body->SetTransform(BallInitialPos, 0);
		reset = false;
		sensorcard10 = false;
		sensorcardJ = false;
		sensorcardQ = false;
		sensorcardK = false;
		sensorcardA = false;
		flippedCards = false;
		//orangemaploaded = false;
		//chicken1state = 1;
		//chicken2state = 1;
		//chicken3state = 1;
		//godball1->body->SetActive(false);
		//godball2->body->SetActive(false);
		//sensorminispring1->body->SetActive(false);
		//sensorminispring2->body->SetActive(false);
		sensorcoin1triggered = false;
		sensorcoin2triggered = false;
		sensorcoin3triggered = false;
		sensorcoin4triggered = false;
		sensorcoin5triggered = false;
		sensorcoin6triggered = false;
		sensorcoin7triggered = false;
		sensorcoin8triggered = false;
		sensorcoin1->body->SetActive(true);
		sensorcoin2->body->SetActive(true);
		sensorcoin3->body->SetActive(true);
		sensorcoin4->body->SetActive(true);
		sensorcoin5->body->SetActive(true);
		sensorcoin6->body->SetActive(true);
		sensorcoin7->body->SetActive(true);
		sensorcoin8->body->SetActive(true);
		//sensorextrapoints1triggered = false;
		//sensorextrapoints2triggered = false;
		//sensorextrapoints3triggered = false;
		//sensorextrapoints4triggered = false;
		//sensorextrapoints1triggeredpoints = false;
		//sensorextrapoints2triggeredpoints = false;
		//sensorextrapoints3triggeredpoints = false;
		//sensorextrapoints4triggeredpoints = false;
		backgroundBlue = App->textures->Load("Assets/backgroundBlue.png");
		game_over = false;
	}
	//sensor1triggered = false;
	//sensor2triggered = false;
	//sensor3triggered = false;
	//sensor4triggered = false;
	//sensor5triggered = false;
	//sensor6triggered = false;
	//sensor7triggered = false;
	//sensor1triggeredpoints = false;
	//sensor2triggeredpoints = false;
	//sensor3triggeredpoints = false;
	//sensor4triggeredpoints = false;
	//sensor5triggeredpoints = false;
	//sensor6triggeredpoints = false;
	//sensor7triggeredpoints = false;
	//sensorballpassedexittriggered = false;
	//wall->body->SetActive(true);
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	if (bodyA != nullptr && bodyB != nullptr) 
	{
		if (bodyA->body != nullptr && bodyB->body != nullptr)
		{
			if (bodyA->body->GetType() == b2Shape::e_circle || bodyB->body->GetType() == b2Shape::e_circle)
			{
				// On collision with reset bar
				if (bodyA == sensorreset || bodyB == sensorreset)
				{
					newball = true;
					numBalls -= 1;
					if (numBalls < 0)
						numBalls = 0;
				}

				// On collision with pinkballs
				if (bodyA == sensorpinkball1 || bodyB == sensorpinkball1) 
				{
					//App->audio->PlayFx(Bouncing_Balls_fx);
					points += 100;
					sensorpinkball1striggered = true;
				}
				if (bodyA == sensorpinkball2 || bodyB == sensorpinkball2) 
				{
					//App->audio->PlayFx(Bouncing_Balls_fx);
					points += 100;
					sensorpinkball2striggered = true;
				}
				if (bodyA == sensorpinkball3 || bodyB == sensorpinkball3)
				{
					//App->audio->PlayFx(Bouncing_Balls_fx);
					points += 100;
					sensorpinkball3striggered = true;
				}
				if (bodyA == sensorpinkball4 || bodyB == sensorpinkball4) 
				{
					//App->audio->PlayFx(Bouncing_Balls_fx);
					points += 100;
					sensorpinkball4striggered = true;
				}

				// On collision with cards
				if (bodyA == card10 || bodyB == card10)
				{
					points += 500;
					sensorcard10 = true;
				}
				if (bodyA == cardJ || bodyB == cardJ)
				{
					points += 500;
					sensorcardJ = true;
				}
				if (bodyA == cardQ || bodyB == cardQ)
				{
					points += 500;
					sensorcardQ = true;
				}
				if (bodyA == cardK || bodyB == cardK)
				{
					points += 500;
					sensorcardK = true;
				}
				if (bodyA == cardA || bodyB == cardA)
				{
					points += 500;
					sensorcardA = true;
				}

				// On Collision with coins
				if (bodyA == sensorcoin1 || bodyB == sensorcoin1) {
					sensorcoin1triggered = true;
					points += 100;
				}
				if (bodyA == sensorcoin2 || bodyB == sensorcoin2) {
					sensorcoin2triggered = true;
					points += 100;
				}
				if (bodyA == sensorcoin3 || bodyB == sensorcoin3) {
					sensorcoin3triggered = true;
					points += 100;
				}
				if (bodyA == sensorcoin4 || bodyB == sensorcoin4) {
					sensorcoin4triggered = true;
					points += 100;
				}
				if (bodyA == sensorcoin5 || bodyB == sensorcoin5) {
					sensorcoin5triggered = true;
					points += 100;
				}
				if (bodyA == sensorcoin6 || bodyB == sensorcoin6) {
					sensorcoin6triggered = true;
					points += 100;
				}
				if (bodyA == sensorcoin7 || bodyB == sensorcoin7) {
					sensorcoin7triggered = true;
					points += 100;					
				}
				if (bodyA == sensorcoin8 || bodyB == sensorcoin8) {
					sensorcoin8triggered = true;
					points += 100;
					;
				}
				
				// On Collision with 100, 500 && 1000 sensors
				if (bodyA == sensor100points || bodyB == sensor100points)
				{
					//App->audio->PlayFx(Points_Lateral_fx);
					points += 100;
					sensor100pointstriggered = true;
				}
				if (bodyA == sensor500points1 || bodyB == sensor500points1 ||
					bodyA == sensor500points2 || bodyB == sensor500points2)
				{
					//App->audio->PlayFx(Points_Lateral_fx);
					points += 500;
				}
				if (bodyA == sensor1000points || bodyB == sensor1000points)
				{
					//App->audio->PlayFx(Points_Lateral_fx);
					points += 1000;
				}
				if (bodyA == sensorcanon1upper || bodyB == sensorcanon1upper) {
					//App->audio->PlayFx(Canon_Enter_fx);
					sensorcanon1uppertriggered = true;
				}
				if (bodyA == sensorcanon1lower || bodyB == sensorcanon1lower) {
					sensorcanon1lowertriggered = true;
				}
			}
		}
	}

}
