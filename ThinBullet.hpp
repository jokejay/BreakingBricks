#ifndef THINBULLET_HPP
#define THINBULLET_HPP
#include "Bullet.hpp"

class Enemy;
class Turret;
namespace Engine {
	struct Point;
}  // namespace Engine

class ThinBullet : public Bullet {
public:
	explicit ThinBullet(Engine::Point position, Engine::Point forwardDirection, float rotation, Turret* parent);
	void OnExplode(Enemy* enemy) override;
};
#endif // THINBULLET_HPP
