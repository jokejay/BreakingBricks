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
const int Brick::StartR = 232, Brick::StartG = 32, Brick::StartB = 105, Brick::EndR = 246, Brick::EndG = 236, Brick::EndB = 112;

PlayScene* Brick::getPlayScene() {
	return dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetActiveScene());
}

Brick::Brick(float x, float y, float hp) :
	Engine::IObject(x, y, PlayScene::BlockWidth, PlayScene::BlockHeight), speed(20), hp(hp), money(hp) {
	Moving = 0;
	shock = 0;
	diff = 0;
	LifeUI = new Engine::Label(std::to_string(static_cast<int>(hp)), "square.ttf", 20, x+0.5*Size.x, y+0.5*Size.y, StartR, StartG, StartB, 255, 0.5, 0.5);
}
void Brick::Hit(float damage) {
	hp -= damage;
	shock += 2;
	LifeUI->Text = std::to_string(static_cast<int>(hp));
	AudioHelper::PlayAudio("collision.ogg");
	if (hp <= 0) {
		getPlayScene()->EarnMoney(money);
		getPlayScene()->BrickGroup->RemoveObject(objectIterator);
	}
}
void Brick::Update(float deltaTime) {
		float TargetY = PlayScene::StartY + (getPlayScene()->wave - money) * (Size.y + boldness + 6);
		Moving = false;
		if (Position.y < TargetY) {
			Position.y += 10;
			Moving = true;
		}
		if (Position.y > TargetY) {
			Position.y = TargetY;
			Moving = true;
		}
			
		LifeUI->Position.x = Position.x + 0.5 * Size.x;
		LifeUI->Position.y = Position.y + 0.5 * Size.y;

		if (shock > 0)
			shock--;
		diff = 1 - hp / getPlayScene()->wave;
}
void Brick::Draw() const {
	if (shock > 0) {
		return;
	}
	int DiffR = EndR - StartR, DiffG = EndG - StartG, DiffB = EndB - StartB;
	
	al_draw_rectangle(Position.x, Position.y, Position.x + Size.x, Position.y + Size.y,
		al_map_rgb(StartR + diff * DiffR, StartG + diff * DiffG, StartB + diff * DiffB), boldness);
	LifeUI->Color = al_map_rgb(StartR + diff * DiffR, StartG + diff * DiffG, StartB + diff * DiffB);
	LifeUI->Draw();
}
