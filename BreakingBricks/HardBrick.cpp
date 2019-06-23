#include <allegro5/allegro_primitives.h>
#include <allegro5/color.h>
#include <cmath>
#include <random>
#include <string>

#include "AudioHelper.hpp"
#include "HardBrick.hpp"
#include "IObject.hpp"
#include "GameEngine.hpp"
#include "Group.hpp"
#include "IScene.hpp"
#include "PlayScene.hpp"

const int HardBrick::boldness = 5;

HardBrick::HardBrick(float x, float y, float hp) :
	Brick(x, y, hp * 1.5) {
	StartR = 0; StartG = 52; StartB = 250; EndR = 0; EndG = 160; EndB = 233;
}

void HardBrick::Update(float deltaTime) {
	float TargetY = PlayScene::StartY + (getPlayScene()->wave - money/1.5) * (Size.y + boldness + 6);
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
	diff = 1 - hp/1.5 / getPlayScene()->wave;
}