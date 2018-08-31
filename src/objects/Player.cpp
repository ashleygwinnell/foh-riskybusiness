/*
 * Player.cpp
 *
 *  Created on: 29 Jan 2011
 *      Author: Ashley
 */

#include "Player.h"
#include "MyParticle.h"
#include "Bomb.h"
#include "Enemy.h"
#include "Collectable.h"
#include "../DefaultGame.h"

#include "EightWayMovementComponent.h"
#include <ARK2D/Core/+.h>
#include <ARK2D/Tiled/TiledMap.h>
#include <ARK2D/Audio/Sound.h>
#include <ARK2D/Util/CameraShake.h>

 // http://gamejolt.com/dashboard/developer/games/news/add/21231/

Image* Player::s_playerIdle1 = NULL;
Image* Player::s_playerIdle2 = NULL;

Image* Player::s_playerGibHead = NULL;
Image* Player::s_playerGibBody = NULL;
Image* Player::s_playerGibHand = NULL;
Image* Player::s_playerGibFoot = NULL;

Image* Player::s_playerRoll = NULL;
Image* Player::s_playerRollParticle = NULL;

Player::Player():
	Object(),
	m_movement(NULL),
	m_tilesTouched()
	{

	DefaultGame* dg = DefaultGame::getInstance();

	if (s_playerIdle1 == NULL) {
		s_playerIdle1 = dg->spritesheet->getSubImage(dg->desc->getItemByName("sprites/player.png"));
		s_playerIdle1->setCenterOfRotation(s_playerIdle1->getWidth()/2, s_playerIdle1->getHeight());

		s_playerIdle2 = dg->spritesheet->getSubImage(dg->desc->getItemByName("sprites/player.png"));
		s_playerIdle2->setCenterOfRotation(s_playerIdle2->getWidth()/2, s_playerIdle2->getHeight());

		// gibs
		s_playerGibHead = dg->spritesheet->getSubImage(dg->desc->getItemByName("sprites/player-gib-head.png"));
		s_playerGibHead->setCenterOfRotation(s_playerGibHead->getWidth()*0.5f, s_playerGibHead->getHeight()*0.5f);

		s_playerGibBody = dg->spritesheet->getSubImage(dg->desc->getItemByName("sprites/player-gib-body.png"));
		s_playerGibBody->setCenterOfRotation(s_playerGibBody->getWidth()*0.5f, s_playerGibBody->getHeight()*0.5f);

		s_playerGibHand = dg->spritesheet->getSubImage(dg->desc->getItemByName("sprites/player-gib-hand.png"));
		s_playerGibHand->setCenterOfRotation(s_playerGibHand->getWidth()*0.5f, s_playerGibHand->getHeight()*0.5f);

		s_playerGibFoot = dg->spritesheet->getSubImage(dg->desc->getItemByName("sprites/player-gib-foot.png"));
		s_playerGibFoot->setCenterOfRotation(s_playerGibFoot->getWidth()*0.5f, s_playerGibFoot->getHeight()*0.5f);

		s_playerRoll = dg->spritesheet->getSubImage(dg->desc->getItemByName("sprites/player-roll.png"));
		s_playerRoll->setCenterOfRotation(s_playerRoll->getWidth()*0.5f, s_playerRoll->getHeight()*0.5f);

		s_playerRollParticle = dg->spritesheet->getSubImage(dg->desc->getItemByName("sprites/cloud-3x3.png"));
		s_playerRollParticle->setCenterOfRotation(s_playerRollParticle->getWidth()*0.5f, s_playerRollParticle->getHeight()*0.5f);
	}

    m_bounds = new ARK::Core::Geometry::Rectangle(0, 0, 3, 6);
    m_rollBounds = new ARK::Core::Geometry::Rectangle(0, 0, 2.5f, 2.5f);

    m_velocity->set(0, 0);
	m_velocityMax->set(40, 100);
	m_velocityRunningMax = new Vector2<float>(80, 100);
	m_velocityRollingMax = new Vector2<float>(120, 100);
	m_acceleration->set(0, 0);

	m_punchBounds = new ARK::Core::Geometry::Circle<float>(0,0,4);

	m_movement = new EightWayMovementComponent(this);
	m_movement->setController(true, 0, Gamepad::ANALOG_STICK_1_X);
	m_movement->setKeysWASD();

	m_grounded = true;

	Image* idle_f1 = s_playerIdle1->getFlippedCopy(false, false);
	Image* idle_f2 = s_playerIdle2->getFlippedCopy(false, false);
	m_animationIdle = new Animation();
	m_animationIdle->addImage(idle_f1);
	m_animationIdle->addImage(idle_f2);
	m_animationIdle->setFrameTime(0.5f);

	m_animationIdleBack = new Animation();
	m_animationIdleBack->addImage(idle_f1);
	m_animationIdleBack->addImage(idle_f2);
	m_animationIdleBack->setFrameTime(0.5f);

	reset();
}

