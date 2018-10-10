/**
* @file TitleState.cpp
*/
#include "GameState.h"
#include "GameEngine.h"
#include "../Res/Audio/SampleCueSheet.h"
#include "LoadState.h"
//#include "Map.h"
namespace GameState {

	/// 背景の更新.
	void UpdateSpaceSphere(Entity::Entity& entity, double delta)
	{
		
	}

	/// タイトル画面の更新.
	void Title::operator()(double delta)
	{
		GameEngine& game = GameEngine::Instance();
		
		game.Camera({ glm::vec3(0, 20, -8), glm::vec3(0, 0, 12), glm::vec3(0, 0, 1) });
		

		const float offset =
			timer == 0 ? 0 : (2.0f - timer) * (2.0f - timer) * 2.0f * 400.0f;
		game.FontScale(glm::vec2(2.0f, 2.0f));
		game.FontColor(glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));
		game.AddString(glm::vec2(300.0f + offset, 260.0f), "RANDOM MAZE");
		game.FontScale(glm::vec2(0.5f, 0.5f));
		game.FontColor(glm::vec4(0.75f, 0.75f, 0.75f, 1.0f));
		game.AddString(glm::vec2(480.0f + offset, 328.0f), "prezented by adati kentarou");
		if (timer > 0) {
			timer -= static_cast<float>(delta);
			if (timer <= 0) {
				//game.UpdateFunc(MainGame());
				//LoadState::CreateMap();
				//printf("change load\n");
				MainGame::stageno = 1;
				game.UpdateFunc(LoadState::LoadGame());

			}
		}
		else if (game.GetGamePad().buttonDown & GamePad::START || game.GetGamePad().mouseClick) {
			game.PlayAudio(AudioPlayerId_UI, CRI_SAMPLECUESHEET_START);
			timer = 2;
		}
	}

}