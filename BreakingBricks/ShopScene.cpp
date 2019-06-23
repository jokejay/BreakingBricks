#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/color.h>
#include <cmath>
#include <fstream>
#include <functional>
#include <queue>
#include <string>
#include <memory>
#include <string>
#include <random>

#include "AudioHelper.hpp"
#include "GameEngine.hpp"
#include "Group.hpp"
#include "IObject.hpp"
#include "Image.hpp"
#include "Label.hpp"
#include "ShopScene.hpp"
#include "ImageButton.hpp"
#include "LOG.hpp"

void ShopScene::Initialize() {
	AddNewObject(new Engine::Image("shop/background.png", 0, 0, 0, 0, 0, 0));
	// Should support buttons.
	AddNewControlObject(UIGroup = new Group());
	ReadDataHelper();
	ConstructUI();
}
void ShopScene::Update(float deltaTime) {
	IScene::Update(deltaTime);
}
void ShopScene::Draw() const {
	IScene::Draw();
}

void ShopScene::ConstructUI() {
	// Background
	// Text
	Engine::ImageButton* btn;
	btn = new Engine::ImageButton("play/back.png", "play/back1.png", 50, 50, 0, 0, 0.5, 0.5, "button.ogg");
	btn->SetOnClickCallback(std::bind(&ShopScene::BackOnClick, this, 1));
	UIGroup->AddNewControlObject(btn);

	UIGroup->AddNewObject(UIMoney = new Engine::Label(std::to_string(money), "square.ttf", 40, 110, 752, 255, 255, 255, 255, 0.5, 0.5));
	UIGroup->AddNewObject(new Engine::Image("play/coin.png", 40, 750, 30, 30, 0.5, 0.5));
}


void ShopScene::BackOnClick(int stage) {
	if (stage == 1) {
		Engine::GameEngine::GetInstance().ChangeScene("start");
	}
}

void ShopScene::ReadDataHelper() {
	std::string filename = std::string("resources/data.txt");
	//Read money and topWave file.
	std::fstream fp(filename, std::ios::in);
	if (!fp.is_open()) {
		fp.open(filename, std::ios::out);
		fp << 200 << std::endl << 0;
		fp.close();
		fp.open(filename, std::ios::in);
	}

	fp >> money >> top_wave;
	fp.close();
}

void ShopScene::SaveDataHelper() {
	std::string filename = std::string("resources/data.txt");
	std::fstream fp(filename, std::ios::out);
	fp << money << std::endl << top_wave;
	fp.close();

}

void ShopScene::Purchase(int cost) {
	money -= cost;
	SaveDataHelper();
}