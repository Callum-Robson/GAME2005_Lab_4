#pragma once
#ifndef __BALL__
#define __BALL__
#include "TextureManager.h"
#include <glm/vec4.hpp>

#include "Agent.h"

class Ball final : public Agent
{
public:
	Ball(float mass = 0);
	~Ball();

	// Inherited via GameObject
	void draw() override;
	void update() override;
	void clean() override;

	void setMassKG(float mass);
	float getMassKG();

private:
	float massKG;

};


#endif /* defined (__BALL__) */

