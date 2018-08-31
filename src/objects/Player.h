/*
 * Player.h
 *
 *  Created on: 29 Jan 2011
 *      Author: Ashley
 */

#ifndef PLAYER_H_
#define PLAYER_H_

#include "../ARK.h"
#include "Object.h"

class Tile;
class EightWayMovementComponent;

class Player : public Object {
	public:
		static Image* s_playerIdle1;
		static Image* s_playerIdle2;
	
		static Image* s_playerGibHead;
		static Image* s_playerGibBody;
		static Image* s_playerGibHand;
		static Image* s_playerGibFoot;
		
		static Image* s_playerRoll;
		static Image* s_playerRollParticle;

	public:
		EightWayMovementComponent* m_movement;
		bool m_grounded;

		Animation* m_animationIdle;
		Animation* m_animationIdleBack;
		
		bool m_alive;
		bool m_moving;
		bool m_facingLeft;
		bool m_facingUp;
		float m_jumpRotation;

		float m_walkWobbleMaxRotation;
		float m_walkWobbleTimer;
		float m_walkWobbleDuration;

		static const unsigned int WEAPON_GLOVE = 0;
		static const unsigned int WEAPON_BOMB = 1;
		int m_currentWeapon;

		float m_punchChargeTimer;
		float m_punchChargeDuration;
		float m_punchFireTimer;
		float m_punchFireDuration;
		Circle<float>* m_punchBounds;
		float m_punchAngle;
		float m_punchParticleTimer;
		float m_punchParticleDuration;

		float m_bombLayTimer;
		float m_bombLayDuration;

		vector<Tile*> m_tilesTouched;

		Vector2<float>* m_velocityRunningMax;
		Vector2<float>* m_velocityRollingMax;

		ARK::Core::Geometry::Rectangle* m_rollBounds;
		Image* m_rollSprite;
		float m_rollRotation;
		bool m_rolling;
		float m_rollTimer;
		float m_rollDuration;
		float m_rollParticleTimer;
		float m_rollParticleDuration;


		int m_dieReason;
		static const unsigned int DIE_NULL = 0;
		static const unsigned int DIE_ARROW = 1;
		static const unsigned int DIE_FALL = 2;
		static const unsigned int DIE_BOMB = 3;
		static const unsigned int DIE_BUZZSAW = 4;
		static const unsigned int DIE_SPIKE = 5;

	public:
		Player();
		virtual void reset();
		void resetWeapons();
		void die();
		void jump();
		void jumppad(float m);
		bool canMove();

		Rectangle* mainBounds();


		void nextWave();
		virtual void update(GameContainer* container, GameTimer* timer);
		void updateWeapon();
		void updatePunchAngle();
		void updateGloveParticles();
		bool isPunchDangerous();
		float getGloveScale();
		virtual void onBoundsMoved();

		virtual void render(GameContainer* container, Renderer* r);
		virtual void collision(Object* other);
		virtual ~Player();
};

#endif /* PLAYER_H_ */
