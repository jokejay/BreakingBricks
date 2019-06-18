#ifndef LIGHTGUNTURRET_HPP
#define LIGHTGUNTURRET_HPP
#include "Turret.hpp"

class LightGunTurret : public Turret {
public:
	static const int Price;
	LightGunTurret(float x, float y);
	void CreateBullet() override;
};
#endif // LIGHTGUNTURRET_HPP
