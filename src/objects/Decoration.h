/*
 * Decoration.h
 *
 *  Created on: 29 Jan 2011
 *      Author: Ashley
 */

#ifndef DECORATION_H_
#define DECORATION_H_

#include "../ARK.h"
#include "Object.h"

class Decoration : public Object {
	public:
		
		Animation* m_animation;

	public:
		Decoration();
		virtual void reset();
		virtual void update(GameContainer* container, GameTimer* timer);
		virtual void render(GameContainer* container, Renderer* r);
		virtual void collision(Object* other);
		virtual ~Decoration();
};

#endif /* DECORATION_H_ */
