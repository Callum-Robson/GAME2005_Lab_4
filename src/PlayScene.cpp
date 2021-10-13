#include "PlayScene.h"
#include "Game.h"
#include "EventManager.h"

// required for IMGUI
#include "imgui.h"
#include "imgui_sdl.h"
#include "Renderer.h"
#include "Util.h"

PlayScene::PlayScene()
{
	PlayScene::start();
}

PlayScene::~PlayScene()
= default;

void PlayScene::draw()
{
	drawDisplayList();

	SDL_SetRenderDrawColor(Renderer::Instance().getRenderer(), 0, 0, 0, 255);
	SDL_RenderDrawLineF(Renderer::Instance().getRenderer(), 0, groundPosition, 800, groundPosition);

	SDL_SetRenderDrawColor(Renderer::Instance().getRenderer(), 255, 0, 0, 255);
	SDL_RenderDrawLineF(Renderer::Instance().getRenderer(), m_pTennisBall->getTransform()->position.x, m_pTennisBall->getTransform()->position.y
		, m_pTennisBall->getTransform()->position.x + vx, m_pTennisBall->getTransform()->position.y + vy);

	SDL_SetRenderDrawColor(Renderer::Instance().getRenderer(), 0, 0, 255, 255);
	SDL_RenderDrawLineF(Renderer::Instance().getRenderer(), m_pTennisBall->getTransform()->position.x, m_pTennisBall->getTransform()->position.y,
		m_pTennisBall->getTransform()->position.x, m_pTennisBall->getTransform()->position.y + acceleration);


	SDL_SetRenderDrawColor(Renderer::Instance().getRenderer(), 255, 255, 255, 255);
}

void PlayScene::update()
{
	m_pLabelAngle->setText("Angle: " + std::to_string(launchAngleDeg));
	m_pLabelVelocity->setText("Velocity (" + std::to_string(vx) + "," + std::to_string(vy * -1) + ")");
	m_pLabelPosition->setText(("Position: " + std::to_string(static_cast<int>(m_pTennisBall->getTransform()->position.x)) + ", " +
		std::to_string(static_cast<int>(m_pTennisBall->getTransform()->position.y))));
	m_pLabelDisplacement->setText(("X Displacement: " + std::to_string(m_pTennisBall->getTransform()->position.x - startPos.x)));

	float dt = Game::Instance().getDeltaTime();
	t += dt;
	updateDisplayList();



	if (launched == true)
	{
		if (m_pTennisBall->getTransform()->position.y > groundPosition)
		{
			vy = 0;
			acceleration = 0;
		}
		else
			vy += (gravity * dt);


		m_pTennisBall->getTransform()->position.x += (vx * dt);

		if (m_pTennisBall->getTransform()->position.y <= groundPosition)
		{
			m_pTennisBall->getTransform()->position.y += (vy * dt);
		}
	}
}

void PlayScene::clean()
{
	removeAllChildren();
}

void PlayScene::handleEvents()
{
	EventManager::Instance().update();
	float dt = Game::Instance().getDeltaTime();
	t += dt;


	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_ESCAPE))
	{
		TheGame::Instance().quit();
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_1))
	{
		TheGame::Instance().changeSceneState(START_SCENE);
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_2))
	{
		TheGame::Instance().changeSceneState(END_SCENE);
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_SPACE))
	{
		std::cout << "launch" << std::endl;
		throwBall();
	}
}

