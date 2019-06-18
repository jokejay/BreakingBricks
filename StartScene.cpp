#include <functional>
#include <string>

#include "AudioHelper.hpp"
#include "GameEngine.hpp"
#include "ImageButton.hpp"
#include "Label.hpp"
#include "StartScene.hpp"
#include "Point.hpp"

void StartScene::Initialize() {
	// TODO 1 (2/2): You can imitate the 2 files: 'LoseScene.hpp', 'LoseScene.cpp' to implement your start scene.
	int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
	int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
	int halfW = w / 2;
	int halfH = h / 2;
	AddNewObject(new Engine::Image("start/background.png", 0, 0, 0, 0, 0, 0));
	Engine::ImageButton* btn, *gts;
	btn = new Engine::ImageButton("start/play_button_1.png", "start/play_button_2.png", 165, 500, 0, 0, 0.5, 0.5, "touch.ogg");
	btn->SetOnClickCallback(std::bind(&StartScene::BackOnClick, this, 2));
	AddNewControlObject(btn);
	gts = new Engine::ImageButton("start/shop_button_1.png", "start/shop_button_2.png", 375, 500, 0, 0, 0.5, 0.5, "touch.ogg");
	gts->SetOnClickCallback(std::bind(&StartScene::BackOnClick, this, 3));
	AddNewControlObject(gts);
	AudioHelper::PlayAudio("lose.wav");
}


void StartScene::BackOnClick(int stage) {
	// Change to select scene.
	if (stage == 2)
		Engine::GameEngine::GetInstance().ChangeScene("play");
	else if(stage == 3)
		Engine::GameEngine::GetInstance().ChangeScene("shop");
}
