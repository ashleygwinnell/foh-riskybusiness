/*
 * Enemy.h
 *
 *  Created on: 29 Jan 2011
 *      Author: Ashley
 */

#ifndef ENEMY_H_
#define ENEMY_H_

#include "../ARK.h"
#include "Object.h"

class Enemy : public Object {
	public:
		static Image* s_enemy;
		
	public:
		Animation* m_animationIdle;
		
		bool m_grounded;
		bool m_moving;
		bool m_facingLeft;
		bool m_facingUp;

		float m_walkWobbleMaxRotation;
		float m_walkWobbleTimer;
		float m_walkWobbleDuration;

		int m_type;
		static const unsigned int TYPE_NORMAL = 0;

	public:
		Enemy();
		virtual void reset();
		void die();
		void killParticles(float startAngle, float power, float powerSpread);
		virtual void update(GameContainer* container, GameTimer* timer);
		virtual void render(GameContainer* container, Renderer* r);
		virtual void collision(Object* other);
		virtual ~Enemy();
};

#endif /* ENEMY_H_ */
