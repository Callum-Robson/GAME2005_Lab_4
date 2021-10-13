#pragma once
#ifndef __PLAY_SCENE__
#define __PLAY_SCENE__

#include "Scene.h"
#include "Plane.h"
#include "Player.h"
#include "Button.h"
#include "Label.h"
#include "ExampleSprite.h"
#include "Ball.h"


class PlayScene : public Scene
{
public:
	PlayScene();
	~PlayScene();

	// Scene LifeCycle Functions
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;
	virtual void handleEvents() override;
	virtual void start() override;
	void throwBall();

private:
	bool launched = false;
	glm::vec2 startPos = glm::vec2(150, 300);
	float speedPixelsPerSec = 95;
	float launchAngleDeg = 15.9;
	float launchAngleRad;
	float gravity = 9.81;
	float acceleration = 0;
	float groundPosition = startPos.y;
	float t = 0;
	float vx;
	float vy;
	float massKG = 0;

	// IMGUI Function
	void GUI_Function();
	std::string m_guiTitle;
	
	glm::vec2 m_mousePosition;
	Ball* m_pTennisBall;

	// UI Items
	Button* m_pStartButton;

	Label* m_pInstructionsLabel;
	Label* m_pLabelAngle;
	Label* m_pLabelVelocity;
	Label* m_pLabelPosition;
	Label* m_pLabelDisplacement;


};

#endif /* defined (__PLAY_SCENE__) */