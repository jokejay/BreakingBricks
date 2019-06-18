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
	Engine::IObject(x, y, 20, 40), speed(10), hp(hp), money(hp) {
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
	// Pre-calculate the velocity.
	if (getPlayScene()->GetState() != PlayScene::State::MOVING_BRICK)
		return;
	float remainSpeed = speed * deltaTime;
	while (remainSpeed != 0) {
		Engine::Point target = path.back() * PlayScene::BlockSize + Engine::Point(PlayScene::BlockSize / 2, PlayScene::BlockSize / 2);
		Engine::Point vec = target - Position;
		Engine::Point normalized = vec.Normalize();
		if (remainSpeed - vec.Magnitude() > 0) {
			Position = target;
			path.pop_back();
			remainSpeed -= vec.Magnitude();
		}
		else {
			Velocity = normalized * remainSpeed / deltaTime;
			remainSpeed = 0;
		}
	}
	Sprite::Update(deltaTime);
}
void Brick::Draw() const {
	Sprite::Draw();
	if (PlayScene::DebugMode) {
		// Draw collision radius.
		al_draw_circle(Position.x, Position.y, CollisionRadius, al_map_rgb(255, 0, 0), 2);
	}
}
