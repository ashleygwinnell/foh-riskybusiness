/*
 * Collectable.h
 *
 *  Created on: 29 Jan 2011
 *      Author: Ashley
 */

#ifndef COLLECTABLE_H_
#define COLLECTABLE_H_

#include "../ARK.h"
#include "Object.h"

class Collectable : public Object {
	public:
		static Image* s_dollar;
		static Image* s_dollarGib1;
		static Image* s_dollarGib2;
		
		static Image* s_briefcase;
		
		Animation* m_animation;
		
		unsigned int m_type;
		static const unsigned int TYPE_DOLLAR = 0;
		static const unsigned int TYPE_GOAL = 1;

		float m_particleTimer;
		float m_particleDuration;

		bool m_visible;

	public:
		Collectable();
		void collected();
		virtual void reset();
		virtual void update(GameContainer* container, GameTimer* timer);
		virtual void render(GameContainer* container, Renderer* r);
		virtual void collision(Object* other);
		virtual ~Collectable();
};

#endif /* BUZZSAW_H_ */
