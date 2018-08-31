
#include "DefaultGame.h"
#include <ARK2D/Core/Graphics/SpriteSheetStore.h>
#include <ARK2D/Core/Tween/Easing.h>

DefaultGame::DefaultGame(string title):
	StateBasedGame(title) {

}

void DefaultGame::initStates(GameContainer* container) {

	stateBlank = new BlankState();
	stateSplash = new SplashState();
	stateMenu = new MenuState();
	stateInstructions = new InstructionsState();
	stateInGame = new InGameState();
	stateSummary = new SummaryState();

	spritesheet = Resource::get("all.png")->asImage();
	desc = Resource::get("all.spritesheetdescription")->asSpriteSheetDescription();
	SpriteSheetStore::addSheet(desc, spritesheet);

	m_music = Resource::get("music.ogg")->asSound();

	m_sfxSelect = Resource::get("select.wav")->asSound();
	m_sfxPlayerDie = Resource::get("playerdie.wav")->asSound();
	m_sfxJump = Resource::get("playerjump.wav")->asSound();
	m_sfxNextWave = Resource::get("next_wave.wav")->asSound();
	m_sfxEnd = Resource::get("end.wav")->asSound();
	m_sfxMoney = Resource::get("money.wav")->asSound();

	addState(stateBlank);
	addState(stateSplash);
	addState(stateMenu);
	addState(stateInstructions);
	addState(stateInGame);
	addState(stateSummary);

	enterState(stateMenu);

	container->setSize(210*4, 120*4);
	//container->setFullscreen(true);
}

void DefaultGame::update(GameContainer* container, GameTimer* timer) {
	StateBasedGame::update(container, timer);

	Input* i = ARK2D::getInput();
	if (i->isGamepadButtonPressed(Gamepad::BUTTON_ACTIVATE) || i->isKeyPressed(Input::KEY_F)) {
		container->setFullscreen(!container->isFullscreen());
	}

	if (i->isKeyDown(Input::KEY_LSHIFT) && i->isKeyPressed(Input::KEY_O)) {
		m_debug = !m_debug;
	}

	if (m_music != NULL) {
		if (!m_music->isPlaying()) {
			m_music->play();
		}
	}
}

void DefaultGame::render(GameContainer* container, Renderer* r) {
	StateBasedGame::render(container, r);
}

void DefaultGame::resize(GameContainer* container, int width, int height) {
	StateBasedGame::resize(container, width, height);
}

DefaultGame* DefaultGame::getInstance() {
    return dynamic_cast<DefaultGame*>(ARK2D::getGame());
}

float DefaultGame::juiceRotation(float maxRotation, float timer, float duration) {
	float rotation = 0.0f;
	float segment = duration / 4.0f;
	if (timer <= segment) {
		rotation = Easing::ease(Easing::QUADRATIC_OUT, timer, 0.0f, maxRotation, segment);
	} else if (timer <= 2*segment) {
		rotation = Easing::ease(Easing::QUADRATIC_IN, timer-segment, maxRotation, maxRotation*-1, segment);
	} else if (timer <= 3*segment) {
		rotation = Easing::ease(Easing::QUADRATIC_OUT, timer-(2*segment), 0.0f, maxRotation*-1, segment);
	} else {
		rotation = Easing::ease(Easing::QUADRATIC_IN, timer-(3*segment), maxRotation*-1, maxRotation, segment);
	}
	return rotation;
}

DefaultGame::~DefaultGame() {

}
