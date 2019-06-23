#ifndef PLAYSCENE_HPP
#define PLAYSCENE_HPP
#include <list>
#include <utility>
#include <vector>

#include "IScene.hpp"
#include "Point.hpp"
#include "Brick.hpp"

class Turret;
namespace Engine {
	class Group;
	class Image;
	class Label;
}  // namespace Engine

class PlayScene final : public Engine::IScene {
protected:
	int balls;
	int added_balls;
	int return_balls;
	int money;
	int SpeedMult;
	float down;
	Engine::Point InitPt, FinPt;
public:
	Engine::Point MotherDirection, MotherPosition;
	static bool DebugMode;
	static const std::vector<Engine::Point> directions;
	static const int MapWidth, MapHeight;
	static const int BlockWidth, BlockHeight;
	static const int StartY, StartX;
	static const int EndY;
	static const int BlockTotalW;
	int wave;
	int top_wave;
	static const std::vector<int> code;
	Engine::Point pos_ball;
	enum State {
		SET_ANGLE,
		BALL_RUNNING,
		MOVING_BRICK,
		GENERATING_BRICK,
		FINISH,
	};
	State cur_State;
	float ticks;
	// Map tiles.
	Group* DebugIndicatorGroup;
	Group* BallGroup;
	Group* BrickGroup;
	Group* EffectGroup;
	Group* UIGroup;
	Engine::Label* UIMoney;
	Engine::Label* UIWave;
	Engine::Label* UITop;
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
	void ReadDataHelper();
	void UIBtnClicked(int id);
	void BackOnClick(int stage);
	void SaveDataHelper();
	void ReturnBall();
	// void ModifyReadMapTiles();
};
#endif // PLAYSCENE_HPP
