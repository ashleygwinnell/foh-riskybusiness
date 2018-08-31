/*
 * MyParticle.cpp
 *
 *  Created on: 29 Jan 2011
 *      Author: Ashley
 */

#include "MyParticle.h"
#include "Bomb.h"
#include "../DefaultGame.h"

#include "EightWayMovementComponent.h"
#include <ARK2D/Core/+.h>
#include <ARK2D/Audio/Sound.h>

Image* MyParticle::s_cloud11 = NULL;
Image* MyParticle::s_cloud22 = NULL;
Image* MyParticle::s_cloud33 = NULL;
Image* MyParticle::s_cloud44 = NULL;
Image* MyParticle::s_cloud55 = NULL;
Image* MyParticle::s_cloud77 = NULL;
vector<Image* >* MyParticle::s_bloodSelection = NULL;

float MyParticle::s_gravity = 150.0f;

MyParticle::MyParticle():
	Object()
	{

	DefaultGame* dg = DefaultGame::getInstance();

	if (s_cloud11 == NULL) {
		s_cloud11 = dg->spritesheet->getSubImage(dg->desc->getItemByName("sprites/cloud-1x1.png"));
		s_cloud11->setCenterOfRotation(s_cloud11->getWidth()/2, s_cloud11->getHeight()*0.5f);

		s_cloud22 = dg->spritesheet->getSubImage(dg->desc->getItemByName("sprites/cloud-2x2.png"));
		s_cloud22->setCenterOfRotation(s_cloud22->getWidth()/2, s_cloud22->getHeight()*0.5f);

		s_cloud33 = dg->spritesheet->getSubImage(dg->desc->getItemByName("sprites/cloud-3x3.png"));
		s_cloud33->setCenterOfRotation(s_cloud33->getWidth()/2, s_cloud33->getHeight()*0.5f);

		s_cloud44 = dg->spritesheet->getSubImage(dg->desc->getItemByName("sprites/cloud-4x4.png"));
		s_cloud44->setCenterOfRotation(s_cloud44->getWidth()/2, s_cloud44->getHeight()*0.5f);

		s_cloud55 = dg->spritesheet->getSubImage(dg->desc->getItemByName("sprites/cloud-5x5.png"));
		s_cloud55->setCenterOfRotation(s_cloud55->getWidth()/2, s_cloud55->getHeight()*0.5f);

		s_cloud77 = dg->spritesheet->getSubImage(dg->desc->getItemByName("sprites/cloud-7x7.png"));
		s_cloud77->setCenterOfRotation(s_cloud77->getWidth()/2, s_cloud77->getHeight()*0.5f);

		s_bloodSelection = new vector<Image*>();
		s_bloodSelection->push_back(s_cloud11);
		s_bloodSelection->push_back(s_cloud22);
		s_bloodSelection->push_back(s_cloud33);
		s_bloodSelection->push_back(s_cloud44);
	}

    m_bounds = new ARK::Core::Geometry::Rectangle(0, 0, 6, 6);
	m_bounds->setLocationByCenter(64, 32);

	
	m_velocityMax->set(400, 400);
	m_acceleration->set(0, 0);

	m_animation = new Animation();
	m_animation->setFrameTime(0.1f);

	reset();
}

void MyParticle::reset() {
	m_animation->clear();

	m_flipX = false;
	m_velocity->set(0, 0);

	m_timer = 0.0f;
	m_duration = 5.0f;

    m_color = NULL;
}

void MyParticle::update(GameContainer* container, GameTimer* timer) {
	Object::update(container, timer);

	DefaultGame* dg = DefaultGame::getInstance();
	InGameState* igs = dg->stateInGame;

	if (m_type == TYPE_PLAYER_GIB || m_type == TYPE_PLAYER_ROLL || m_type == TYPE_BLOOD) {
		gravity();
	}

	m_timer += timer->getDelta();
	if (m_timer >= m_duration){
		m_timer = 0.0f;
		setPendingRemoval(true);
	}
	

	m_bounds->asRectangle()->setLocationByCenter(
		m_bounds->getCenterX() + (m_velocity->getX() * timer->getDelta()),
		m_bounds->getCenterY() + (m_velocity->getY() * timer->getDelta())
	);

}
void MyParticle::collision(Object* other) {

}

