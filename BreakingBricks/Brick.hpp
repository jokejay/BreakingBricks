#ifndef BRICK_HPP
#define BRICK_HPP
#include <list>
#include <vector>
#include <string>

#include "Point.hpp"
#include "IObject.hpp"
#include "Label.hpp"

class Ball;
class PlayScene;

class Brick : public Engine::IObject {
protected:
	float speed;
	float hp;
	int money;
	PlayScene* getPlayScene();
	Engine::Label* LifeUI;
	
public:
	const static int StartR, StartG, StartB , EndR, EndG, EndB;
	static const int boldness;
	bool Moving;
	Brick(float x, float y, float hp);
	void Hit(float damage);
	void Update(float deltaTime) override;
	void Draw() const override;
};
#endif // ENEMY_HPP
