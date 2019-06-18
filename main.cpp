// [main.cpp]
// this template is provided for the 2D tower defense game.
// Program entry point.
// Returns program exit code.
#include "GameEngine.hpp"
#include "LOG.hpp"
#include "LoseScene.hpp"
#include "PlayScene.hpp"
#include "ShopScene.hpp"
#include "StageSelectScene.hpp"
#include "WinScene.hpp"
#include "StartScene.hpp"

int main(int argc, char **argv) {
	Engine::LOG::SetConfig(true);
	Engine::GameEngine& game = Engine::GameEngine::GetInstance();
	game.AddNewScene("play", new PlayScene());
	game.AddNewScene("shop", new ShopScene());
	// TODO 1 (1/2): Add a New Scene here and change the start scene below.
	game.AddNewScene("start", new StartScene());
	game.Start("start", 60, 480, 800);
	return 0;
}
