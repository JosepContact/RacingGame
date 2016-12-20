#pragma once
#include "Module.h"
#include "p2DynArray.h"
#include "Globals.h"
#include "Primitive.h"

#define NCUBES 20
struct PhysBody3D;
struct PhysMotor3D;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void ModuleSceneIntro::OnCollision(PhysBody3D * body1, PhysBody3D * body2);

	vec3 ondeath;

public:
	p2DynArray<PhysBody3D*> obstacles;
	p2DynArray<Cube> c_obstacles;

	PhysBody3D* sensor_fail;
	p2List<PhysBody3D*> sensors;
	PhysBody3D* checkpoints[2];

};