void PlayScene::start()
{
	// Set GUI Title
	m_guiTitle = "Play Scene";

	m_pTennisBall = new Ball();
	addChild(m_pTennisBall);
	m_pTennisBall->getTransform()->position.x = startPos.x;
	m_pTennisBall->getTransform()->position.y = startPos.y;

	// Back Button
	m_pStartButton = new Button("../Assets/textures/startButton.png", "startButton", START_BUTTON);
	m_pStartButton->getTransform()->position = glm::vec2(400.0f, 400.0f);
	m_pStartButton->addEventListener(CLICK, [&]()-> void
	{
			m_pStartButton->setActive(false);
			throwBall();
	});

	m_pStartButton->addEventListener(MOUSE_OVER, [&]()->void
	{
			m_pStartButton->setAlpha(128);
	});

	m_pStartButton->addEventListener(MOUSE_OUT, [&]()->void
	{
			m_pStartButton->setAlpha(255);
	});
	addChild(m_pStartButton);


	const SDL_Color black = { 0, 0, 0, 255 };

	/* Instructions Label */
	m_pInstructionsLabel = new Label("Press the backtick (`) character to toggle Debug View", "Consolas");
	m_pInstructionsLabel->getTransform()->position = glm::vec2(Config::SCREEN_WIDTH * 0.5f, 500.0f);
	addChild(m_pInstructionsLabel);

	m_pInstructionsLabel = new Label("Press SPACE to launch", "Consolas", 30);
	m_pInstructionsLabel->getTransform()->position = glm::vec2(Config::SCREEN_WIDTH * 0.5f, 550.0f);
	addChild(m_pInstructionsLabel);

	m_pLabelAngle = new Label("Angle: " + std::to_string(launchAngleDeg), "Consolas", 20, black, glm::vec2(690.0f, 50.0f));
	m_pLabelAngle->setParent(this);
	addChild(m_pLabelAngle);

	m_pLabelVelocity = new Label("Velocity(" + std::to_string(vx) + "," + std::to_string(vy) + ")", "Consolas", 20, black, glm::vec2(625.0f, 75.0f));
	m_pLabelVelocity->setParent(this);
	addChild(m_pLabelVelocity);

	m_pLabelPosition = new Label("Position: " + std::to_string(static_cast<int>(m_pTennisBall->getTransform()->position.x)) + ", " +
		std::to_string(static_cast<int>(m_pTennisBall->getTransform()->position.y)), "Consolas", 20, black, glm::vec2(695.0f, 100.0f));
	m_pLabelPosition->setParent(this);
	addChild(m_pLabelPosition);

	m_pLabelDisplacement = new Label("X Displacement: " + std::to_string(m_pTennisBall->getTransform()->position.x - startPos.x), "Consolas",
		20, black, glm::vec2(695.0f, 125.0f));
	m_pLabelDisplacement->setParent(this);
	addChild(m_pLabelDisplacement);


	ImGuiWindowFrame::Instance().setGUIFunction(std::bind(&PlayScene::GUI_Function, this));


	std::cout << vy << std::endl;
}

void PlayScene::GUI_Function()
{
	// Always open with a NewFrame
	ImGui::NewFrame();

	// See examples by uncommenting the following - also look at imgui_demo.cpp in the IMGUI filter
	//ImGui::ShowDemoWindow();
	
	ImGui::Begin("Controls", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoMove);

	ImGui::SliderFloat("Angle", &launchAngleDeg, -360.0f, 360.0f, "%.1f");
	ImGui::SliderFloat("Speed", &speedPixelsPerSec, 0.0f, 200.0f, "%.1f");
	ImGui::SliderFloat("Ball Position.x", &m_pTennisBall->getTransform()->position.x, 0.0f, 800.0f, "%.1f");
	ImGui::SliderFloat("Ball Position.y", &m_pTennisBall->getTransform()->position.y, 0.0f, 600.0f, "%.1f");
	ImGui::SliderFloat("Ground Position", &groundPosition, 0.0f, 600.0f, "%.1f");
	ImGui::SliderFloat("massKG", &massKG, 0.0f, 100.0f, "%.1f");
	ImGui::SliderFloat("Acceleration Gravity", &gravity, 0.0f, 100.0f, "%.1f");


	if(ImGui::Button("Launch"))
	{
		std::cout << "My Button Pressed" << std::endl;

		throwBall();
	}

	
	
	ImGui::End();
}

void PlayScene::throwBall()
{
	launched = false;
	if (launched == false)
	{
		const float DEG_TO_RADIANS = (double)M_PI / (double)180.0;
		launchAngleRad = launchAngleDeg * DEG_TO_RADIANS;
		vx = cos(-launchAngleRad) * speedPixelsPerSec;
		vy = sin(-launchAngleRad) * speedPixelsPerSec;
		acceleration = gravity;
	}
	launched = true;
	m_pTennisBall->getTransform()->position.x = 150;
	m_pTennisBall->getTransform()->position.y = 300;

}
