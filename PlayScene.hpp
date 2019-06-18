#ifndef PLAYSCENE_HPP
#define PLAYSCENE_HPP
#include <list>
#include <utility>
#include <vector>

#include "IScene.hpp"
#include "Point.hpp"

class Turret;
namespace Engine {
	class Group;
	class Image;
	class Label;
}  // namespace Engine

class PlayScene final : public Engine::IScene {
protected:
	int balls;
	int money;
	int SpeedMult;
	int wave;
	int State;
public:
	static bool DebugMode;
	static const std::vector<Engine::Point> directions;
	static const int MapWidth, MapHeight;
	static const int BlockSize;
	static const std::vector<Engine::Point> SpawnPoint;
	static const int EndY;
	static const std::vector<int> code;
	Engine::Point pos_ball;
	enum State {
		SET_ANGLE,
		BALL_RUNNING,
		MOVING_BRICK,
	};
	float ticks;
	// Map tiles.
	Group* DebugIndicatorGroup;
	Group* BallGroup;
	Group* BrickGroup;
	Group* EffectGroup;
	Group* UIGroup;
	Engine::Label* UIMoney;
	Engine::Label* UIBalls;
	explicit PlayScene() = default;
	void Initialize() override;
	void Update(float deltaTime) override;
	void Draw() const override;
	void OnMouseDown(int button, int mx, int my) override;
	void OnMouseMove(int mx, int my) override;
	void OnMouseUp(int button, int mx, int my) override;
	void OnKeyDown(int keyCode) override;
	void Hit();
	void EarnMoney(int money);
	void ConstructUI();
	void UIBtnClicked(int id);
	enum State GetState();
	// void ModifyReadMapTiles();
};
#endif // PLAYSCENE_HPP
