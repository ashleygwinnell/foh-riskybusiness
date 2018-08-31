/*
 * Object.h
 * 18 April 2012
 */

#ifndef OBJECT_H_
#define OBJECT_H_

#include "../ARK.h"

class DefaultGame;
#include <ARK2D/Core/GameObject.h>

class Object : public GameObject {
	public:

		Shape<float>* m_bounds;
		Vector2<float>* m_velocity;
		Vector2<float>* m_velocityMax;
		Vector2<float>* m_acceleration;
		float m_radius;

		Object();
		void update(GameContainer* container, GameTimer* timer);
		void render(GameContainer* container, Renderer* r);

		virtual void fireParticles();

		inline Shape<float>* getBounds() { return m_bounds; }
		inline Vector2<float>* getVelocity() { return m_velocity; }

		virtual void reset();
		virtual void move(float multiplier);
		virtual void gravity();
		virtual void gravity(float multiplier);
		virtual void collision(Object* other);

		virtual void onBoundsMoved() { }

		virtual ~Object();

		static float s_gravity;
};

#endif /* OBJECT_H_ */
