#ifndef BALL_HPP
#define BALL_HPP
#include <string>

#include "Sprite.hpp"
#include "Point.hpp"

class Brick;
class PlayScene;
namespace Engine {
	struct Point;
}  // namespace Engine

class Ball : public Engine::IObject {
protected:
	float speed;
	float damage;
	PlayScene* getPlayScene();
public:
	bool active;
	int shock;
	float CollisionRadius;
	Engine::Point Velocity;
	explicit Ball(float speed, float damage, Engine::Point position, Engine::Point velocity, float radius);
	void Update(float deltaTime) override;
	void Draw() const override;
};
#endif // BALL_HPP
