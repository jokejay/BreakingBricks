#ifndef BRICK_HPP
#define BRICK_HPP
#include <list>
#include <vector>
#include <string>

#include "Point.hpp"
#include "IObject.hpp"

class Ball;
class PlayScene;

class Brick : public Engine::IObject {
protected:
	float speed;
	float hp;
	int money;
	PlayScene* getPlayScene();
public:
	Brick(float x, float y, float hp);
	void Hit(float damage);
	void Update(float deltaTime) override;
	void Draw() const override;
};
#endif // ENEMY_HPP
