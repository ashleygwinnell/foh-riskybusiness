/*
 * MyParticle.h
 *
 *  Created on: 29 Jan 2011
 *      Author: Ashley
 */

#ifndef MYPARTICLE_H_
#define MYPARTICLE_H_

#include "../ARK.h"
#include "Object.h"

class MyParticle : public Object {
	public:
		static Image* s_cloud11;
		static Image* s_cloud22;
		static Image* s_cloud33;
		static Image* s_cloud44;
		static Image* s_cloud55;
		static Image* s_cloud77;
		static vector<Image*>* s_bloodSelection;
		
	public:
		Animation* m_animation;

		unsigned int m_type;
		static float s_gravity;

		bool m_flipX;
		Color* m_color;

		static const unsigned int TYPE_BLOOD = 0;
		static const unsigned int TYPE_WALKING_DUST = 1;
		static const unsigned int TYPE_GRASS_CHOP = 2;
		static const unsigned int TYPE_FLOWER_CHOP = 3;
		static const unsigned int TYPE_PLAYER_GIB = 4;
		static const unsigned int TYPE_PLAYER_ROLL = 44;
		static const unsigned int TYPE_TILE_ADVANCE = 5;
		static const unsigned int TYPE_GLOVE_EFFECT = 6;
		static const unsigned int TYPE_BOMB_FUSE = 7;
		static const unsigned int TYPE_BOMB_CIRCLE = 8;
		static const unsigned int TYPE_SMOKE = 9;

		float m_timer;
		float m_duration;

		float m_bombCircleRadius;

	public:
		MyParticle();
		virtual void reset();
		virtual void update(GameContainer* container, GameTimer* timer);
		virtual void render(GameContainer* container, Renderer* r);
		virtual void collision(Object* other);
		void gravity();
		void gravity(float mult);
		virtual ~MyParticle();
};

#endif /* MYPARTICLE_H_ */
