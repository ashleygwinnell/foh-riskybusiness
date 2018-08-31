/*
 * Object.cpp
 *
 *  Created on: 18 April 2012
 *      Author: Ashley
 */

#include "Object.h"
#include "MyParticle.h"
#include "../DefaultGame.h"
#include <ARK2D/Core/+.h>

float Object::s_gravity = 425.0f;

Object::Object():
	GameObject(),
	m_bounds(NULL)
	{
	m_velocity = new Vector2<float>(0,0);
	m_velocityMax = new Vector2<float>(0,0);
	m_acceleration = new Vector2<float>(0,0);
}

void Object::update(GameContainer* container, GameTimer* timer) {
	GameObject::update();
}


void Object::reset() {
	components.clear();
}

void Object::render(GameContainer* container, Renderer* r) {
	//if (ARK2D::isDebug() && m_bounds != NULL) {
	//	m_bounds->render();
	//}
	GameObject::render();
	if (DefaultGame::getInstance()->m_debug && m_bounds != NULL) {
		r->setDrawColor(Color::white);
		m_bounds->render();
	}

}


void Object::fireParticles() {
    DefaultGame* dg = DefaultGame::getInstance();
    InGameState* igs = dg->stateInGame;

	MyParticle* part = NULL;
	
	part = igs->m_particles->get();
	part->reset();
	part->m_animation->addFrame(MyParticle::s_cloud55);
	part->m_duration = MathUtil::randBetweenf(1.0f, 2.0f);
	part->m_type = MyParticle::TYPE_SMOKE;
	part->m_bounds->setLocationByCenter(m_bounds->getCenterX(), m_bounds->getCenterY());
	part->m_velocity->set(0.0f, 0.0f);
	MathUtil::moveAngle<float>(part->m_velocity, MathUtil::randBetweenf(-180.0f, 0.0f), MathUtil::randBetweenf(5.0f, 20.0f));

	part = igs->m_particles->get();
	part->reset();
	part->m_animation->addFrame(MyParticle::s_cloud33);
	part->m_duration = MathUtil::randBetweenf(1.0f, 2.0f);
	part->m_type = MyParticle::TYPE_SMOKE;
	part->m_bounds->setLocationByCenter(m_bounds->getCenterX(), m_bounds->getCenterY());
	part->m_velocity->set(0.0f, 0.0f);
	MathUtil::moveAngle<float>(part->m_velocity, MathUtil::randBetweenf(-180.0f, 0.0f), MathUtil::randBetweenf(5.0f, 20.0f));

	part = igs->m_particles->get();
	part->reset();
	part->m_animation->addFrame(MyParticle::s_cloud22);
	part->m_duration = MathUtil::randBetweenf(1.0f, 2.0f);
	part->m_type = MyParticle::TYPE_SMOKE;
	part->m_bounds->setLocationByCenter(m_bounds->getCenterX(), m_bounds->getCenterY());
	part->m_velocity->set(0.0f, 0.0f);
	MathUtil::moveAngle<float>(part->m_velocity, MathUtil::randBetweenf(-180.0f, 0.0f), MathUtil::randBetweenf(5.0f, 20.0f));
	
}

void Object::gravity() {
	gravity(1.0f);
}
void Object::gravity(float multiplier) {
	// Add Gravity
	GameTimer* timer = ARK2D::getContainer()->getTimer();
	//if (!DefaultGame::getInstance()->stateInGame->isCoffeeMode()) {
		m_velocity->add(0, s_gravity * timer->getDelta());
	//}

	m_velocity->restrictMax(m_velocityMax);
}

void Object::move(float multiplier = 1) {
	GameTimer* timer = ARK2D::getContainer()->getTimer();
    ARK::Core::Geometry::Rectangle* bounds = m_bounds->asRectangle();
	bounds->adjustX(m_velocity->getX() * multiplier * timer->getDelta());
	bounds->adjustY(m_velocity->getY() * multiplier * timer->getDelta());
}

void Object::collision(Object* other) {

}

Object::~Object() {
	delete m_velocity;
	delete m_velocityMax;
}
