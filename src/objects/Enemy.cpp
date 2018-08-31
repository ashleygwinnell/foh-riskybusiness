/*
 * Enemy.cpp
 *
 *  Created on: 29 Jan 2011
 *      Author: Ashley
 */

#include "Enemy.h"
#include "Player.h"
#include "MyParticle.h"
#include "../DefaultGame.h"

#include "EightWayMovementComponent.h"
#include <ARK2D/Core/+.h>
#include <ARK2D/Audio/Sound.h>

Image* Enemy::s_enemy = NULL;

Enemy::Enemy():
	Object()
	{

	DefaultGame* dg = DefaultGame::getInstance();

	if (s_enemy == NULL) {
		// small fly
		s_enemy = dg->spritesheet->getSubImage(dg->desc->getItemByName("sprites/enemy.png"));
		s_enemy->setCenterOfRotation(s_enemy->getWidth()*0.5f, s_enemy->getHeight()*0.5f);
	}

    m_bounds = new ARK::Core::Geometry::Rectangle(0, 0, 3, 6);
	m_bounds->setLocationByCenter(64, 32);

	m_velocity->set(0, 0);
	m_velocityMax->set(40, 100);
	m_acceleration->set(0, 0);

	m_animationIdle = new Animation();
	m_animationIdle->setFrameTime(0.5f);

	m_moving = false;
	m_facingLeft = false;
	m_facingUp = false;

	m_type = TYPE_NORMAL;

	reset();
}

void Enemy::reset() {
	m_grounded = true;

	m_walkWobbleTimer = 0.0f;
	m_walkWobbleDuration = 0.25f;
	m_walkWobbleMaxRotation = 7.0f;
	
	m_animationIdle->clear();
	m_animationIdle->addFrame(s_enemy);
}

void Enemy::die() {
	DefaultGame* dg = DefaultGame::getInstance();
	InGameState* igs = dg->stateInGame;

	ARK2D::getLog()->e("enemy die");

	// if (igs->isDead()) { return; }

	// dg->m_sfxDie->play();
	// dg->m_saveFile->clearUnsaved();

//	igs->m_deadTimer = 0.001f;
	if (!isPendingRemoval() ) {
		//if (!igs->m_processingWave) {
		//	dg->m_sfxButterflySmallDie->play();
		//}
	}

	setPendingRemoval(true);


}

void Enemy::killParticles(float startAngle, float power, float powerSpread) {
    DefaultGame* dg = DefaultGame::getInstance();
    InGameState* igs = dg->stateInGame;
    igs->spawnBlood(m_bounds->getCenterX(), m_bounds->getCenterY(), 1, 4);
}


