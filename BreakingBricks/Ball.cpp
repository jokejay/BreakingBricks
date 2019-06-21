#include <allegro5/allegro_primitives.h>
#include <allegro5/color.h>

#include "Ball.hpp"
#include "Collider.hpp"
#include "Brick.hpp"
#include "GameEngine.hpp"
#include "Group.hpp"
#include "IObject.hpp"
#include "IScene.hpp"
#include "PlayScene.hpp"
#include "Point.hpp"
#include "Sprite.hpp"

PlayScene* Ball::getPlayScene() {
	return dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetActiveScene());
}
Ball::Ball(float speed, float damage, Engine::Point position, Engine::Point velocity, float radius) :
	IObject(position.x, position.y), speed(speed), damage(damage), CollisionRadius(radius) {
	Velocity = velocity.Normalize() * speed;
}
void Ball::Update(float deltaTime) {
	PlayScene* scene = getPlayScene();
	if (scene->cur_State == PlayScene::State::BALL_RUNNING){
		if (!Engine::Collider::IsRectOverlap(Position - Size / 2, Position + Size / 2, Engine::Point(0, 0), Engine::Point(480, 800)))
			getPlayScene()->cur_State = PlayScene::State::GENERATING_BRICK;
		Position = Position + Velocity*speed;
	}
	/*
	// TBD: Elastic Collision if Collide with Blocks
	
	if (scene->cur_State == PlayScene::State::BALL_RUNNING) {
		for (auto& it : scene->BrickGroup->GetObjects()) {
			Brick* brick = dynamic_cast<Brick*>(it);
			Engine::Point rectp1(brick->Position.x - 0.5 * brick->Size.x, brick->Position.y - 0.5 * brick->Size.y),
				rectp2(brick->Position.x + 0.5 * brick->Size.x, brick->Position.y + 0.5 * brick->Size.y);
			if (Engine::Collider::IsCircleOverlapRect(Position, CollisionRadius, rectp1, rectp2)) {
				// TBD: You should change your velocity according to the contact surface
				// If you want, you can design a func. in Collider and call it to simplfy code here
				brick->Hit(damage);
				break;
			}
		}
		// TBD: If it touches the boundary...
		if (...) {
			// TBD: You should change change the velocity 
		}
	}*/
}

void Ball::Draw() const {
	al_draw_filled_circle(Position.x, Position.y, CollisionRadius, al_map_rgb(255, 255, 255));
}