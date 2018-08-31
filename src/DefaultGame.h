
#ifndef ARK2D_DEFAULTGAME_H_
#define ARK2D_DEFAULTGAME_H_

#include <ARK2D/Audio/Sound.h>
#include <ARK2D/Core/Graphics/Image.h>
#include <ARK2D/Core/Graphics/SpriteSheetDescription.h>
#include <ARK2D/Core/State/StateBasedGame.h>

#include "states/BlankState.h"
#include "states/InGameState.h"
#include "states/InstructionsState.h"
#include "states/MenuState.h"
#include "states/SplashState.h"
#include "states/SummaryState.h"

class StatesList {
	public:
		static const unsigned int STATE_BLANK = 0;
		static const unsigned int STATE_LOADING = 1;
		static const unsigned int STATE_SPLASH = 2;
		static const unsigned int STATE_MENU = 3;
		static const unsigned int STATE_INSTRUCTIONS = 4;
		static const unsigned int STATE_INGAME = 5;
		static const unsigned int STATE_SUMMARY = 6;
};

class DefaultGame : public StateBasedGame {
	public:

		BlankState* stateBlank = NULL;
		SplashState* stateSplash = NULL;
		MenuState* stateMenu = NULL;
		InstructionsState* stateInstructions = NULL;
		InGameState* stateInGame = NULL;
		SummaryState* stateSummary = NULL;

		Image* spritesheet = NULL;
		SpriteSheetDescription* desc = NULL;

		Sound* m_music = NULL;

		Sound* m_sfxSelect = NULL;
		Sound* m_sfxPlayerDie = NULL;
		Sound* m_sfxJump = NULL;
		Sound* m_sfxNextWave = NULL;
		Sound* m_sfxEnd = NULL;
		Sound* m_sfxMoney = NULL;

		bool m_debug = false;

		DefaultGame(string title);
		virtual void initStates(GameContainer* container);
		virtual void update(GameContainer* container, GameTimer* timer);
		virtual void render(GameContainer* container, Renderer* r);
		virtual void resize(GameContainer* container, int width, int height);
		virtual ~DefaultGame();

		float juiceRotation(float maxRotation, float timer, float duration);

		static DefaultGame* getInstance();
};

#endif /* ARK2D_DEFAULTGAME_H_ */