void MyParticle::gravity() {
	gravity(1.0f);
}
void MyParticle::gravity(float multiplier) {
	// Add Gravity
	GameTimer* timer = ARK2D::getContainer()->getTimer();
	//if (!DefaultGame::getInstance()->stateInGame->isCoffeeMode()) {
		m_velocity->add(0, s_gravity * timer->getDelta());
	//}

	m_velocity->restrictMax(m_velocityMax);
}

void MyParticle::render(GameContainer* container, Renderer* r)
{
	InGameState* igs = DefaultGame::getInstance()->stateInGame;
	r->setDrawColor(Color::white);

	Object::render(container, r);

	float rotation = 0;
	if (m_type == TYPE_PLAYER_GIB || m_type == TYPE_PLAYER_ROLL || m_type == TYPE_BLOOD || m_type == TYPE_GLOVE_EFFECT || m_type == TYPE_SMOKE) {
		rotation = MathUtil::anglef(0,0,m_velocity->getX(), m_velocity->getY());
	}

	if (m_type == TYPE_BLOOD) {
		r->setDrawColor(Color::red);
	}
	else if (m_type == TYPE_GLOVE_EFFECT) {
		r->setDrawColor("#e11f1f",1.0f);
	}
	else if (m_type == TYPE_BOMB_FUSE) {
		r->setDrawColor(m_color);
	}

	Image* img = m_animation->getCurrentFrame();
	img->setRotation(rotation);
	if (m_type == TYPE_TILE_ADVANCE) {
		float sc = Easing::easebetween(Easing::QUADRATIC_IN, m_timer, 1.0f, 3.0f, m_duration);
		img->setAlpha(std::max(1.0f - (m_timer / 1.0f), 0.0f));
		img->drawCenteredScaled(m_bounds->getCenterX(), m_bounds->getCenterY(), sc, sc);
		img->setAlpha(1.0f);
	}
	else if (m_type == TYPE_GLOVE_EFFECT || m_type == TYPE_BOMB_FUSE) {
		float sc = Easing::easebetween(Easing::QUADRATIC_IN, m_timer, 1.0f, 0.0f, m_duration);;
		img->drawCenteredScaled(m_bounds->getCenterX(), m_bounds->getCenterY(), sc, sc);
	}
	else if (m_type == TYPE_SMOKE) {
		float alpha = Easing::easebetween(Easing::QUADRATIC_IN, m_timer, 1.0f, 0.0f, m_duration);;
		if (m_color != NULL) {
			img->setColor(m_color);
		}
		img->setAlpha(alpha);
		img->drawCentered(m_bounds->getCenterX(), m_bounds->getCenterY());
		img->setAlpha(1.0f);
		img->setColor(NULL);
	}
	else if (m_type == TYPE_BOMB_CIRCLE) {
		float alpha = Easing::easebetween(Easing::QUADRATIC_OUT, m_timer, 1.0f, 0.0f, m_duration);
        r->setDrawColorf(1.0f, 1.0f, 1.0f, alpha);
		r->fillCircle(m_bounds->getCenterX(), m_bounds->getCenterY(), m_bombCircleRadius * alpha);
	}
	else if (m_type == TYPE_PLAYER_ROLL) {
		float sc = Easing::easebetween(Easing::QUADRATIC_IN, m_timer, 0.5f, 0.0f, m_duration);
		img->setAlpha(std::max(1.0f - (m_timer / 1.0f), 0.0f));
		img->drawCenteredScaled(m_bounds->getCenterX(), m_bounds->getCenterY(), sc, sc);
		img->setAlpha(1.0f);
	}
	 
	else {
		img->drawCenteredFlipped(m_bounds->getCenterX(), m_bounds->getCenterY(), m_flipX, false);
	}
	img->setRotation(0);

	//r->setDrawColor(Color::white);
	//r->drawRect(m_bounds->getMinX(), m_bounds->getMinY(), m_bounds->getWidth(), m_bounds->getHeight());
}

MyParticle::~MyParticle() {

}
