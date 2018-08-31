
#include "MenuState.h"
#include "../DefaultGame.h"

MenuState::MenuState():
	GameState() {

}

unsigned int MenuState::id() {
	return StatesList::STATE_MENU;
}

void MenuState::enter(GameContainer* container, StateBasedGame* game, GameState* from) {

}

void MenuState::leave(GameContainer* container, StateBasedGame* game, GameState* to) {

}

void MenuState::init(GameContainer* container, StateBasedGame* game) {
	DefaultGame* dg = DefaultGame::getInstance();
	m_selectedIndex = 0;

	logo = dg->spritesheet->getSubImage(dg->desc->getItemByName("sprites/logo.png"));
}

void MenuState::update(GameContainer* container, StateBasedGame* game, GameTimer* timer) {
	DefaultGame* dg = DefaultGame::getInstance();
	Input* i = ARK2D::getInput();

	if (i->isKeyPressed(Input::KEY_W) ||
		i->isKeyPressed(Input::KEY_UP) ||
		i->isGamepadButtonPressed(Gamepad::DPAD_UP)) {
		m_selectedIndex--;
		if (m_selectedIndex < MENU_BEGIN) {
			m_selectedIndex = MENU_END;
		}
	}
	if (i->isKeyPressed(Input::KEY_S) ||
		i->isKeyPressed(Input::KEY_DOWN) ||
		i->isGamepadButtonPressed(Gamepad::DPAD_DOWN)) {
		m_selectedIndex++;
		if (m_selectedIndex > MENU_END) {
			m_selectedIndex = MENU_BEGIN;
		}
	}


	if (i->isKeyPressed(Input::KEY_ENTER) ||
		i->isGamepadButtonPressed(Gamepad::BUTTON_START) ||
		i->isGamepadButtonPressed(Gamepad::BUTTON_A)) {
		if (m_selectedIndex == MENU_PLAY) {
			dg->stateInGame->reset();
			dg->stateInGame->start();
			dg->m_music->setVolume(1.0f);
			game->enterState(dg->stateInGame);

		}
		else if (m_selectedIndex == MENU_INSTRUCTIONS) {
			game->enterState(dg->stateInstructions);
		}
		else if (m_selectedIndex == MENU_EXIT) {
			container->close();
		}
	}
}

void MenuState::render(GameContainer* container, StateBasedGame* game, Renderer* r) {
	DefaultGame* dg = DefaultGame::getInstance();
	InGameState* igs = dg->stateInGame;
	//r->drawString("SWAT ANALYSIS", 20, 20, Renderer::ALIGN_LEFT, Renderer::ALIGN_TOP);

	r->setDrawColor(Color::white);

	logo->drawCentered(container->getWidth()*0.5f, 40.0f);

	int rooty = 320;
	int wid = (signed int) container->getWidth();

	// if (m_selectedIndex == MENU_PLAY) {
	 	r->getFont()->drawStringCenteredAt("< Play >", wid/2, 70);
	 	r->getFont()->drawStringCenteredAt("twitter.com/forcehabit", wid/2, container->getHeight() - 10);
	// }
	// else {
	// 	r->getFont()->drawStringCenteredAt("Play", wid/2, rooty);
	// }

	// if (m_selectedIndex == MENU_INSTRUCTIONS) {
	// 	r->getFont()->drawStringCenteredAt("< Instructions >", wid/2, rooty+40);
	// }
	// else {
	// 	r->getFont()->drawStringCenteredAt("Instructions", wid/2, rooty+40);
	// }

	// if (m_selectedIndex == MENU_EXIT) {
	// 	r->getFont()->drawStringCenteredAt("< Exit >", wid/2, rooty+80);
	// }
	// else {
	// 	r->getFont()->drawStringCenteredAt("Exit", wid/2, rooty+80);
	// }
}

MenuState::~MenuState() {

}
