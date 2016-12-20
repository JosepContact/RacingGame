#pragma once
#include "Module.h"
#include "p2DynArray.h"
#include "Globals.h"
#include "Primitive.h"
#include "PhysBody3D.h"

#define NCUBES 20
struct PhysBody3D;
struct PhysMotor3D;

struct MBlock {
	PhysBody3D* body;
	bool backwards;
	vec3 LSpeed;

	uint id;

public:
	MBlock() : backwards(false) {
		LSpeed = { 0, 0, 0 };
	};

	void Move() {
		float trans[16];
		body->GetTransform(trans);

		if (backwards == false)
			body->SetPos(trans[12] + LSpeed.x, trans[13] + LSpeed.y, trans[14] + LSpeed.z);
		else 
			body->SetPos(trans[12] - LSpeed.x, trans[13] - LSpeed.y, trans[14] - LSpeed.z);
		
	}
};

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

	MBlock block;

};
