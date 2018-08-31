/*
 * Collectable.cpp
 *
 *  Created on: 29 Jan 2011
 *      Author: Ashley
 */

#include "Collectable.h"

#include "MyParticle.h"
#include "Enemy.h"
#include "Player.h"
#include "../DefaultGame.h"

#include "EightWayMovementComponent.h"
#include <ARK2D/Core/+.h>
#include <ARK2D/Audio/Sound.h>
#include <ARK2D/Util/CameraShake.h>

Image* Collectable::s_dollar = NULL;
Image* Collectable::s_dollarGib1 = NULL;
Image* Collectable::s_dollarGib2 = NULL;
Image* Collectable::s_briefcase = NULL;

Collectable::Collectable():
	Object()
	{

	DefaultGame* dg = DefaultGame::getInstance();

	if (s_dollar == NULL) {
		s_dollar = dg->spritesheet->getSubImage(dg->desc->getItemByName("sprites/collectable-dollar.png"));
		s_dollar->setCenterOfRotation(s_dollar->getWidth()/2, s_dollar->getHeight());

		s_dollarGib1 = dg->spritesheet->getSubImage(dg->desc->getItemByName("sprites/collectable-dollar-gib-1.png"));
		s_dollarGib1->setCenterOfRotation(s_dollarGib1->getWidth()/2, s_dollarGib1->getHeight());

		s_dollarGib2 = dg->spritesheet->getSubImage(dg->desc->getItemByName("sprites/collectable-dollar-gib-2.png"));
		s_dollarGib2->setCenterOfRotation(s_dollarGib2->getWidth()/2, s_dollarGib2->getHeight());

		s_briefcase = dg->spritesheet->getSubImage(dg->desc->getItemByName("sprites/briefcase.png"));
		s_briefcase->setCenterOfRotation(s_briefcase->getWidth()/2, s_briefcase->getHeight());
	}

    m_bounds = new ARK::Core::Geometry::Rectangle(0, 0, 4, 2);
	m_bounds->setLocationByCenter(64, 32);

	m_animation = new Animation();
	m_animation->setFrameTime(0.05f);
	m_animation->addFrame(s_dollar);

	m_velocity->set(0, 0);
	m_velocityMax->set(0, 0);
	m_acceleration->set(0, 0);

	reset();
}

void Collectable::reset() {
	m_animation->reset();
	m_animation->clear();

	m_particleTimer = 0.0f;
	m_particleDuration = 0.1f;

	m_visible = true;
}

void Collectable::collected() {
	

	DefaultGame* dg = DefaultGame::getInstance();
    InGameState* igs = dg->stateInGame;

    dg->m_sfxMoney->play();

	MyParticle* part = NULL;
	
	part = igs->m_particles->get();
	part->reset();
	part->m_animation->addFrame(Collectable::s_dollarGib1);
	part->m_duration = MathUtil::randBetweenf(1.0f, 2.0f);
	part->m_type = MyParticle::TYPE_PLAYER_GIB;
	part->m_bounds->setLocationByCenter(m_bounds->getCenterX(), m_bounds->getCenterY());
	part->m_velocity->set(0.0f, 0.0f);
	MathUtil::moveAngle<float>(part->m_velocity, MathUtil::randBetweenf(0.0f, 360.0f), MathUtil::randBetweenf(10.0f, 20.0f));

	part = igs->m_particles->get();
	part->reset();
	part->m_animation->addFrame(Collectable::s_dollarGib2);
	part->m_duration = MathUtil::randBetweenf(1.0f, 2.0f);
	part->m_type = MyParticle::TYPE_PLAYER_GIB;
	part->m_bounds->setLocationByCenter(m_bounds->getCenterX(), m_bounds->getCenterY());
	part->m_velocity->set(0.0f, 0.0f);
	MathUtil::moveAngle<float>(part->m_velocity, MathUtil::randBetweenf(0.0f, 360.0f), MathUtil::randBetweenf(10.0f, 20.0f));

	setPendingRemoval(true);
}

void Collectable::update(GameContainer* container, GameTimer* timer) {
	Object::update(container, timer);

	DefaultGame* dg = DefaultGame::getInstance();
	InGameState* igs = dg->stateInGame;

	m_animation->update(timer);

	// PoolIterator<Player*> it = igs->m_players->newiteratorref();
	// while(it.hasNext()) {
	// 	Player* p = it.next();
	// 	if (p->m_bounds->collides(m_bounds)) {
	// 		setPendingRemoval(true);
	// 	}
	// }

	m_bounds->asRectangle()->setLocationByCenter(
		m_bounds->getCenterX() + (m_velocity->getX() * timer->getDelta()),
		m_bounds->getCenterY() + (m_velocity->getY() * timer->getDelta())
	);

	if (m_type == TYPE_GOAL) {
		if (m_visible) {
			m_particleTimer += timer->getDelta();
			if (m_particleTimer >= m_particleDuration) {
				m_particleTimer -= m_particleDuration;

				MyParticle* p = igs->m_particles->get();
				p->reset();
				p->m_animation->addFrame(Collectable::s_dollar);
				p->m_bounds->setLocationByCenter(m_bounds->getCenterX(), m_bounds->getCenterY());
				p->m_type = MyParticle::TYPE_PLAYER_GIB;
				MathUtil::moveAngle<float>(p->m_velocity, MathUtil::randBetweenf(0,360), MathUtil::randBetweenf(20.0f, 30.0f));

			}
		}
	}
}

void Collectable::collision(Object* other) {

}

void Collectable::render(GameContainer* container, Renderer* r)
{
	InGameState* igs = DefaultGame::getInstance()->stateInGame;
	r->setDrawColor(Color::white);

	if (m_visible) { 

		Image* img = m_animation->getCurrentFrame();

	//if (m_type == TYPE_BLADE) { 
	//	img->setRotation(m_bladeRotation);
		img->drawCentered( m_bounds->getCenterX(), m_bounds->getCenterY() );
	//}
	}
//	else {
		//img->setRotation(rotation);
		//img->setFlipped(!m_facingLeft, false);
    	//float scale = 1.0f;
		//img->drawCenteredScaled(m_bounds->getCenterX(), m_bounds->getCenterY(), scale, scale);
	//}

	//r->drawRect(m_bounds->getMinX(), m_bounds->getMinY(), m_bounds->getWidth(), m_bounds->getHeight());
}

Collectable::~Collectable() {

}
