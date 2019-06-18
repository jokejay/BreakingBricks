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

PlayScene* Brick::getPlayScene() {
	return dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetActiveScene());
}

Brick::Brick(float x, float y, float hp) :
	Engine::IObject(x, y, PlayScene::BlockWidth, PlayScene::BlockHeight), speed(20), hp(hp), money(hp) {
	Moving = 0;
}
void Brick::Hit(float damage) {
	hp -= damage;
	if (hp <= 0) {
		getPlayScene()->EarnMoney(money);
		getPlayScene()->BrickGroup->RemoveObject(objectIterator);
		AudioHelper::PlayAudio("explosion.wav");
	}
}
void Brick::Update(float deltaTime) {
		Position.y = PlayScene::StartY + (getPlayScene()->wave - money) * Size.y;
}
void Brick::Draw() const {
	al_draw_rectangle(Position.x, Position.y, Position.x + Size.x, Position.y + Size.y,
		al_map_rgb(255, 255, 255), 3);
}
