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
#include "PlayScene.hpp"
#include "Sprite.hpp"
#include "ImageButton.hpp"
#include "LOG.hpp"

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
	wave = 0;
	cur_State = State::GENERATING_BRICK;
	MotherPosition = Engine::Point(240, EndY - 10);
	MotherDirection = Engine::Point(0, -1);
	// Add groups from bottom to top.
	AddNewObject(new Engine::Image("play/background.png", 0, 0, 0, 0, 0, 0));
	AddNewObject(BallGroup = new Group());
	AddNewObject(BrickGroup = new Group());
	AddNewObject(DebugIndicatorGroup = new Group());
	AddNewObject(EffectGroup = new Group());
	// Should support buttons.
	AddNewControlObject(UIGroup = new Group());
	ReadDataHelper();
	ConstructUI();
	BallGroup->AddNewObject(new Ball(8, 1, MotherPosition, MotherDirection, 10));
}
void PlayScene::Update(float deltaTime) {
	// If we use deltaTime directly, then we might have Bullet-through-paper problem.
	// Reference: Bullet-Through-Paper
	for (int i = 0; i < SpeedMult; i++) {
		IScene::Update(deltaTime);
	}
	if (cur_State == State::GENERATING_BRICK) {
		wave += 1;
		UIWave->Text = std::to_string(wave);
		if (wave > top_wave) {
			top_wave = wave;
			UITop->Text = std::to_string(top_wave);
		}
		std::random_device rd;
		std::default_random_engine gen = std::default_random_engine(rd());
		std::uniform_int_distribution<int> dis(0, 1);

		for (int i = 0; i < MapWidth; i++) {
			if(dis(gen))
				BrickGroup->AddNewObject(new Brick(StartX + i * BlockTotalW, StartY - BlockHeight - 6, wave));
		}
		cur_State = MOVING_BRICK;
	}
	else if (cur_State == State::MOVING_BRICK) {
		bool finish_moving = true;
		for (auto& it : BrickGroup->GetObjects()) {
			Brick* brick = dynamic_cast<Brick*>(it);
			if (brick->Moving) {
				finish_moving = false;
				break;
			}
		}
		if (finish_moving) {
			bool get_end = false;
			for (auto& it : BrickGroup->GetObjects()) {
				Brick* brick = dynamic_cast<Brick*>(it);
				if (brick->Position.y + 2 * BlockHeight > EndY) {
					get_end = true;
					break;
				}
			}
			if (!get_end)
				cur_State = SET_ANGLE;
		}
	}
	
}
void PlayScene::Draw() const {
	IScene::Draw();
	if (down) {
		Engine::Point diff = InitPt - FinPt, velo = diff.Normalize();
		if (diff.Magnitude() < 50 || velo.y > -0.2)
			return;
		al_draw_line(MotherPosition.x, MotherPosition.y, MotherPosition.x + velo.x * 100, MotherPosition.y + velo.y*100, 
			al_map_rgba(255, 255, 255, 150), 5);
	}
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
	down = false;
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
	UIMoney->Text = std::to_string(this->money);
}
void PlayScene::ConstructUI() {
	// Background
	// Text
	Engine::ImageButton* btn;
	btn = new Engine::ImageButton("play/back.png", "play/back.png", 50, 50, 0, 0, 0.5, 0.5, "button.ogg");
	btn->SetOnClickCallback(std::bind(&PlayScene::BackOnClick, this, 1));
	UIGroup -> AddNewControlObject(btn);

	UIGroup->AddNewObject(UIWave = new Engine::Label(std::to_string(wave), "square.ttf", 70, 240, 52, 255, 255, 255, 255, 0.5, 0.5));
	UIGroup->AddNewObject(new Engine::Label(std::string("TOP"), "square.ttf", 30, 415, 25, 255, 255, 255, 255, 0.5, 0.5));
	UIGroup->AddNewObject(UITop = new Engine::Label(std::to_string(top_wave), "square.ttf", 48, 415, 66, 255, 255, 255, 255, 0.5, 0.5));
	UIGroup->AddNewObject(UIMoney = new Engine::Label(std::to_string(money), "square.ttf", 48, 125, 752, 255, 255, 255, 255, 0.5, 0.5));
	UIGroup->AddNewObject(new Engine::Image("play/coin.png", 40, 750, 50, 50, 0.5, 0.5));
}

void PlayScene::UIBtnClicked(int id) {
	return;
}

void PlayScene::BackOnClick(int stage) {
	if (stage == 1) {
		Engine::GameEngine::GetInstance().ChangeScene("start");
	}
}

void PlayScene::ReadDataHelper() {
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

PlayScene::~PlayScene() {
	std::string filename = std::string("resources/data.txt");
	std::fstream fp(filename, std::ios::out);
	fp << money << std::endl << top_wave;
	fp.close();
	Engine::LOG(Engine::INFO) << "saved";
}