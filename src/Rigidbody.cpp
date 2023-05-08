#include "Rigidbody.h"
/*
void Rigidbody::applyGravity(float dt, Transform& transform)
{
	float force = mass * -9.81f;

	float acceleration = force / mass;

	velocity.y += acceleration * dt;

	transform.position += transform.up * velocity.y *dt;
}

void Rigidbody::addForce(Math::Vector3 force, float dt, Transform& transform) {
	force *= mass;
	
	Math::Vector3 accel = force / mass;

	velocity += accel * dt;

	transform.position += velocity * dt;
}*/