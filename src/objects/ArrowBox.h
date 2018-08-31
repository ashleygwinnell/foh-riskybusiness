/*
 * ArrowBox.h
 *
 *  Created on: 29 Jan 2011
 *      Author: Ashley
 */

#ifndef ARROW_BOX_H_
#define ARROW_BOX_H_

#include "../ARK.h"
#include "Object.h"

class ArrowBox : public Object {
	public:
		static Image* s_boxFull;
		static Image* s_boxEmpty;
		static Image* s_arrow;
		
	public:
		float m_shootTimer;
		float m_shootDuration;
		float m_reloadTimer;
		float m_reloadDuration;
		bool m_hasArrow;

		bool m_facingLeft;

	public:
		ArrowBox();
		void shoot();
		virtual void reset();
		virtual void update(GameContainer* container, GameTimer* timer);
		virtual void render(GameContainer* container, Renderer* r);
		virtual void collision(Object* other);
		virtual ~ArrowBox();
};

#endif /* ARROW_BOX_H_ */