void Player::reset() {
	//m_movement->reset();

	m_walkWobbleTimer = 0.0f;
	m_walkWobbleDuration = 0.25f;
	m_walkWobbleMaxRotation = 7.0f;

	m_rollSprite = s_playerRoll;
	m_rollRotation = 0.0f;
	m_rollTimer = 0.0f;
	m_rollDuration = 0.5f;
	m_rollParticleTimer = 0.0f;
	m_rollParticleDuration = 0.075f;
	m_rolling = false;

	m_currentWeapon = WEAPON_GLOVE;

	m_alive = true;
	m_moving = false;
	m_facingLeft = false;
	m_facingUp = false;
	m_jumpRotation = 0.0f;
	m_radius = 3;

	m_dieReason = DIE_NULL;

	resetWeapons();

	
	
	DefaultGame* dg = DefaultGame::getInstance();
	InGameState* igs = dg->stateInGame;
	
	m_velocity->set(0,0);
	m_acceleration->set(0,0);
	m_bounds->setLocation(0,0
	 //	igs->worldStartX + (igs->worldWidth * igs->worldTileSize * 0.5f),
	 //	igs->worldStartY + (igs->worldHeight * igs->worldTileSize * 0.5f)
	);
	//ARK2D::getLog()->e(StringUtil::appendf("player x ", m_bounds->getCenterX()));
	//ARK2D::getLog()->e(StringUtil::appendf("player y ", m_bounds->getCenterY()));
}

void Player::resetWeapons() {
	m_punchChargeTimer = 0.0f;
	m_punchChargeDuration = 0.5f;
	m_punchFireTimer = 0.0f;
	m_punchFireDuration = 0.8f;
	
	m_punchParticleTimer = 0.0f;
	m_punchParticleDuration = 0.05f;

	m_bombLayTimer = 0.0f;
	m_bombLayDuration = 2.0f;
}

void Player::nextWave() {
	DefaultGame* dg = DefaultGame::getInstance();
	InGameState* igs = dg->stateInGame;

	// assign new weapon. 
//	if (igs->m_waveNumber > 0) { 
//		if (m_tilesTouched.size() > igs->m_tilesLinear.size() * 0.35f) {
//			m_currentWeapon = WEAPON_GLOVE;
//		} else {
//			m_currentWeapon = WEAPON_BOMB;
//		}
//	}
//	resetWeapons();

	// do new things
	m_alive = true;

	// clear touched tiles. 
	m_tilesTouched.clear();
}

