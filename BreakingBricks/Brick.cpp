#include <allegro5/allegro_primitives.h>
#include <allegro5/color.h>
#include <cmath>
#include <random>
#include <string>

#include "AudioHelper.hpp"
#include "Brick.hpp"
#include "IObject.hpp"
#include "GameEngine.hpp"
#include "Group.hpp"
#include "IScene.hpp"
#include "PlayScene.hpp"

const int Brick::boldness = 5;
const int Brick::StartR = 211, Brick::StartG = 38, Brick::StartB = 13, Brick::EndR = 246, Brick::EndG = 236, Brick::EndB = 112;

PlayScene* Brick::getPlayScene() {
	return dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetActiveScene());
}

Brick::Brick(float x, float y, float hp) :
	Engine::IObject(x, y, PlayScene::BlockWidth, PlayScene::BlockHeight), speed(20), hp(hp), money(hp) {
	Moving = 0;
	LifeUI = new Engine::Label(std::to_string(static_cast<int>(hp)), "pirulen.ttf", 24, x+0.5*Size.x, y+0.5*Size.y, StartR, StartG, StartB, 255, 0.5, 0.5);
}
void Brick::Hit(float damage) {
	hp -= damage;
	LifeUI->Text = std::to_string(static_cast<int>(hp));
	if (hp <= 0) {
		getPlayScene()->EarnMoney(money);
		getPlayScene()->BrickGroup->RemoveObject(objectIterator);
		AudioHelper::PlayAudio("explosion.wav");
	}
}
void Brick::Update(float deltaTime) {
		Position.y = PlayScene::StartY + (getPlayScene()->wave - money) * (Size.y + boldness + 6);
		LifeUI->Position.x = Position.x + 0.5 * Size.x;
		LifeUI->Position.y = Position.y + 0.5 * Size.y;
}
void Brick::Draw() const {
	const static int StartR = 211, StartG = 38, StartB = 13, EndR = 246, EndG = 236, EndB = 112;
	int DiffR = EndR - StartR, DiffG = EndG - StartG, DiffB = EndB - StartB;
	float Diff = 1 - hp / money;
	al_draw_rectangle(Position.x, Position.y, Position.x + Size.x, Position.y + Size.y,
		al_map_rgb(StartR + Diff * DiffR, StartG + Diff * DiffG, StartB + Diff * DiffB), boldness);
	LifeUI->Draw();
}
