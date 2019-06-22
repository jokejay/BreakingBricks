#include <allegro5/allegro.h>
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
#include "PlayScene.hpp"
#include "Sprite.hpp"

#include "Brick.hpp"
#include "Ball.hpp"

bool PlayScene::DebugMode = false;
const std::vector<Engine::Point> PlayScene::directions = { Engine::Point(-1, 0), Engine::Point(0, -1), Engine::Point(1, 0), Engine::Point(0, 1) };
const int PlayScene::StartY = 166+0.5*Brick::boldness, PlayScene::EndY = 700, PlayScene::StartX = 12+0.5*Brick::boldness;
const int PlayScene::BlockWidth = 60- Brick::boldness, PlayScene::BlockHeight = 60 - Brick::boldness;
const int PlayScene::MapWidth = 7, PlayScene::MapHeight = 7;
const int PlayScene::BlockTotalW = PlayScene::BlockWidth + Brick::boldness + 6;
const std::vector<int> PlayScene::code = { ALLEGRO_KEY_UP, ALLEGRO_KEY_UP, ALLEGRO_KEY_DOWN, ALLEGRO_KEY_DOWN,
									ALLEGRO_KEY_LEFT, ALLEGRO_KEY_RIGHT, ALLEGRO_KEY_LEFT, ALLEGRO_KEY_RIGHT,
									ALLEGRO_KEY_B, ALLEGRO_KEY_A, ALLEGRO_KEYMOD_SHIFT, ALLEGRO_KEY_ENTER };
void PlayScene::Initialize() {
	// TODO 5 (1/2): There's a bug in this file, which crashes the game when you win. Try to find it.
	// TODO 5 (2/2): There's a cheat code in this file. Try to find it.
	ticks = 0;
	balls = 1;
	money = 150;
	SpeedMult = 1;
	wave = 1;
	MotherPosition = Engine::Point(240, EndY);
	MotherDirection = Engine::Point(0, -1);
	// Add groups from bottom to top.
	AddNewObject(new Engine::Image("play/background.png", 0, 0, 0, 0, 0, 0));
	AddNewObject(BallGroup = new Group());
	AddNewObject(BrickGroup = new Group());
	AddNewObject(DebugIndicatorGroup = new Group());
	AddNewObject(EffectGroup = new Group());
	// Should support buttons.
	AddNewControlObject(UIGroup = new Group());
	ConstructUI();
	BallGroup->AddNewObject(new Ball(10, 1, MotherPosition, MotherDirection, 10));
	AudioHelper::PlayBGM("play.ogg");
}
void PlayScene::Update(float deltaTime) {
	// If we use deltaTime directly, then we might have Bullet-through-paper problem.
	// Reference: Bullet-Through-Paper
	for (int i = 0; i < SpeedMult; i++) {
		IScene::Update(deltaTime);
	}
	if (cur_State == State::GENERATING_BRICK) {
		wave += 1;
		std::random_device rd;
		std::default_random_engine gen = std::default_random_engine(rd());
		std::uniform_int_distribution<int> dis(0, 1);

		for (int i = 0; i < MapWidth; i++) {
			if(dis(gen))
				BrickGroup->AddNewObject(new Brick(StartX + i * BlockTotalW, StartY - BlockHeight - 6, wave));
		}
		cur_State = SET_ANGLE;
	}
	
}
void PlayScene::Draw() const {
	IScene::Draw();
}
void PlayScene::OnMouseDown(int button, int mx, int my) {
	IScene::OnMouseDown(button, mx, my);
	if (cur_State == State::SET_ANGLE) {
		down = true;
		InitPt = Engine::Point(mx, my);
	}
		
}
void PlayScene::OnMouseMove(int mx, int my) {
	IScene::OnMouseMove(mx, my);
	if (cur_State == State::SET_ANGLE && down) {
		FinPt = Engine::Point(mx, my);
	}

}
void PlayScene::OnMouseUp(int button, int mx, int my) {
	IScene::OnMouseUp(button, mx, my);
	if (cur_State == State::SET_ANGLE) {
		Engine::Point diff = InitPt - FinPt, velo = diff.Normalize();
		if (diff.Magnitude() < 50 || velo.y > -0.2)
			return;
		for (auto& it : BallGroup->GetObjects()) {
			Ball* ball = dynamic_cast<Ball*>(it);
			ball->Position = MotherPosition;
			ball->Velocity = velo;
		}
		cur_State = State::BALL_RUNNING;
	}
}
void PlayScene::OnKeyDown(int keyCode) {
	IScene::OnKeyDown(keyCode);
}
void PlayScene::Hit() {
	return;
}
void PlayScene::EarnMoney(int money) {
	this->money += money;
	UIMoney->Text = std::string("$") + std::to_string(this->money);
}
void PlayScene::ConstructUI() {
	// Background
	UIGroup->AddNewObject(new Engine::Image("play/sand.png", 1280, 0, 320, 832));
	// Text
	UIGroup->AddNewObject(new Engine::Label(std::string("wave ") + std::to_string(wave), "pirulen.ttf", 32, 1294, 0));
	UIGroup->AddNewObject(UIMoney = new Engine::Label(std::string("$") + std::to_string(money), "pirulen.ttf", 24, 1294, 48));
}

void PlayScene::UIBtnClicked(int id) {
	return;
}