void Player::die() {
	DefaultGame* dg = DefaultGame::getInstance();
	InGameState* igs = dg->stateInGame;

	

	// if (igs->isDead()) { return; }

	
	// dg->m_saveFile->clearUnsaved();

	

	//if (igs->m_deadTimer == 0.0f) {



		ARK2D::getLog()->e("player die");
		dg->m_sfxPlayerDie->play();

		igs->m_cameraShake->setSpeedMultiplier(2, 0.5f);
		igs->m_cameraShake->start(2, 0.5f);

		// music lpf
//		if (dg->m_musicCurrent->isPlaying()) { 
//			dg->m_musicCurrent->setLowpass(true); 
//			dg->m_musicCurrent->setLowpassFrequency(500.0f);
//		}

		// spawn particles
		float diePower = 25.0f;
		MyParticle* part = NULL;
		
		//head
		part = igs->m_particles->get();
		part->reset();
		part->m_animation->addFrame(s_playerGibHead);
		part->m_type = MyParticle::TYPE_PLAYER_GIB;
		part->m_bounds->setLocationByCenter(m_bounds->getCenterX(), m_bounds->getMinY() - 6);
		part->m_flipX = !m_facingLeft;
		MathUtil::moveAngle<float>(part->m_velocity, MathUtil::randBetweenf(225,315), diePower);

		part = igs->m_particles->get();
		part->reset();
		part->m_animation->addFrame(s_playerGibBody);
		part->m_type = MyParticle::TYPE_PLAYER_GIB;
		part->m_bounds->setLocationByCenter(m_bounds->getCenterX(), m_bounds->getMinY() - 1);
		part->m_flipX = !m_facingLeft;
		MathUtil::moveAngle<float>(part->m_velocity, MathUtil::randBetweenf(225,315), diePower);

		// spawn blood...
		igs->spawnBlood(m_bounds->getCenterX(), m_bounds->getMinY() - 3, 3.0f, 4);

		// hands/feet
		part = igs->m_particles->get();
		part->reset();
		part->m_animation->addFrame(s_playerGibFoot);
		part->m_type = MyParticle::TYPE_PLAYER_GIB;
		part->m_bounds->setLocationByCenter(m_bounds->getCenterX() - 2, m_bounds->getMaxY() - 2);
		MathUtil::moveAngle<float>(part->m_velocity, MathUtil::randBetweenf(210,330), diePower);

		part = igs->m_particles->get();
		part->reset();
		part->m_animation->addFrame(s_playerGibFoot);
		part->m_type = MyParticle::TYPE_PLAYER_GIB;
		part->m_bounds->setLocationByCenter(m_bounds->getCenterX() + 2, m_bounds->getMaxY() - 2);
		MathUtil::moveAngle<float>(part->m_velocity, MathUtil::randBetweenf(210,330), diePower);

		part = igs->m_particles->get();
		part->reset();
		part->m_animation->addFrame(s_playerGibHand);
		part->m_type = MyParticle::TYPE_PLAYER_GIB;
		part->m_bounds->setLocationByCenter(m_bounds->getCenterX() - 4, m_bounds->getMinY() - 1);
		MathUtil::moveAngle<float>(part->m_velocity, MathUtil::randBetweenf(210,330), diePower);

		part = igs->m_particles->get();
		part->reset();
		part->m_animation->addFrame(s_playerGibHand);
		part->m_type = MyParticle::TYPE_PLAYER_GIB;
		part->m_bounds->setLocationByCenter(m_bounds->getCenterX() + 4, m_bounds->getMinY() - 1);
		MathUtil::moveAngle<float>(part->m_velocity, MathUtil::randBetweenf(210,330), diePower);



	//	igs->m_deadTimer = 0.01f;
		//m_alive = false;
	//}

	//igs->m_cameraShake->setSpeedMultiplier(1, 0.5f);
	//igs->m_cameraShake->start(1, 2.0f);

	igs->m_cameraShake->setSpeedMultiplier(2, 0.5f);
	igs->m_cameraShake->start(2, 0.5f);

		m_bounds->setLocation(igs->m_playerStart->getX(), igs->m_playerStart->getY());
		fireParticles();

	igs->m_died = true;
}

void Player::onBoundsMoved() {
	m_rollBounds->setLocationByCenter(m_bounds->getCenterX(), m_bounds->getCenterY());

	DefaultGame* dg = DefaultGame::getInstance();
	InGameState* igs = dg->stateInGame;
	if (m_bounds->getMaxX() < -4 || 
		m_bounds->getMinX() > igs->m_map->getWidth() + 4 ||
		m_bounds->getMinY() > igs->m_map->getHeight() + 8

		) {
		die();
	}
}
Rectangle* Player::mainBounds() {
	if (m_rolling) {
		return m_rollBounds;
	}
	return dynamic_cast<Rectangle*>(m_bounds);
}

