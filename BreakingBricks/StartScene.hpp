#ifndef STARTSCENE_HPP
#define STARTSCENE_HPP
#include "IScene.hpp"
#include "Point.hpp"

class StartScene final : public Engine::IScene {
private:
	Engine::Point ballPos;
	Engine::Point ballDir;
public:
	explicit StartScene() = default;
	void Initialize() override;
	void Update(float deltatime) override;
	void Draw() const override;
	void BackOnClick(int stage);
};

#endif // LOSESCENE_HPP
#pragma once
