#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "Primitive.h"
#include "PhysVehicle3D.h"
#include "PhysBody3D.h"
#include "Timer.h"
#include "ModuleAudio.h"



ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled), vehicle(NULL)
{
	turn = acceleration = brake = 0.0f;
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");

	VehicleInfo car;

	//FX ------------
	startmotor = App->audio->LoadFx("FX/StartMotor.wav");
	gas = App->audio->LoadFx("FX/Gas.wav");
	App->audio->PlayFx(startmotor);


	// Car properties ----------------------------------------
	car.chassis_size.Set(1, 1, 3);
	car.chassis_offset.Set(0, 1.5, 0);
	car.mass = 100.0f;
	car.suspensionStiffness = 15.88f;
	car.suspensionCompression = 0.83f;
	car.suspensionDamping = 0.88f;
	car.maxSuspensionTravelCm = 1000.0f;
	car.frictionSlip = 50.5;
	car.maxSuspensionForce = 6000.0f;

	// Wheel properties ---------------------------------------
	float connection_height = 1.2f;
	float wheel_radius = 0.6f;
	float wheel_width = 0.4f;
	float suspensionRestLength = 1.2f;

	// Don't change anything below this line ------------------

	float half_width = car.chassis_size.x*0.5f;
	float half_length = car.chassis_size.z*0.5f;
	
	vec3 direction(0,-1,0);
	vec3 axis(-1,0,0);
	
	car.num_wheels = 2;
	car.wheels = new Wheel[car.num_wheels];

	// FRONT-LEFT ------------------------
	car.wheels[0].connection.Set(0, connection_height, half_length);
	car.wheels[0].direction = direction;
	car.wheels[0].axis = axis;
	car.wheels[0].suspensionRestLength = suspensionRestLength;
	car.wheels[0].radius = wheel_radius;
	car.wheels[0].width = wheel_width;
	car.wheels[0].front = true;
	car.wheels[0].drive = true;
	car.wheels[0].brake = false;
	car.wheels[0].steering = true;

	// REAR-RIGHT ------------------------
	car.wheels[1].connection.Set(0, connection_height, -half_length);
	car.wheels[1].direction = direction;
	car.wheels[1].axis = axis;
	car.wheels[1].suspensionRestLength = suspensionRestLength;
	car.wheels[1].radius = wheel_radius;
	car.wheels[1].width = wheel_width;
	car.wheels[1].front = false;
	car.wheels[1].drive = true;
	car.wheels[1].brake = true;
	car.wheels[1].steering = false;

	vehicle = App->physics->AddVehicle(car);
	vehicle->SetPos(0,15, -200);

	vehicle->GetBody()->setLinearFactor(btVector3(0, 1, 1));
	vehicle->GetBody()->setAngularFactor(btVector3(1, 0, 0));

	vehiclepoint = vehicle;
	deaths = 0;
	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	return true;
}

// Update: draw background
update_status ModulePlayer::Update(float dt)
{

	btVector3 vehicle_pos = vehicle->vehicle->getChassisWorldTransform().getOrigin();

	turn = acceleration = brake = 0.0f;
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {
		vehicle->Push(0, 100, 0);
	}
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
		vehicle->Push(0, 0, 100);
	}
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
		vehicle->Push(0, 0, -100);
	}
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
		vehicle->Push(0, -50, 0);
	}

	if(App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
	{
		acceleration = MAX_ACCELERATION;
	}

	if(App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
	{
		acceleration = -MAX_ACCELERATION;
		//brake = BRAKE_POWER;
	}
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT)
	{
		brake = BRAKE_POWER;
	}
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	{
		if (vehicle->GetBody()->getAngularVelocity().getX() < 5)
			vehicle->GetBody()->applyImpulse({ 0, -25, 0 }, { 0, 0, 3 });
	}
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	{
		if (vehicle->GetBody()->getAngularVelocity().getX() > -5)
			vehicle->GetBody()->applyImpulse({ 0, 25, 0 }, { 0, 0, 3 });
	}
	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)
	{
		
	}

	vehicle->ApplyEngineForce(acceleration);
	vehicle->Turn(turn);
	vehicle->Brake(brake);

	vehicle->Render();

	char title[80];
	sprintf_s(title, "%.1f Km/h Deaths: %d Time:%.2f Best: -", vehicle->GetKmh(), deaths, ((float)time.Read() / 1000  ) + deaths*5);
	App->window->SetTitle(title);

	btVector3 hello = vehicle->vehicle->getChassisWorldTransform().getOrigin();
	vec3 pos(vehicle->vehicle->getChassisWorldTransform().getOrigin().getX() - 40, hello.getY() + 6 , hello.getZ() -6 * vehicle->vehicle->getForwardVector().getZ());
	float camera_x = vehicle->vehicle->getChassisWorldTransform().getOrigin().getX();
	float camera_z = vehicle->vehicle->getChassisWorldTransform().getOrigin().getZ() + 15;
	float camera_y = vehicle->vehicle->getChassisWorldTransform().getOrigin().getY() + 10;
	App->camera->LookAt(vec3(camera_x, camera_y, camera_z));
	App->camera->Position = pos;


	return UPDATE_CONTINUE;
}

