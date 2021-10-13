#include "ExampleSprite.h"

#include "TextureManager.h"

ExampleSprite::ExampleSprite()
{
	TextureManager::Instance().load("../Assets/textures/soccerball.png", "soccerball");

	auto size = TextureManager::Instance().getTextureSize("soccerball");
	setWidth(size.x);
	setHeight(size.y);
}

ExampleSprite::~ExampleSprite()
= default;

void ExampleSprite::draw()
{
	TextureManager::Instance().draw("soccerball",
		getTransform()->position.x, getTransform()->position.y, 0, 255, true);
}

void ExampleSprite::update()
{
}

void ExampleSprite::clean()
{
}
