#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{
}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	for (uint c = 0; c < 50; ++c) {
		Cube cube;
		cube.color = { 0.50f ,0.23f,0 };
		cube.size = 2;

		c_obstacles.PushBack(cube);
	}
	c_obstacles[0].size = 20;
	c_obstacles[0].SetPos(0, 4,  -200);
	c_obstacles[0].color = Red;	

	c_obstacles[1].size = 20;
	c_obstacles[1].SetPos(0, 4, - 178);
	c_obstacles[1].color = Red;

	c_obstacles[2].size = 12;
	c_obstacles[2].SetPos(0, 10, - 165);
	c_obstacles[2].color = Green;

	c_obstacles[3].size = 12;
	c_obstacles[3].SetPos(0, 12, - 155);
	c_obstacles[3].color = Green;

	c_obstacles[4].size = 7;
	c_obstacles[4].SetPos(0, 14, -145);
	c_obstacles[4].color = {0.63f, 0, 0.33f};
	c_obstacles[4].SetRotation(30.0f, vec3(1,0,0));

	c_obstacles[5].size = 5;
	c_obstacles[5].SetPos(0, 14, -135);
	c_obstacles[5].color = Green;
	c_obstacles[5].SetRotation(40.0f, vec3(1, 1, 0));

	c_obstacles[6].size = 7;
	c_obstacles[6].SetPos(0, 10, -123);
	c_obstacles[6].color = Blue;

	for (uint c = 0; c < c_obstacles.Count(); ++c) {
		PhysBody3D* body;
		body = App->physics->AddBody(c_obstacles[c], 0);
		obstacles.PushBack(body);
	}

	PhysBody3D* sens;
	Cube p(5, 1, 1000);
	p.SetPos(0, 8, 0);

	sens = App->physics->AddBody(p, 0);

	sens->SetAsSensor(true);
	sens->collision_listeners.add(this);



	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{

	Plane p(0, 1, 0, 0);
	p.axis = true;
	p.Render();


	b.Render();

	for (uint c = 0; c < 50; ++c) {
		obstacles[c]->GetTransform(c_obstacles[c].transform.M);
		c_obstacles[c].Render();
	}

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
}

