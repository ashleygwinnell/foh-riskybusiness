/*
 * Tile.cpp
 *
 *  Created on: 29 Jan 2011
 *      Author: Ashley
 */

#include "Tile.h"
#include "../DefaultGame.h"
#include "../objects/MyParticle.h"

#include <ARK2D/Core/+.h>
#include <ARK2D/Audio/Sound.h>

Image* Tile::s_tileEmpty = NULL;

Image* Tile::s_tileTopLeft = NULL;
Image* Tile::s_tileTop = NULL;
Image* Tile::s_tileLeft = NULL;
Image* Tile::s_tileBottomLeft = NULL;
Image* Tile::s_tileBottom = NULL;
Image* Tile::s_tileMiddle = NULL;

Image* Tile::s_tileAdvance = NULL;
Image* Tile::s_tileAdvanceGibPlus = NULL;
Image* Tile::s_tileAdvanceGibBG = NULL;

Color* Tile::s_floorColor = NULL;
Color* Tile::s_bgColor = NULL;

Tile::Tile():
	Object()
	{

	DefaultGame* dg = DefaultGame::getInstance();

	if (s_tileEmpty == NULL) {
		s_tileEmpty = dg->spritesheet->getSubImage(dg->desc->getItemByName("white/tile.png"));
		s_tileEmpty->setCenterOfRotation(s_tileEmpty->getWidth()/2, s_tileEmpty->getHeight()/2);

		s_tileTopLeft = dg->spritesheet->getSubImage(dg->desc->getItemByName("sprites/tile-tl.png"));
		s_tileTopLeft->setCenterOfRotation(s_tileTopLeft->getWidth()/2, s_tileTopLeft->getHeight()/2);

		s_tileTop = dg->spritesheet->getSubImage(dg->desc->getItemByName("sprites/tile-top.png"));
		s_tileTop->setCenterOfRotation(s_tileTop->getWidth()/2, s_tileTop->getHeight()/2);

		s_tileLeft = dg->spritesheet->getSubImage(dg->desc->getItemByName("sprites/tile-left.png"));
		s_tileLeft->setCenterOfRotation(s_tileLeft->getWidth()/2, s_tileLeft->getHeight()/2);

		s_tileBottom = dg->spritesheet->getSubImage(dg->desc->getItemByName("sprites/tile-bottom.png"));
		s_tileBottom->setCenterOfRotation(s_tileBottom->getWidth()/2, s_tileBottom->getHeight()/2);

		s_tileBottomLeft = dg->spritesheet->getSubImage(dg->desc->getItemByName("sprites/tile-bl.png"));
		s_tileBottomLeft->setCenterOfRotation(s_tileBottomLeft->getWidth()/2, s_tileBottomLeft->getHeight()/2);

		s_tileMiddle = dg->spritesheet->getSubImage(dg->desc->getItemByName("sprites/tile-middle.png"));
		s_tileMiddle->setCenterOfRotation(s_tileMiddle->getWidth()/2, s_tileMiddle->getHeight()/2);

		s_tileAdvance = dg->spritesheet->getSubImage(dg->desc->getItemByName("sprites/tile-advance.png"));
		s_tileAdvance->setCenterOfRotation(s_tileAdvance->getWidth()/2, s_tileAdvance->getHeight()/2);

		s_tileAdvanceGibPlus = dg->spritesheet->getSubImage(dg->desc->getItemByName("sprites/tile-advance-gib-1.png"));
		s_tileAdvanceGibPlus->setCenterOfRotation(s_tileAdvanceGibPlus->getWidth()/2, s_tileAdvanceGibPlus->getHeight()/2);

		s_tileAdvanceGibBG = dg->spritesheet->getSubImage(dg->desc->getItemByName("sprites/tile-advance-gib-2.png"));
		s_tileAdvanceGibBG->setCenterOfRotation(s_tileAdvanceGibBG->getWidth()/2, s_tileAdvanceGibBG->getHeight()/2);

		s_floorColor = new Color("#9bcc00");
		s_bgColor = new Color("#002503");
	}

    m_bounds = new ARK::Core::Geometry::Rectangle(0, 0, 4, 4);
	m_bounds->setLocationByCenter(0, 0);

	m_velocity->set(0, 0);
	m_velocityMax->set(0, 0);
	m_acceleration->set(0, 0);

}

void Tile::reset() {
	gx = 0;
	gy = 0;
	solid = true;
	advance = false;
	itemonit = false;

	m_introTimer = 0.001f;
	m_introDuration = 0.4f;
}

void Tile::die() {
	DefaultGame* dg = DefaultGame::getInstance();
	InGameState* igs = dg->stateInGame;

	// fall down
}

void Tile::update(GameContainer* container, GameTimer* timer) {
	Object::update(container, timer);


	DefaultGame* dg = DefaultGame::getInstance();
	InGameState* igs = dg->stateInGame;

//	if (m_introTimer > 0.0f && igs->m_waveIntroTimer == 0.0f) {
//		m_introTimer += timer->getDelta();
//		if (m_introTimer >= m_introDuration) {
//			m_introTimer = 0.0f;
//		}
//	}
}