void Enemy::update(GameContainer* container, GameTimer* timer) {
	Object::update(container, timer);

	DefaultGame* dg = DefaultGame::getInstance();
	InGameState* igs = dg->stateInGame;

	bool keyDown = true;
	if (m_facingLeft) {

	} else {

	}

	//m_velocity->setX(0);
	//m_velocity->setY(0);
    
	//if (igs->m_player->m_moving ){//|| !igs->m_player->m_grounded) {
	if (true) {
		m_moving = true;
		// float distance = 1.0f;
		float maxAcceleration = 1600.0f;
        
        float angle = (m_facingLeft)?180:0;

		m_acceleration->set(0.0f, 0.0f);
		MathUtil::moveAngle<float>(m_acceleration, angle, maxAcceleration);

		m_velocity->addX(m_acceleration->getX() * timer->getDelta());
		m_velocity->addY(m_acceleration->getY() * timer->getDelta());
		m_velocity->restrictMax(m_velocityMax->getX(), m_velocityMax->getY());
		
		//m_acceleration->set(m_accelerationMax);

		// MathUtil::moveAngle<float>(m_acceleration, angle, maxSpeed);
  //       float maxSpeed = m_velocityMax->getX();
		// if (m_velocity->getX() < -maxSpeed) { m_velocity->setX(-maxSpeed); }
		// else if (m_velocity->getX() > maxSpeed) { m_velocity->setX(maxSpeed); }
		//if (m_velocity->getY() < -maxSpeed) { m_velocity->setY(-maxSpeed); }
		//else if (m_velocity->getY() > maxSpeed) { m_velocity->setY(maxSpeed); }

		m_walkWobbleTimer += timer->getDelta();
		if (m_walkWobbleTimer >= m_walkWobbleDuration) {
			m_walkWobbleTimer -= m_walkWobbleDuration;
		}

		//m_animationIdle->setTime(0.0f);
		m_animationIdle->setFrameTime(0.2f);
	} else {

		m_moving = false;
		m_acceleration->set(0.0f, 0.0f);
		
		m_velocity->frictionX(2400 * timer->getDelta());
		//m_velocity->frictionY(600 * timer->getDelta());

		m_walkWobbleTimer = 0.0f;

		m_animationIdle->setFrameTime(0.5f);
	}
	m_animationIdle->update(timer);
	


	

//	if (m_introTimer > 0.0f) {
//		m_introTimer += timer->getDelta();
//		if (m_introTimer >= m_introDuration) {
//			m_introTimer = 0.0f;
//		}
//	}

	// cool shield behavioiur
	// if (m_type == TYPE_BUTTERFLY_SMALL) {
	// 	// move towards player
	// 	float angle = MathUtil::anglef(
	// 		m_bounds->getCenterX(), 
	// 		m_bounds->getCenterY(), 
	// 		igs->m_player->m_bounds->getCenterX(),
	// 		igs->m_player->m_bounds->getCenterY()
	// 	);
	// 	MathUtil::moveAngle<float>(m_velocity, angle - 15 + MathUtil::randBetweenf(0.0f, 30.0f), 20.0f);
	// 	m_velocity->restrictMax(m_velocityMax);

	// 	if (m_bounds->collides(igs->m_player->m_bounds)) {
	// 		float dist = MathUtil::distance(
	// 			m_bounds->getCenterX(), 
	// 			m_bounds->getCenterY(), 
	// 			igs->m_player->m_bounds->getCenterX(),
	// 			igs->m_player->m_bounds->getCenterY()
	// 		);
	// 	}
	// }


    m_animationIdle->update(timer);

    gravity(1.0f);

	m_bounds->asRectangle()->setLocationByCenter(
		m_bounds->getCenterX() + (m_velocity->getX() * timer->getDelta()),
		m_bounds->getCenterY() + (m_velocity->getY() * timer->getDelta())
	);

}
void Enemy::collision(Object* other) {

}



void Enemy::render(GameContainer* container, Renderer* r)
{
	InGameState* igs = DefaultGame::getInstance()->stateInGame;
	r->setDrawColor(Color::white);
	//ColourObject::render(container, r);
	Image* img = NULL;
	img = m_animationIdle->getCurrentFrame();
	

	if (!m_facingLeft) { img->flip(true, false); }
	if (!m_grounded) { img->setRotation(0); }

	float rotation = 0.0f;
	float qwd = m_walkWobbleDuration*0.25f;
    
    if (m_walkWobbleTimer > 0.0f) {
    	//ARK2D::getLog()->e(StringUtil::appendf("m_walkWobbleTimer", m_walkWobbleTimer));
		if (m_walkWobbleTimer <= qwd) {
			rotation = Easing::easebetween(Easing::LINEAR, m_walkWobbleTimer, 0.0f, -m_walkWobbleMaxRotation, qwd);
		} else if (m_walkWobbleTimer <= qwd*3) {
			rotation = Easing::easebetween(Easing::LINEAR, m_walkWobbleTimer-qwd, -m_walkWobbleMaxRotation, m_walkWobbleMaxRotation, qwd*2);
		} else if (m_walkWobbleTimer <= qwd*4) {
			rotation = Easing::easebetween(Easing::LINEAR, m_walkWobbleTimer-(qwd*3), m_walkWobbleMaxRotation, 0.0f, qwd);
		}
	}
	img->setRotation(rotation);
	img->drawPivoted(m_bounds->getCenterX(), m_bounds->getCenterY(), 1.5f, 3.0f, 1.0f);


	if (!m_grounded) { img->setRotation(0.0f); }
	if (!m_facingLeft) { img->flip(true, false); }
}

Enemy::~Enemy() {

}