void Player::update(GameContainer* container, GameTimer* timer) {
	Object::update(container, timer);

	if (!m_alive) { 
		return; 
	}

	DefaultGame* dg = DefaultGame::getInstance();
	InGameState* igs = dg->stateInGame;

	//if (canMove()) {
		m_movement->update(container, timer);
	//}



	//m_velocity->set(m_movement->getX() * 60.0f, m_movement->getY() * 60.0f);

	Input* in = ARK2D::getInput();


	if (in->isKeyPressed(Input::KEY_L)) {
		m_currentWeapon = (m_currentWeapon == WEAPON_GLOVE) 
			? WEAPON_BOMB
			: WEAPON_GLOVE;
	}

	if (!m_rolling && 
		(in->isKeyPressed(Input::KEY_S) || in->isKeyPressed(Input::KEY_DOWN))) {
		m_rollTimer = 0.01f;
		m_rolling = true;
	}

	if (m_rollTimer >= 0.0f) {

		if (m_rolling) {
			m_rollParticleTimer += timer->getDelta();
			if (m_rollParticleTimer >= m_rollParticleDuration) {
				m_rollParticleTimer -= m_rollParticleDuration;

				DefaultGame* dg = DefaultGame::getInstance();
			    InGameState* igs = dg->stateInGame;

				MyParticle* part = NULL;
				float partAngleMin = (m_facingLeft)?-20:180;
				float partAngleMax = (m_facingLeft)?0:200;
				
				part = igs->m_particles->get();
				part->reset();
				part->m_animation->addFrame(s_playerRollParticle);
				part->m_duration = MathUtil::randBetweenf(1.0f, 2.0f);
				part->m_type = MyParticle::TYPE_PLAYER_ROLL;
				part->m_bounds->setLocationByCenter(m_bounds->getCenterX(), m_bounds->getCenterY());
				part->m_velocity->set(0.0f, 0.0f);
				MathUtil::moveAngle<float>(part->m_velocity, MathUtil::randBetweenf(partAngleMin, partAngleMax), MathUtil::randBetweenf(10.0f, 20.0f));

			}
		}

		m_rollRotation += 720.0f * timer->getDelta();
		m_rollTimer += timer->getDelta();
		if (m_rollTimer >= m_rollDuration) {
			m_rollTimer = 0.0f;
			m_rolling = false;
		}
	}


	float angle = m_movement->m_lastAngle;
	bool keyDown = m_movement->m_stickPushing || m_movement->m_buttonPushing;

	if (!m_rolling) { 
		if (m_movement->isLeftDown() && m_movement->isRightDown()) {

		} else if (m_movement->isLeftDown()) {
			angle = 180;
			//keyDown = true;
			m_facingLeft = true;
		} else if (m_movement->isRightDown()) {
			angle = 0;
			//keyDown = true;
			m_facingLeft = false;
		}
	} 

	// if (m_movement->isUpDown()) {
	// 	//angle = -90;
	// 	//keyDown = true;
	// 	m_facingUp = true;
	// } 
	// if (m_movement->isDownDown()) {
	// 	//angle = 90;
	// 	//keyDown = true;
	// 	m_facingUp = false;
	// }

	if (m_movement->isJumpPressed() ) { 
		if (m_grounded) {
			jump();
		}
	}// else if (in->isKeyDown(Input::KEY_UP) && !m_grounded) {
	//	m_velocity->m_y -= 150 * timer->getDelta();
 	//}


		bool isRunning = in->isKeyDown(Input::KEY_SHIFT) || in->isKeyDown(Input::KEY_LSHIFT) || in->isKeyDown(Input::KEY_RSHIFT);

	//m_velocity->setX(0);
	//m_velocity->setY(0);
	if (m_rolling) {
		float fangle = (m_facingLeft)?180:0;
		float velocity = Easing::easebetween(Easing::QUADRATIC_OUT, m_rollTimer, 500.0f, 0.0f, m_rollDuration);
		m_acceleration->set(0.0f, 0.0f);
		MathUtil::moveAngle<float>(m_acceleration, fangle, velocity);

		m_velocity->addX(m_acceleration->getX() * timer->getDelta());
		m_velocity->restrictMax(m_velocityRollingMax->getX(), m_velocityRollingMax->getY());

		m_moving = true;

	} else {
		if (keyDown && canMove()) {
			m_moving = true;
			// float distance = 1.0f;
			float maxAcceleration = 1600.0f;;//(isRunning)?3200.0f:1600.0f;

			m_acceleration->set(0.0f, 0.0f);
			MathUtil::moveAngle<float>(m_acceleration, angle, maxAcceleration);

			m_velocity->addX(m_acceleration->getX() * timer->getDelta());
			m_velocity->addY(m_acceleration->getY() * timer->getDelta());

			
			
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
			m_animationIdleBack->setFrameTime(0.2f);
		} else {

			m_moving = false;
			m_acceleration->set(0.0f, 0.0f);
			
			m_velocity->frictionX(2400 * timer->getDelta());
			//m_velocity->frictionY(600 * timer->getDelta());

			m_walkWobbleTimer = 0.0f;

			m_animationIdle->setFrameTime(0.5f);
			m_animationIdleBack->setFrameTime(0.5f);
		}

		if (isRunning) {
			m_velocity->restrictMax(m_velocityRunningMax->getX(), m_velocityRunningMax->getY());
		} else {
			m_velocity->restrictMax(m_velocityMax->getX(), m_velocityMax->getY());
		}
		m_animationIdle->update(timer);
		m_animationIdleBack->update(timer);
	}

	// if (!m_grounded) {
	// 	if (m_facingLeft) {
	// 		m_jumpRotation -= 720 * timer->getDelta();
	// 	} else if (!m_facingLeft) {
	// 		m_jumpRotation += 720 * timer->getDelta();
	// 	}
	// }

	//gravity(1.0f);
	m_velocity->add(0, s_gravity * timer->getDelta());
		

	updateWeapon();


	m_bounds->asRectangle()->setLocationByCenter(
		m_bounds->getCenterX() + (m_velocity->getX() * timer->getDelta()),
		m_bounds->getCenterY() + (m_velocity->getY() * timer->getDelta())
	);
	onBoundsMoved();

}
void Player::updatePunchAngle() {
	m_punchAngle = m_movement->m_lastAngle;//MathUtil::anglef(0, 0, m_movement->m_velocity->getX(), m_movement->m_velocity->getY());
	ARK2D::getLog()->e(StringUtil::appendf("punchAngle - before: ", m_punchAngle));
	m_punchAngle = MathUtil::snap_udlr(m_punchAngle);
	ARK2D::getLog()->e(StringUtil::appendf("punchAngle - after: ", m_punchAngle));
}
void Player::updateWeapon() {
	DefaultGame* dg = DefaultGame::getInstance();
	InGameState* igs = dg->stateInGame;

	Input* in = ARK2D::getInput();
	GameTimer* timer = ARK2D::getTimer();
    GameContainer* container = ARK2D::getContainer();

	// weponz
	if (m_currentWeapon == WEAPON_GLOVE) {
		
		// release the beast
		if (m_punchChargeTimer >= m_punchChargeDuration// && 
			//!in->isKeyDown(Input::KEY_SPACE) && 
			//((m_movement->m_controllerInput && !in->getGamepad(m_movement->m_controllerIndex)->isButtonDown(Gamepad::BUTTON_A)) || 
			//!m_movement->m_controllerInput)) { 
			){
			// TODO: play sfx
			
			updatePunchAngle();
			m_punchFireTimer = 0.01f;
			m_punchChargeTimer = 0.0f;

			//dg->m_sfxPunchGo->play();
			
		} 
		// track the beast.
		else if (m_punchFireTimer > 0.0f) {
			m_punchFireTimer += timer->getDelta();
			if (m_punchFireTimer >= m_punchFireDuration) {
				m_punchFireTimer = 0.0f;
				m_punchBounds->setLocationByCenter(-900, -900);
			} else { 

				float punchStartX = m_bounds->getCenterX();
				float punchStartY = m_bounds->getMinY();
				float dist = Easing::easebetweenback(Easing::QUADRATIC_OUT, Easing::QUADRATIC_OUT, m_punchFireTimer, 0.0f, 40.0f, m_punchFireDuration);
				MathUtil::moveAngle<float>(punchStartX, punchStartY, m_punchAngle, 10 + dist);
				m_punchBounds->setLocationByCenter(punchStartX, punchStartY);
				updateGloveParticles();
			}
		}
		// charge the beast
		else if (in->isKeyDown(Input::KEY_SPACE) || 
			(m_movement->m_controllerInput && in->getGamepad(m_movement->m_controllerIndex) != NULL && in->getGamepad(m_movement->m_controllerIndex)->isButtonDown(Gamepad::BUTTON_A)) ||
                 (container->isTouchMode() && in->isKeyDown(Input::MOUSE_BUTTON_LEFT)))
			
		{
//			if (m_punchChargeTimer == 0.0f) {
//				dg->m_sfxPunchCharge->play();
//			}
			// m_punchChargeTimer += timer->getDelta();
			// if (m_punchChargeTimer >= m_punchChargeDuration) {
			// 	m_punchChargeTimer = m_punchChargeDuration;
			// }

			// updatePunchAngle();

			// float punchStartX = m_bounds->getCenterX();
			// float punchStartY = m_bounds->getMinY();
			// MathUtil::moveAngle<float>(punchStartX, punchStartY, m_punchAngle, 10);
			// m_punchBounds->setLocationByCenter(punchStartX, punchStartY);

			// // glove particles
			// updateGloveParticles();
		} else {
			if (m_punchChargeTimer >= 0.0f) {
//				if (dg->m_sfxPunchCharge->isPlaying()) {
//					dg->m_sfxPunchCharge->stop();
//				}

				m_punchChargeTimer -= timer->getDelta();
				if (m_punchChargeTimer <= 0.0f) {
					m_punchChargeTimer = 0.0f;
					m_punchBounds->setLocationByCenter(-900, -900);
				}
			}
		}

		if (isPunchDangerous()) {
			PoolIterator<Enemy*> it_enemies = igs->m_enemies->newiteratorref();
			while(it_enemies.hasNext()) {
				Enemy* e = it_enemies.next();
				if (e->isPendingRemoval()) { continue; }

				float result[2];
				bool collides = Shape<float>::collision_circleCircleVec(
					&result[0],
					m_punchBounds->getCenterX(), m_punchBounds->getCenterY(), m_punchBounds->getRadius(),
					e->m_bounds->getCenterX(), e->m_bounds->getCenterY(), e->m_radius
				);
				if (collides) {
					e->die();

					float startAngle = MathUtil::anglef(m_punchBounds->getCenterX(), m_punchBounds->getCenterY(), e->m_bounds->getCenterX(), e->m_bounds->getCenterY());
					e->killParticles(startAngle, 100, 20);

					
				}
			}
		}
		
	}
	else if (m_currentWeapon == WEAPON_BOMB) {
		// are there bombs in play?
		if (//igs->m_bombs->sizeActive() == 0 || 
			m_bombLayTimer == 0.0f) {
			// if (in->isKeyPressed(Input::KEY_SPACE) || 
			// 	(m_movement->m_controllerInput && in->getGamepad(m_movement->m_controllerIndex) != NULL && in->getGamepad(m_movement->m_controllerIndex)->isButtonPressed(Gamepad::BUTTON_A)) ||
			// 	(container->isTouchMode() && in->isKeyDown(Input::MOUSE_BUTTON_LEFT)))
			// {
			// 	//Bomb* b = igs->m_bombs->get();
			// 	//b->reset();
			// 	//b->m_bounds->setLocationByCenter(m_bounds->getCenterX(), m_bounds->getCenterY() - 2);
			// 	m_bombLayTimer = 0.01f;

			// //	dg->m_sfxBombPlace->play();
			// //	dg->m_sfxBombFuse->play();
			// }
		}

		if (m_bombLayTimer > 0.0f) {
			m_bombLayTimer += timer->getDelta();
			if (m_bombLayTimer >= m_bombLayDuration) {
				m_bombLayTimer = 0.0f;
			}
		}
	}
}
void Player::updateGloveParticles() {
	
	GameTimer* timer = ARK2D::getTimer();
	m_punchParticleTimer += timer->getDelta();
	if (m_punchParticleTimer >= m_punchParticleDuration) {
		m_punchParticleTimer -= m_punchParticleDuration;

		MyParticle* part = NULL;

		float sx = m_punchBounds->getCenterX();
		float sy = MathUtil::randBetweenf(m_punchBounds->getMinY()+1, m_punchBounds->getMaxY()-1);

		float diePower = MathUtil::randBetween(15.0f, 30.0f);
        float angleToPlayer = MathUtil::anglef(m_punchBounds->getCenterX(), m_punchBounds->getCenterY(), m_bounds->getCenterX(), m_bounds->getCenterY());
        MathUtil::moveAngle<float>(sx, sy, angleToPlayer, 4);

        DefaultGame* dg = DefaultGame::getInstance();
        InGameState* igs = dg->stateInGame;
		part = igs->m_particles->get();
		part->reset();
		part->m_animation->addFrame(MyParticle::s_cloud11);
		part->m_type = MyParticle::TYPE_GLOVE_EFFECT;
		part->m_duration = 0.25f;
		part->m_bounds->setLocationByCenter(sx, sy);
		MathUtil::moveAngle<float>(part->m_velocity, angleToPlayer - 30 + MathUtil::randBetweenf(0,60), diePower);

	}
}
void Player::jump() {
	//return;

	DefaultGame* dg = DefaultGame::getInstance();
	if (dg->m_sfxJump != NULL) dg->m_sfxJump->play();
	//m_velocity->setY(-155);
	m_velocity->setY(-130);
	m_grounded = false;
}
void Player::jumppad(float multiplier) {
	DefaultGame* dg = DefaultGame::getInstance();
	if (dg->m_sfxJump != NULL) dg->m_sfxJump->play();
	//m_velocity->setY(-155.0f * multiplier);
	//m_velocity->setY(-125);
	m_grounded = false;
}
void Player::collision(Object* other) {

}

