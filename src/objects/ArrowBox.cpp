/*
 * ArrowBox.cpp
 *
 *  Created on: 29 Jan 2011
 *      Author: Ashley
 */

#include "ArrowBox.h"
#include "Arrow.h"
#include "MyParticle.h"
#include "Enemy.h"
#include "Player.h"
#include "../DefaultGame.h"

#include "EightWayMovementComponent.h"
#include <ARK2D/Core/+.h>
#include <ARK2D/Audio/Sound.h>
#include <ARK2D/Util/CameraShake.h>

Image* ArrowBox::s_boxFull = NULL;
Image* ArrowBox::s_boxEmpty = NULL;

ArrowBox::ArrowBox():
	Object()
	{

	DefaultGame* dg = DefaultGame::getInstance();

	if (s_boxFull == NULL) {
		s_boxFull = dg->spritesheet->getSubImage(dg->desc->getItemByName("sprites/arrow-block-full.png"));
		s_boxFull->setCenterOfRotation(s_boxFull->getWidth()/2, s_boxFull->getHeight());

		s_boxEmpty = dg->spritesheet->getSubImage(dg->desc->getItemByName("sprites/arrow-block-empty.png"));
		s_boxEmpty->setCenterOfRotation(s_boxEmpty->getWidth()/2, s_boxEmpty->getHeight());
	}

    m_bounds = new ARK::Core::Geometry::Rectangle(0, 0, 7, 7);
	m_bounds->setLocationByCenter(64, 32);

	m_velocity->set(0, 0);
	m_velocityMax->set(0, 0);
	m_acceleration->set(0, 0);

	reset();
}

void ArrowBox::reset() {

	m_shootTimer = 0.0f;
	m_shootDuration = 2.0f;
	m_reloadTimer = 0.0f;
	m_reloadDuration = 1.0f;
	m_hasArrow = true;

	m_facingLeft = true;
}

void ArrowBox::shoot() {
	DefaultGame* dg = DefaultGame::getInstance();
	InGameState* igs = dg->stateInGame;

	ARK2D::getLog()->e("ArrowBox shoot");

	//igs->m_flashTimer = 0.01f;
	igs->m_cameraShake->setSpeedMultiplier(3, 1.0f);
	igs->m_cameraShake->start(3, 0.5f);

	
}

void ArrowBox::update(GameContainer* container, GameTimer* timer) {
	Object::update(container, timer);

	DefaultGame* dg = DefaultGame::getInstance();
	InGameState* igs = dg->stateInGame;

	/*if (!m_hasArrow) {
		m_reloadTimer += timer->getDelta();
		if (m_reloadTimer >= m_reloadDuration) {
			m_reloadTimer = 0.0f;
			m_hasArrow = true;
		}	
	}
	else if (m_hasArrow) {
		m_shootTimer += timer->getDelta();
		if (m_shootTimer >= m_shootDuration) {
			m_hasArrow = false;
			m_shootTimer = 0.0f;

			Arrow* arrow = igs->m_arrows->get();
			arrow->reset();
			arrow->m_bounds->setLocationByCenter(m_bounds->getCenterX(), m_bounds->getCenterY() + 0.5f );
			if (!m_facingLeft) { 
				arrow->m_velocity->set(60,0);
			} else {
				arrow->m_velocity->set(-60,0);
			}

			dg->m_sfxArrowFire->play();
		}	
	}*/

	m_bounds->asRectangle()->setLocationByCenter(
		m_bounds->getCenterX() + (m_velocity->getX() * timer->getDelta()),
		m_bounds->getCenterY() + (m_velocity->getY() * timer->getDelta())
	);

}

void ArrowBox::collision(Object* other) {

}

void ArrowBox::render(GameContainer* container, Renderer* r)
{
	InGameState* igs = DefaultGame::getInstance()->stateInGame;
	r->setDrawColor(Color::white);

	Image* img = NULL;
	if (m_hasArrow) {
		img = s_boxFull;
	} else if (!m_hasArrow) {
		img = s_boxEmpty;
	}

	float scale = 1.0f; 
	float rotation = 0.0f;

	if (m_hasArrow && m_shootTimer < 0.25f) {
		scale = Easing::easebetween(Easing::QUADRATIC_OUT, m_shootTimer, 0.9f, 1.0f, 0.25f);
	} else if (!m_hasArrow && m_reloadTimer < 0.25f) {
		scale = Easing::easebetween(Easing::QUADRATIC_OUT, m_reloadTimer, 1.25f, 1.0f, 0.25f);
	}

	img->setRotation(rotation);
	img->setFlipped(!m_facingLeft, false);
	img->drawCenteredScaled(m_bounds->getCenterX(), m_bounds->getCenterY(), scale, scale);
	
	//r->drawRect(m_bounds->getMinX(), m_bounds->getMinY(), m_bounds->getWidth(), m_bounds->getHeight());
}

ArrowBox::~ArrowBox() {

}
