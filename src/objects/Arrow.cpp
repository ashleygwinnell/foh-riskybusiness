/*
 * Arrow.cpp
 *
 *  Created on: 29 Jan 2011
 *      Author: Ashley
 */

#include "Arrow.h"
#include "MyParticle.h"
#include "Enemy.h"
#include "Player.h"
#include "../DefaultGame.h"

#include "EightWayMovementComponent.h"
#include <ARK2D/Core/+.h>
#include <ARK2D/Audio/Sound.h>
#include <ARK2D/Util/CameraShake.h>

Image* Arrow::s_arrow = NULL;

Arrow::Arrow():
	Object()
	{

	DefaultGame* dg = DefaultGame::getInstance();

	if (s_arrow == NULL) {
		s_arrow = dg->spritesheet->getSubImage(dg->desc->getItemByName("sprites/arrow.png"));
		s_arrow->setCenterOfRotation(s_arrow->getWidth()*0.5f, s_arrow->getHeight()*0.5f);
	}

    m_bounds = new ARK::Core::Geometry::Rectangle(0, 0, 7, 2);
	m_bounds->setLocationByCenter(64, 32);

	m_velocity->set(0, 0);
	m_velocityMax->set(100, 100);
	m_acceleration->set(0, 0);

	reset();
}

void Arrow::reset() {

}

void Arrow::update(GameContainer* container, GameTimer* timer) {
	Object::update(container, timer);

	DefaultGame* dg = DefaultGame::getInstance();
	InGameState* igs = dg->stateInGame;

	m_bounds->asRectangle()->setLocationByCenter(
		m_bounds->getCenterX() + (m_velocity->getX() * timer->getDelta()),
		m_bounds->getCenterY() + (m_velocity->getY() * timer->getDelta())
	);

}

void Arrow::collision(Object* other) {

}

void Arrow::render(GameContainer* container, Renderer* r)
{
	InGameState* igs = DefaultGame::getInstance()->stateInGame;
	r->setDrawColor(Color::white);

	float scale = 1.0f; 
	float rotation = 0.0f;
	if (m_velocity->getX() > 0.0f) {
		rotation = 180;
	}

	s_arrow->setRotation(rotation);
	s_arrow->drawCentered(m_bounds->getCenterX(), m_bounds->getCenterY());
	
	//r->drawRect(m_bounds->getMinX(), m_bounds->getMinY(), m_bounds->getWidth(), m_bounds->getHeight());
}

Arrow::~Arrow() {

}
