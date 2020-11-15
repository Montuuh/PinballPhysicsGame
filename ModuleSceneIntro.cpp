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

	smallWall = App->physics->CreateRectangle(214, 305, 9, 24, 0, b2_staticBody, 0);

	//Sensors
	sensor100points = App->physics->CreateRectangleSensor(114, 80, 12, 4, 172);
	sensor500points1 = App->physics->CreateRectangleSensor(160, 60, 12, 4, 0);
	sensor500points2 = App->physics->CreateRectangleSensor(127, 60, 12, 4, 0);
	sensor1000points = App->physics->CreateRectangleSensor(144, 60, 12, 4, 0);

	sensorreset = App->physics->CreateRectangleSensor(145, SCREEN_HEIGHT + 10, 50, 4, 0);

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
	boardItems.add(App->physics->CreateChain(69, 0, Pink_Right_Bouncer_coordinates, 14, 1.5));

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
	boardItems.add(App->physics->CreateChain(69, 0, Pink_Left_Bouncer_coordinates, 12, 1.5));

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
	
	/*static float push = 0.0f;
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT)
	{
		push += 40.0f;
		bouncer->body->ApplyForceToCenter(b2Vec2(0, (push)), true);
	}
	else
		push = 0.0f;*/

	/*b2Vec2 pos = bouncer->body->GetPosition();*/

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

	//Padle rendering
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

	// TODO 5: Move all creation of bodies on 1,2,3 key press here in the scene
	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		/*App->physics->SpawnCircle(30.0f, 1.0f, 0.6f, 0.0f);*/
		circles.add(App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 5.5, b2_dynamicBody, 0));
		circles.getLast()->data->listener = this;
	}

	if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
	{
		// TODO 1: When pressing 2, create a box on the mouse position


		// TODO 2: To have the box behave normally, set fixture's density to 1.0f
		App->physics->SpawnRectangle(30.0f, 10.0f, 1.0f, 0.5f, 0.0f);
	}

	if (App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
	{
		// TODO 3: Create a chain shape using those vertices
		// remember to convert them from pixels to meters!
		/*SpawnChain(points, 1.0f, 0.5f, 1.0f);*/

	}

	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)
	{
		points = 0;
		numBalls = 3;
		reset = true;
	}

	circles.getLast();
	if ((newball == true) || (reset == true))
	{
		if ((numBalls > 0) || (reset == true))
		{
			b2Vec2 BallInitialPos;
			BallInitialPos.x = 4.55;
			BallInitialPos.y = 7.89;
			circles.getLast()->data->body->SetTransform(BallInitialPos, 0);
			reset = false;
		}
		newball = false;
	}

	/*if (newball == true)
	{
		circles.add(App->physics->CreateCircle(225, 390, 5.5, b2_dynamicBody, 0.0f));
		circles.getLast()->data->listener = this;
		newball = false;
	}*/
	

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	if (bodyA != nullptr && bodyB != nullptr) {
		if (bodyA->body != nullptr && bodyB->body != nullptr)
		{
			if (bodyA->body->GetType() == b2Shape::e_circle || bodyB->body->GetType() == b2Shape::e_circle)
			{
				if (bodyA == sensorreset || bodyB == sensorreset)
				{
					newball = true;
					/*numBalls -= 1;
					if (numBalls < 0) {
						numBalls = 0;*/
					
				}
			}
		}
	}

}
