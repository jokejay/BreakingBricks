#ifndef HARDBRICK_HPP
#define HARDBRICK_HPP
#include <list>
#include <vector>
#include <string>

#include "Point.hpp"
#include "IObject.hpp"
#include "Label.hpp"
#include "Brick.hpp"

class Ball;
class PlayScene;

class HardBrick : public Brick {
public:
	static const int boldness;
	HardBrick(float x, float y, float hp);
	void Update(float deltatime) override;
};
#endif // ENEMY_HPP