bool Player::canMove() {
	return m_punchChargeTimer == 0.0f && m_punchFireTimer == 0.0f;
}
bool Player::isPunchDangerous() {
	return m_punchFireTimer > 0.0f && m_punchFireTimer <= m_punchFireDuration*0.5f;
}

float Player::getGloveScale() {
    float scale = 0.0f;
	if (m_punchFireTimer > 0.0f) {
		scale = 1.0f;	
		if (m_punchFireTimer >= m_punchFireDuration*0.5f) {
			scale = Easing::easebetween(Easing::QUADRATIC_IN, m_punchFireTimer-(m_punchFireDuration*0.5f), 1.0f, 0.0f, m_punchFireDuration*0.5f );
		}	
	} else if (m_punchChargeTimer >= 0.0f) {
		scale = Easing::ease(Easing::QUADRATIC_OUT, m_punchChargeTimer, 0.0f, 1.0f, m_punchChargeDuration);
	}
	return scale;
}

void Player::render(GameContainer* container, Renderer* r)
{
	if (!m_alive) { 
		return; 
	}

	InGameState* igs = DefaultGame::getInstance()->stateInGame;
	r->setDrawColor(Color::white);
	//ColourObject::render(container, r);
	Image* img = NULL;
	if (!m_facingUp) {
		img = m_animationIdle->getCurrentFrame();
	} else {
		img = m_animationIdleBack->getCurrentFrame();
	}

	if (m_rolling) {
		img = m_rollSprite;
		img->setRotation(m_rollRotation);
	}

	if (!m_facingLeft) { img->flip(true, false); }
	if (!m_grounded) { img->setRotation(m_jumpRotation); }

	float rotation = 0.0f;
	if (m_rolling) {
		rotation = m_rollRotation;
	}

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
	if (m_rolling) {
		img->drawCentered(m_rollBounds->getCenterX(), m_rollBounds->getCenterY());
	} else {
		img->drawPivoted(m_bounds->getCenterX(), m_bounds->getCenterY(), 1.5f, 3.0f, 1.0f);
	}


	if (!m_grounded) { img->setRotation(0.0f); }
	if (!m_facingLeft) { img->flip(true, false); }


	//Object::render(container, r);
	if (DefaultGame::getInstance()->m_debug && m_bounds != NULL) {
		r->setDrawColor(Color::white);
		if (m_rolling) { 
			m_rollBounds->render();
		} else {
			m_bounds->render();
		}
	}

	// render glove
	// if (m_currentWeapon == WEAPON_GLOVE) { 
	// 	float scale = getGloveScale();
	// 	//s_playerGlove->setRotation(m_punchAngle);
	// 	//s_playerGlove->drawCenteredScaled(m_punchBounds->getCenterX(), m_punchBounds->getCenterY(), scale, scale);
	// }

	

	// r->setDrawColor(Color::white);
	// r->drawRect(m_bounds->getMinX(), m_bounds->getMinY(), m_bounds->getWidth(), m_bounds->getHeight());
	// r->setDrawColor(Color::yellow);
	// r->drawCircle(m_bounds->getCenterX(), m_bounds->getCenterY(), m_radius);

	//r->setDrawColor(Color::yellow);
	//r->drawCircle(m_punchBounds->getCenterX(), m_punchBounds->getCenterY(), m_punchBounds->getRadius());

	//DefaultGame* dg = DefaultGame::getInstance();
	//dg->playerImg->drawCentered(m_bounds->getCenterX(), m_bounds->getCenterY());
}

Player::~Player() {

}
