#include <functional>
#include <string>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/color.h>

#include "AudioHelper.hpp"
#include "GameEngine.hpp"
#include "ImageButton.hpp"
#include "Label.hpp"
#include "StartScene.hpp"
#include "Point.hpp"
#include "Ball.hpp"

void StartScene::Initialize() {
	// TODO 1 (2/2): You can imitate the 2 files: 'LoseScene.hpp', 'LoseScene.cpp' to implement your start scene.
	int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
	int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
	int halfW = w / 2;
	int halfH = h / 2;
	AddNewObject(new Engine::Image("start/background.png", 0, 0, 0, 0, 0, 0));
	Engine::ImageButton* btn, *gts;
	btn = new Engine::ImageButton("start/play_button_1.png", "start/play_button_2.png", 165, 500, 0, 0, 0.5, 0.5, "button.ogg");
	btn->SetOnClickCallback(std::bind(&StartScene::BackOnClick, this, 2));
	AddNewControlObject(btn);
	gts = new Engine::ImageButton("start/shop_button_1.png", "start/shop_button_2.png", 375, 500, 0, 0, 0.5, 0.5, "button.ogg");
	gts->SetOnClickCallback(std::bind(&StartScene::BackOnClick, this, 3));
	AddNewControlObject(gts);
	ballPos = Engine::Point(375, 435);
	ballDir = Engine::Point(0, -6);
	AudioHelper::PlayAudio("startGame.ogg");
}

void StartScene::Update(float deltatime) {
	IScene::Update(deltatime);
	if (ballPos.y > 435) {
		ballDir.y *= -1;
		ballPos.y = 435;
		AudioHelper::PlayAudio("collision.ogg");
	}
	ballDir.y += 0.2;
	ballPos = ballPos + ballDir;
}

void StartScene::Draw() const {
	IScene::Draw();
	al_draw_filled_circle(ballPos.x, ballPos.y, 10, al_map_rgb(255, 255, 255));
}

void StartScene::BackOnClick(int stage) {
	// Change to select scene.
	if (stage == 2)
		Engine::GameEngine::GetInstance().ChangeScene("play");
	else if(stage == 3)
		Engine::GameEngine::GetInstance().ChangeScene("shop");
}