void Tile::collision(Object* other) {

}

bool Tile::isEdgeTile() {
	InGameState* igs = DefaultGame::getInstance()->stateInGame;
    return false;
    //return gx == 0 || gy == 0 || gx == igs->worldWidth-1 || gy == igs->worldHeight-1;
}

void Tile::removeSolid() {
	if (itemonit) { return; }
	solid = false;

	DefaultGame* dg = DefaultGame::getInstance();
    InGameState* igs = dg->stateInGame;

	MyParticle* part = NULL;
	
	part = igs->m_particles->get();
	part->reset();
	part->m_animation->addFrame(MyParticle::s_cloud55);
	part->m_color = s_floorColor;
	part->m_duration = MathUtil::randBetweenf(1.0f, 2.0f);
	part->m_type = MyParticle::TYPE_SMOKE;
	part->m_bounds->setLocationByCenter(m_bounds->getCenterX(), m_bounds->getCenterY());
	part->m_velocity->set(0.0f, 0.0f);
	MathUtil::moveAngle<float>(part->m_velocity, MathUtil::randBetweenf(-135.0f, -45.0f), MathUtil::randBetweenf(10.0f, 30.0f));

	part = igs->m_particles->get();
	part->reset();
	part->m_animation->addFrame(MyParticle::s_cloud33);
	part->m_color = s_floorColor;
	part->m_duration = MathUtil::randBetweenf(1.0f, 2.0f);
	part->m_type = MyParticle::TYPE_SMOKE;
	part->m_bounds->setLocationByCenter(m_bounds->getCenterX() - 3, m_bounds->getCenterY() - 3);
	part->m_velocity->set(0.0f, 0.0f);
	MathUtil::moveAngle<float>(part->m_velocity, MathUtil::randBetweenf(-135.0f, -45.0f), MathUtil::randBetweenf(10.0f, 30.0f));

	part = igs->m_particles->get();
	part->reset();
	part->m_animation->addFrame(MyParticle::s_cloud22);
	part->m_color = s_floorColor;
	part->m_duration = MathUtil::randBetweenf(1.0f, 2.0f);
	part->m_type = MyParticle::TYPE_SMOKE;
	part->m_bounds->setLocationByCenter(m_bounds->getCenterX() + 3, m_bounds->getCenterY() + 3 );
	part->m_velocity->set(0.0f, 0.0f);
	MathUtil::moveAngle<float>(part->m_velocity, MathUtil::randBetweenf(-135.0f, -45.0f), MathUtil::randBetweenf(10.0f, 30.0f));
	
}

void Tile::render(GameContainer* container, Renderer* r)
{
	InGameState* igs = DefaultGame::getInstance()->stateInGame;
	r->setDrawColor(Color::white);
		
	/*bool flipX = false;
	bool flipY = false;
	
	Image* img = s_tileEmpty;

    bool isLeftEdge = false;//(gx == 0 || (gx > 0 && !igs->getTileAt(gx-1, gy)->solid));
    bool isRightEdge = false;//(gx == igs->worldWidth-1 || ((gx < igs->worldWidth-1 && !igs->getTileAt(gx+1, gy)->solid)));
    bool isTopEdge = false;//(gy == 0 || (gy > 0 && !igs->getTileAt(gx, gy-1)->solid));
    bool isBottomEdge = false;//(gy == igs->worldHeight-1 || (gy <= igs->worldHeight-1 && !igs->getTileAt(gx,gy+1)->solid));

	if ( isLeftEdge ) {
		if (isTopEdge) {
			img = s_tileTopLeft;
		} else if (isBottomEdge) {
			img = s_tileBottomLeft;
		} else {
			img = s_tileLeft;
		}
	} else if ( isRightEdge ) {
		if (isTopEdge) {
			img = s_tileTopLeft;
			flipX = true;
		} else if (isBottomEdge) {
			img = s_tileBottomLeft;
			flipX = true;
		} else {
			img = s_tileLeft;
			flipX = true;
		}
	} else if (isTopEdge) {
		img = s_tileTop;
	} else if (isBottomEdge) {
		img = s_tileBottom;
	} else {
		img = s_tileMiddle;
	}
	if (solid) { 
		img->setFlipped(flipX, flipY);
		img->drawCenteredScaled(m_bounds->getCenterX(), m_bounds->getCenterY(), 1.01f, 1.01f);
	}

	if (advance) {
		float sc = 1.01f;
		if (m_introTimer > 0.0f) {
			sc = Easing::easebetween(Easing::QUADRATIC_OUT, m_introTimer, 0.0f, 1.0f, m_introDuration);
		}
		s_tileAdvance->drawCenteredScaled(m_bounds->getCenterX(), m_bounds->getCenterY(), sc, sc);
	}*/

	Object::render(container, r);

	//r->setDrawColor("#007029", 1.0f);
	//r->drawRect(m_bounds->getMinX(), m_bounds->getMinY(), m_bounds->getWidth(), m_bounds->getHeight());

	//DefaultGame* dg = DefaultGame::getInstance();
	//dg->playerImg->drawCentered(m_bounds->getCenterX(), m_bounds->getCenterY());
}

Tile::~Tile() {

}
