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
#include "cmath"

#include "LOG.hpp"

PlayScene* Ball::getPlayScene() {
	return dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetActiveScene());
}
Ball::Ball(float speed, float damage, Engine::Point position, Engine::Point velocity, float radius) :
	IObject(position.x, position.y), speed(speed), damage(damage), CollisionRadius(radius) {
	Velocity = velocity.Normalize() * speed;
	active = false;
	shock = 0;
}
void Ball::Update(float deltaTime) {
	PlayScene* scene = getPlayScene();
	// TBD: Elastic Collision if Collide with Blocks
	
	if (scene->cur_State == PlayScene::State::BALL_RUNNING) {
		if (!active) {
			//Engine::LOG(Engine::INFO) << "notactive";
			return;
		}
		if (shock > 0) {
			shock -= 1;
			return;
		}
		else if(shock == 0){
			scene->EmitBall();
			shock = -1;
		}
		auto remain_speed = speed;
		while (remain_speed > 0) {
			for (auto& it : scene->BrickGroup->GetObjects()) {
				Brick* brick = dynamic_cast<Brick*>(it);
				if (Engine::Collider::IsCircleOverlapRect(Position, CollisionRadius, brick->Position, brick->Size.x, brick->Size.y)) {
					// TBD: You should change your velocity according to the contact surface
					// If you want, you can design a func. in Collider and call it to simplfy code here
					float cx = Position.x;
					float cy = Position.y;
					Engine::Point rectp1((brick->Position.x), brick->Position.y),
						rectp2((brick->Position.x) + brick->Size.x, brick->Position.y + brick->Size.y);


					//down
					if (cx + CollisionRadius > rectp1.x && cx - CollisionRadius < rectp2.x && cy - CollisionRadius < rectp2.y && cy - CollisionRadius > rectp1.y + 0.5 * brick->Size.y) {
						Velocity.y = abs(Velocity.y);
						//Position.y = rectp2.y + CollisionRadius;
						//Engine::LOG(Engine::INFO) << "down";
					}
					//up
					else if (cx + CollisionRadius > rectp1.x && cx - CollisionRadius < rectp2.x && cy + CollisionRadius > rectp1.y && cy + CollisionRadius < rectp1.y + 0.5 * brick->Size.y) {
						Velocity.y = -abs(Velocity.y);
						//Position.y = rectp1.y - CollisionRadius;
						//Engine::LOG(Engine::INFO) << "up";
					}
					//left
					if (cx + CollisionRadius > rectp1.x && cy + CollisionRadius > rectp1.y && cy - CollisionRadius < rectp2.y && cx + CollisionRadius < rectp1.x + 0.5 * brick->Size.x) {
						Velocity.x = -abs(Velocity.x);
						//Position.x = rectp1.x - CollisionRadius;
						//Engine::LOG(Engine::INFO) << "left";
					}
					//right
					else if (cx - CollisionRadius < rectp2.x && cy + CollisionRadius > rectp1.y && cy - CollisionRadius < rectp2.y && cx - CollisionRadius > rectp1.x + 0.5 * brick->Size.x) {
						Velocity.x = abs(Velocity.x);
						//Position.x = rectp2.x + CollisionRadius;
						//Engine::LOG(Engine::INFO) << "right";
					}



					brick->Hit(damage);
					break;
				}
			}


			// TBD: If it touches the boundary...
			if (Position.x - CollisionRadius < 0) {
				// TBD: You should change the velocity 
				Velocity.x *= -1;
				Position.x = CollisionRadius;
			}
			//right
			else if (Position.x + CollisionRadius > 480) {
				Velocity.x *= -1;
				Position.x = 480 - CollisionRadius;
			}
			//up
			else if (Position.y - CollisionRadius < 100) {
				Velocity.y *= -1;
				Position.y = 100 + CollisionRadius;
			}
			//down
			else if (Position.y + CollisionRadius > 700) {
				Position.y = 700 - CollisionRadius - 1;
				if (scene->MotherPosition.y == -1) {
					scene->MotherPosition = Position;
					active = false;
					scene->ReturnBall();
					//Engine::LOG(Engine::INFO) << "return a ball 1";
					return;
				}
				else {
					Velocity = (scene->MotherPosition.x > Position.x) ? Engine::Point(2, 0) : Engine::Point(-2, 0);
				}
			}
			else if (Position.y + CollisionRadius + 1 == 700) {
				if (Velocity.x == 2) {
					if (Position.x + Velocity.x > scene->MotherPosition.x) {
						Position.x = scene->MotherPosition.x;
						active = false;
						scene->ReturnBall();
						//Engine::LOG(Engine::INFO) << "return a ball 2";
						return;
					}
				}
				else if(Velocity.x == -2){
					if (Position.x + Velocity.x < scene->MotherPosition.x) {
						Position.x = scene->MotherPosition.x;
						active = false;
						scene->ReturnBall();
						//Engine::LOG(Engine::INFO) << "return a ball 3";
						return;
					}
				}
			}

			Position = Position + Velocity;
			remain_speed -= 1;
		}
	}
}

void Ball::Draw() const {
	al_draw_filled_circle(Position.x, Position.y, CollisionRadius, al_map_rgb(255, 255, 255));
}