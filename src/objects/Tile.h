/*
 * Tile.h
 *
 *  Created on: 29 Jan 2011
 *      Author: Ashley
 */

#ifndef TILE_H_
#define TILE_H_

#include "../ARK.h"
#include "Object.h"

class Tile : public Object {
	public:
		static Image* s_tileEmpty;

		static Image* s_tileTopLeft;
		static Image* s_tileTop;
		static Image* s_tileLeft;
		static Image* s_tileBottomLeft;
		static Image* s_tileBottom;
		static Image* s_tileMiddle;
		
		static Image* s_tileAdvance;
		static Image* s_tileAdvanceGibPlus;
		static Image* s_tileAdvanceGibBG;
		static Color* s_floorColor;
		static Color* s_bgColor;

		int gx;
		int gy;
		bool solid;
		bool advance;
		bool itemonit;

		float m_introTimer;
		float m_introDuration;

	public:
		Tile();
		void removeSolid();
		bool isEdgeTile();
		virtual void reset();
		virtual void die();
		virtual void update(GameContainer* container, GameTimer* timer);
		virtual void render(GameContainer* container, Renderer* r);
		virtual void collision(Object* other);
		virtual ~Tile();
};

#endif /* TILE_H_ */
