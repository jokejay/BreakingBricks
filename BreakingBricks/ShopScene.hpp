#ifndef SHOPSCENE_HPP
#define SHOPSCENE_HPP

#include "IScene.hpp"
#include "Point.hpp"
#include "Label.hpp"


class ShopScene final : public Engine::IScene {
private:
	int money;
	int top_wave;
public:
	Group* UIGroup;
	Engine::Label* UIMoney;
	explicit ShopScene() = default;
	void Initialize() override;
	void Update(float deltaTime) override;
	void Draw() const override;
	void ConstructUI();
	void ReadDataHelper();
	void BackOnClick(int stage);
	void SaveDataHelper();
	void Purchase(int cost);
};
#endif // PLAYSCENE_HPP