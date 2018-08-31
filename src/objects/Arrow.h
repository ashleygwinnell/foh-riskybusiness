/*
 * Arrow.h
 *
 *  Created on: 29 Jan 2011
 *      Author: Ashley
 */

#ifndef ARROW_H_
#define ARROW_H_

#include "../ARK.h"
#include "Object.h"

class Arrow : public Object {
	public:
		static Image* s_arrow;

	public:
		Arrow();
		virtual void reset();
		virtual void update(GameContainer* container, GameTimer* timer);
		virtual void render(GameContainer* container, Renderer* r);
		virtual void collision(Object* other);
		virtual ~Arrow();
};

#endif /* ARROW_H_ */
