/*
 * ArrowBox.h
 *
 *  Created on: 29 Jan 2011
 *      Author: Ashley
 */

#ifndef BUZZSAW_H_
#define BUZZSAW_H_

#include "../ARK.h"
#include "Object.h"

class Buzzsaw : public Object {
	public:
		static Image* s_frame1;
		static Image* s_frame2;
		
		Animation* m_animation;
	public:
		Buzzsaw();
		virtual void reset();
		virtual void update(GameContainer* container, GameTimer* timer);
		virtual void render(GameContainer* container, Renderer* r);
		virtual void collision(Object* other);
		virtual ~Buzzsaw();
};

#endif /* BUZZSAW_H_ */
