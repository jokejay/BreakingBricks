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

PlayScene* Ball::getPlayScene() {
	return dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetActiveScene());
}
Ball::Ball(float speed, float damage, Engine::Point position, Engine::Point velocity, float radius) :
	IObject(position.x, position.y), speed(speed), damage(damage), CollisionRadius(radius) {
	Velocity = velocity.Normalize() * speed;
	moving = false;
}
void Ball::Update(float deltaTime) {
	PlayScene* scene = getPlayScene();
	// TBD: Elastic Collision if Collide with Blocks
	
	if (scene->cur_State == PlayScene::State::BALL_RUNNING) {
		
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
				if (cx+CollisionRadius > rectp1.x && cx-CollisionRadius < rectp2.x && cy-CollisionRadius < rectp2.y && cy + CollisionRadius > rectp1.y + brick->Size.y) {
					Velocity.y = abs(Velocity.y);
					Position.y = rectp2.y + CollisionRadius;
				}
				//up
				else if (cx+ CollisionRadius > rectp1.x && cx- CollisionRadius < rectp2.x && cy+ CollisionRadius > rectp1.y && cy + CollisionRadius < rectp1.y + brick->Size.y) {
					Velocity.y = -abs(Velocity.y);
					Position.y = rectp1.y - CollisionRadius;
				}
				//left
				if (cx+ CollisionRadius > rectp1.x && cy+ CollisionRadius < rectp1.y && cy- CollisionRadius > rectp2.y && cx + CollisionRadius < rectp1.x + brick->Size.x) {
					Velocity.x = -abs(Velocity.x);
					Position.x = rectp1.x - CollisionRadius;
				}
				//right
				else if (cx- CollisionRadius < rectp2.x && cy+ CollisionRadius < rectp1.y && cy- CollisionRadius > rectp2.y && cx + CollisionRadius > rectp1.x + brick->Size.x) {
					Velocity.x = abs(Velocity.x);
					Position.x = rectp2.x + CollisionRadius;
				}
				
				brick->Hit(damage);
				break;
			}
		}
		

		// TBD: If it touches the boundary...scene->StartY
		//scene->EndY
		//0~480
		//boundary
		//left
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
			if (!moving)
				moving = true;
			else {
				getPlayScene()->cur_State = PlayScene::State::GENERATING_BRICK;
				Position.y = 700 - 2*CollisionRadius;
				moving = false;
				scene->MotherPosition = Position;

			}
			
		}

		Position = Position + Velocity * speed;
		// TBD: 
		//	scene->BlockHeight;
		//scene->MotherPosition;
		
			
	}
}

void Ball::Draw() const {
	al_draw_filled_circle(Position.x, Position.y, CollisionRadius, al_map_rgb(255, 255, 255));
}