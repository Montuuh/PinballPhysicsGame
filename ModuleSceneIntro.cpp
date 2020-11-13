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

	//Paddles coords
	leftPaddles.add(App->physics->CreatePaddleLeft(108, 429, 40 * DEGTORAD, -30 * DEGTORAD));
	leftPaddles.add(App->physics->CreatePaddleLeft(109, 201, 40 * DEGTORAD, -30 * DEGTORAD));
	rightPaddles.add(App->physics->CreatePaddleRight(180, 430, 152 * DEGTORAD, 82 * DEGTORAD));
	rightPaddles.add(App->physics->CreatePaddleRight(180, 203, 152 * DEGTORAD, 82 * DEGTORAD));

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
		App->physics->SpawnCircle(30.0f, 1.0f, 0.6f, 0.0f);
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

	// TODO 7: Draw all the circles using "circle" texture

	return UPDATE_CONTINUE;
}
