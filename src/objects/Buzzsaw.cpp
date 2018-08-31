/*
 * Buzzsaw.cpp
 *
 *  Created on: 29 Jan 2011
 *      Author: Ashley
 */

#include "Buzzsaw.h"

#include "MyParticle.h"
#include "Enemy.h"
#include "Player.h"
#include "../DefaultGame.h"

#include "EightWayMovementComponent.h"
#include <ARK2D/Core/+.h>
#include <ARK2D/Audio/Sound.h>
#include <ARK2D/Util/CameraShake.h>

Image* Buzzsaw::s_frame1 = NULL;
Image* Buzzsaw::s_frame2 = NULL;

Buzzsaw::Buzzsaw():
	Object()
	{

	DefaultGame* dg = DefaultGame::getInstance();

	if (s_frame1 == NULL) {
		s_frame1 = dg->spritesheet->getSubImage(dg->desc->getItemByName("sprites/buzzsaw-1.png"));
		s_frame1->setCenterOfRotation(s_frame1->getWidth()/2, s_frame1->getHeight());

		s_frame2 = dg->spritesheet->getSubImage(dg->desc->getItemByName("sprites/buzzsaw-2.png"));
		s_frame2->setCenterOfRotation(s_frame2->getWidth()/2, s_frame2->getHeight());
	}

    m_bounds = new ARK::Core::Geometry::Rectangle(0, 0, 7, 5);
	m_bounds->setLocationByCenter(64, 32);

	m_animation = new Animation();
	m_animation->setFrameTime(0.05f);
	m_animation->addFrame(s_frame1);
	m_animation->addFrame(s_frame2);

	m_velocity->set(0, 0);
	m_velocityMax->set(0, 0);
	m_acceleration->set(0, 0);

	reset();
}

void Buzzsaw::reset() {

}

void Buzzsaw::update(GameContainer* container, GameTimer* timer) {
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
				p->m_dieReason = Player::DIE_BUZZSAW;
			}
		}
	}

	m_bounds->asRectangle()->setLocationByCenter(
		m_bounds->getCenterX() + (m_velocity->getX() * timer->getDelta()),
		m_bounds->getCenterY() + (m_velocity->getY() * timer->getDelta())
	);

}

void Buzzsaw::collision(Object* other) {

}

void Buzzsaw::render(GameContainer* container, Renderer* r)
{
	InGameState* igs = DefaultGame::getInstance()->stateInGame;
	r->setDrawColor(Color::white);

	Image* img = m_animation->getCurrentFrame();

	//img->setRotation(rotation);
	//img->setFlipped(!m_facingLeft, false);
    float scale = 1.0f;
	img->drawCenteredScaled(m_bounds->getCenterX(), m_bounds->getCenterY(), scale, scale);
	
	//r->drawRect(m_bounds->getMinX(), m_bounds->getMinY(), m_bounds->getWidth(), m_bounds->getHeight());
}

Buzzsaw::~Buzzsaw() {

}
