
#include "SummaryState.h"
#include "../DefaultGame.h"

SummaryState::SummaryState():
    GameState() {

}

unsigned int SummaryState::id() {
	return StatesList::STATE_SUMMARY;
}

void SummaryState::enter(GameContainer* container, StateBasedGame* game, GameState* from) { }
void SummaryState::leave(GameContainer* container, StateBasedGame* game, GameState* to) { }

void SummaryState::init(GameContainer* container, StateBasedGame* game) {
	DefaultGame* dg = DefaultGame::getInstance();

}

void SummaryState::update(GameContainer* container, StateBasedGame* game, GameTimer* timer) {
	
	DefaultGame* dg = DefaultGame::getInstance();
	Input* in = ARK2D::getInput();
	if (in->isKeyPressed(Input::KEY_ENTER) ||
        in->isGamepadButtonPressed(Gamepad::BUTTON_START) ||
        in->isGamepadButtonPressed(Gamepad::BUTTON_A)) {
		dg->enterState(dg->stateMenu);
	}
}  

void SummaryState::render(GameContainer* container, StateBasedGame* game, Renderer* r) {
	//r->drawString("Summary State", 20, 20, Renderer::ALIGN_LEFT, Renderer::ALIGN_TOP);

	DefaultGame* dg = DefaultGame::getInstance();
	r->setDrawColor(Color::white);

	dg->stateMenu->logo->drawCentered(container->getWidth()*0.5f, 40.0f);

	int rooty = 320;
	int wid = (signed int) container->getWidth();

	// if (m_selectedIndex == MENU_PLAY) {
	 	r->getFont()->drawStringCenteredAt("What were you thinking?", wid/2, 65);
	 	r->getFont()->drawStringCenteredAt("Money won't bring you happiness...", wid/2, 85);
	 	r->getFont()->drawStringCenteredAt("THE END", wid/2, 100);
	 	//r->getFont()->drawStringCenteredAt("Money will make things worse.", wid/2, 120);
	// }

}

SummaryState::~SummaryState() {

}

