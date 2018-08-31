/*
 * Trap.cpp
 *
 *  Created on: 29 Jan 2011
 *      Author: Ashley
 */

#include "Trap.h"

#include "MyParticle.h"
#include "Enemy.h"
#include "Player.h"
#include "../DefaultGame.h"

#include "EightWayMovementComponent.h"
#include <ARK2D/Core/+.h>
#include <ARK2D/Audio/Sound.h>
#include <ARK2D/Util/CameraShake.h>

Image* Trap::s_spike = NULL;
Image* Trap::s_blade = NULL;

Trap::Trap():
	Object()
	{

	DefaultGame* dg = DefaultGame::getInstance();

	if (s_spike == NULL) {
		s_spike = dg->spritesheet->getSubImage(dg->desc->getItemByName("sprites/spike-up-1.png"));
		s_spike->setCenterOfRotation(s_spike->getWidth()/2, s_spike->getHeight());

		s_blade = dg->spritesheet->getSubImage(dg->desc->getItemByName("sprites/fan-blade.png"));
		s_blade->setCenterOfRotation(1, 1);
	}

    m_bounds = new ARK::Core::Geometry::Rectangle(0, 0, 3, 4);
	m_bounds->setLocationByCenter(64, 32);

	m_animation = new Animation();
	m_animation->setFrameTime(0.05f);
	m_animation->addFrame(s_spike);

	m_velocity->set(0, 0);
	m_velocityMax->set(0, 0);
	m_acceleration->set(0, 0);

	reset();
}

void Trap::reset() {
	m_bladeRotation = 0;
	m_bladeRotationSpeed = 180;
	m_animation->reset();
	m_animation->clear();
}

void Trap::update(GameContainer* container, GameTimer* timer) {
	Object::update(container, timer);

	DefaultGame* dg = DefaultGame::getInstance();
	InGameState* igs = dg->stateInGame;

	m_animation->update(timer);

	PoolIterator<Player*> it = igs->m_players->newiteratorref();
	while(it.hasNext()) {
		Player* p = it.next();
		if (p->m_bounds->collides(m_bounds)) {
			p->die();

			if (p->m_dieReason == Player::DIE_NULL) { 
				igs->spawnBlood(m_bounds->getCenterX(), m_bounds->getCenterY(), 3.0f, 5);
				p->m_dieReason = Player::DIE_SPIKE;
			}
		}
	}

	m_bounds->asRectangle()->setLocationByCenter(
		m_bounds->getCenterX() + (m_velocity->getX() * timer->getDelta()),
		m_bounds->getCenterY() + (m_velocity->getY() * timer->getDelta())
	);


	//if (igs->m_player->m_moving){//} || !igs->m_player->m_grounded ){ 
	if (true) {
		m_bladeRotation += m_bladeRotationSpeed * timer->getDelta();
	}
}

void Trap::collision(Object* other) {

}

void Trap::render(GameContainer* container, Renderer* r)
{
	InGameState* igs = DefaultGame::getInstance()->stateInGame;
	r->setDrawColor(Color::white);

	Image* img = m_animation->getCurrentFrame();

	if (m_type == TYPE_BLADE) { 
		img->setRotation(m_bladeRotation);
		img->draw( m_bounds->getCenterX(), m_bounds->getCenterY() );
	}
	else {
		//img->setRotation(rotation);
		//img->setFlipped(!m_facingLeft, false);
    	float scale = 1.0f;
		img->drawCenteredScaled(m_bounds->getCenterX(), m_bounds->getCenterY(), scale, scale);
	}


	
	
	//r->drawRect(m_bounds->getMinX(), m_bounds->getMinY(), m_bounds->getWidth(), m_bounds->getHeight());
}

Trap::~Trap() {

}
