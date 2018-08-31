/*
 * Trap.h
 *
 *  Created on: 29 Jan 2011
 *      Author: Ashley
 */

#ifndef TRAP_H_
#define TRAP_H_

#include "../ARK.h"
#include "Object.h"

class Trap : public Object {
	public:
		static Image* s_spike;
		static Image* s_blade;
		
		Animation* m_animation;

		float m_bladeRotation;
		float m_bladeRotationSpeed;

		unsigned int m_type;
		static const unsigned int TYPE_SPIKE = 0;
		static const unsigned int TYPE_BLADE = 1;
	public:
		Trap();
		virtual void reset();
		virtual void update(GameContainer* container, GameTimer* timer);
		virtual void render(GameContainer* container, Renderer* r);
		virtual void collision(Object* other);
		virtual ~Trap();
};

#endif /* BUZZSAW_H_ */
