#include "Ball.h"
#include "glm/gtx/string_cast.hpp"
#include "PlayScene.h"
#include "TextureManager.h"
#include "Util.h"

Ball::Ball(float mass)
{
	TextureManager::Instance().load("../Assets/textures/ball.png", "ball");

	auto size = TextureManager::Instance().getTextureSize("ball");
	setWidth(size.x);
	setHeight(size.y);

	getTransform()->position = glm::vec2(400.0f, 300.0f);
	getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	getRigidBody()->isColliding = false;
	setType(AGENT);
	massKG = mass;

}


Ball::~Ball()
= default;

void Ball::draw()
{
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	// draw the ship
	TextureManager::Instance().draw("ball", x, y, getCurrentHeading(), 255, true);

	// draw LOS
	/*Util::DrawLine(getTransform()->position, getTransform()->position + getCurrentDirection() * getLOSDistance(), getLOSColour());*/
}


void Ball::update()
{
	/*move();
	m_checkBounds();*/
}

void Ball::clean()
{
}

void Ball::setMassKG(float mass)
{
	massKG = mass;
}

float Ball::getMassKG()
{
	return massKG;
}
