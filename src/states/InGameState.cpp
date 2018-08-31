
#include "InGameState.h"
#include "../DefaultGame.h"

#include "../objects/Player.h"
#include "../objects/Enemy.h"
#include "../objects/Tile.h"
#include "../objects/Trap.h"
#include "../objects/MyParticle.h"
#include "../objects/Collectable.h"
#include "../objects/Decoration.h"

#include <ARK2D/Util/CameraShake.h>
#include <ARK2D/Core/+.h>
#include <ARK2D/Tiled/+.h>
#include <ARK2D/UI/+.h>


InGameState::InGameState():
	GameState(),
	m_playerStart()
	{

}

unsigned int InGameState::id() {
	return StatesList::STATE_INGAME;
}

void InGameState::enter(GameContainer* container, StateBasedGame* game, GameState* from) {

}

void InGameState::leave(GameContainer* container, StateBasedGame* game, GameState* to) {

}

void InGameState::init(GameContainer* container, StateBasedGame* game) {
	m_players = new Pool<Player*>(8);
	m_enemies = new Pool<Enemy*>(20);
    m_tiles = new Pool<Tile*>(60);
    m_traps = new Pool<Trap*>(20);
    m_collectables = new Pool<Collectable*>(20);
    m_decorations = new Pool<Decoration*>(20);
    //m_bombs = new Pool<Bomb*>(5);
    //m_arrowBoxes = new Pool<ArrowBox*>(5);
    //m_arrows = new Pool<Arrow*>(5);
    //m_buzzsaws = new Pool<Buzzsaw*>(6);
    
    m_particles = new Pool<MyParticle*>(60);


	m_cameraShake = new CameraShake(game, 4);

	m_levelNumber = 0;

	nextLevel();

	

	start();
}

void InGameState::nextLevel() {
	m_levelNumber++;

	string mapFile = StringUtil::append("level-", m_levelNumber) + string(".tmx");
	string nextMapFile = StringUtil::append("level-", m_levelNumber+1) + string(".tmx");
    
    if (!StringUtil::file_exists(mapFile.c_str())) {
    	//ErrorDialog::createAndShow("YOU WIN!");
    	DefaultGame* dg = DefaultGame::getInstance();
    	dg->enterState(dg->stateSummary, NULL, NULL);

    	return;
    }

    m_lastLevel = false;
    if (!StringUtil::file_exists(nextMapFile.c_str())) {
    	m_lastLevel = true;
    }
    
	m_map = TiledMap::createFromFile(mapFile);
}


void InGameState::reset() {
	
	m_levelNumber = 0;
	m_levelStartTimer = 0.0f;
	m_levelStartDuration = 0.5f;;

	m_players->reset();
	
	
	m_particles->reset();
	

}

void InGameState::start() {
	reset();
	nextLevel();

	m_player = m_players->get();
    
    m_playerStart = new Vector2<float>(0.0f, 0.0f);

    reloadObjects();
	
}
void InGameState::reloadObjects() {
	m_died = false;

	m_levelStartTimer = 0.01f;

	m_levelRotation = -2.0f + MathUtil::randBetweenf(0.0f, 4.0f);
	m_levelRotationTimer = 0.0f;
	m_levelRotationDuration = 5.0f;

	
	m_enemies->reset();
	m_tiles->reset();
	m_traps->reset();
	m_collectables->reset();
	m_decorations->reset();

	TiledMapObjectGroup* objlayer = m_map->getObjectGroupByName("objects");
	for(int i = 0; i < objlayer->getObjects()->size(); i++) {
		TiledMapObject* obj = objlayer->getObjects()->at(i);
		int gid = obj->getGID();
		string typeName = m_map->getTileByGID(gid)->getPropertyByName("type")->getValue();
		ARK2D::getLog()->e(StringUtil::append("typename: ", typeName));
		if (typeName == "player") {
			m_playerStart->set(obj->getX(), obj->getY());
		}
		else if (typeName == "spike") {
			Trap* trap = m_traps->get();
			trap->reset();
			trap->m_type = Trap::TYPE_SPIKE;
			trap->m_bounds->asRectangle()->setSize(3, 4);
			trap->m_bounds->setLocation( obj->getX(), obj->getY() );
			trap->m_animation->addFrame(Trap::s_spike);
		}
		else if (typeName == "blade") {
			Trap* trap = m_traps->get();
			trap->reset();
			trap->m_type = Trap::TYPE_BLADE;
			trap->m_bounds->asRectangle()->setSize(2, 2);
			trap->m_bounds->setLocationByCenter( obj->getX(), obj->getY() );
			trap->m_animation->addFrame(Trap::s_blade);
			trap->m_bladeRotation = Cast::fromString<float>(m_map->getTileByGID(gid)->getPropertyByName("angle")->getValue());
		}
		else if (typeName == "enemy") {
		 	Enemy* e = m_enemies->get();
		 	e->reset();
		 	e->m_type = Enemy::TYPE_NORMAL;
		 	e->m_bounds->setLocationByCenter(obj->getX(), obj->getY() );
		}
		else if (typeName == "collectable") {
			Collectable* c = m_collectables->get();
			c->reset();
			c->m_type = Collectable::TYPE_DOLLAR;
			c->m_bounds->setLocationByCenter( obj->getX(), obj->getY() );
			c->m_animation->addFrame(Collectable::s_dollar);
		}
		else if (typeName == "briefcase") {
			Collectable* c = m_collectables->get();
			c->reset();
			c->m_type = Collectable::TYPE_GOAL;
			c->m_visible = false;
			c->m_bounds->setLocationByCenter( obj->getX(), obj->getY() );
			c->m_animation->addFrame(Collectable::s_briefcase);
		}
		else if (typeName == "decoration") {
			Decoration* c = m_decorations->get();
			c->reset();

			TiledMapTile* t = m_map->getTileByGID(gid);

			c->m_bounds->setLocationByCenter( obj->getX(), obj->getY() );
			c->m_animation->addFrame(  t->getImage() );
		}
	}
	
    
    TiledMapLayer* layer = m_map->getLayerByName("solid");
    for(int x = 0; x < layer->getWidth(); x++) {
        for(int y = 0; y < layer->getHeight(); y++) {
			int gid = layer->getTileGID(x, y);
			if (gid != 0) {
				Tile* tile = m_tiles->get();
				tile->reset();
				tile->m_bounds->setLocation(x*4, y*4);
			}
		}
	}

	m_player->m_bounds->setLocation(m_playerStart->getX(), m_playerStart->getY());
}

void InGameState::spawnBlood(float x, float y, float radius, int num) {
	MyParticle* part = NULL;

	for(int i = 0; i < num; i++) {
		float splashStrength = MathUtil::randBetweenf(10.0f, 30.0f); 
		float tx = x;
		float ty = y;
		MathUtil::moveAngle<float>(tx, ty, MathUtil::randBetweenf(0,360), MathUtil::randBetweenf(0.0f, radius));

		Image* frame = MyParticle::s_bloodSelection->at(MathUtil::randBetween(0, MyParticle::s_bloodSelection->size()));
		part = m_particles->get();
		part->reset();
		part->m_animation->reset();
		part->m_animation->addFrame(frame);
		part->m_type = MyParticle::TYPE_BLOOD;
		part->m_bounds->setLocationByCenter(tx, ty);
		MathUtil::moveAngle<float>(part->m_velocity, MathUtil::randBetweenf(0,360), splashStrength);
	}
}


void InGameState::update(GameContainer* container, StateBasedGame* game, GameTimer* timer) {
	DefaultGame* dg = DefaultGame::getInstance();
	Input* in = ARK2D::getInput();

	if (in->isKeyPressed(Input::KEY_ENTER) || 
		in->isGamepadButtonPressed(Gamepad::BUTTON_START)) {
		dg->m_sfxSelect->play();
		//m_paused = !m_paused;
	}
	//if (m_paused) { return; }

	if (m_levelStartTimer > 0.0f) {
		m_levelStartTimer += timer->getDelta();
		if (m_levelStartTimer >= m_levelStartDuration) {
			m_levelStartTimer = 0.0f;
		}
	}

	m_levelRotationTimer += timer->getDelta();
	if (m_levelRotationTimer >= m_levelRotationDuration) {
		m_levelRotationTimer -= m_levelRotationDuration;
	}


	if (in->isKeyPressed(Input::KEY_ESCAPE) ||
		in->isKeyPressed(Input::KEY_BACKSPACE) ||
		in->isGamepadButtonPressed(Gamepad::BUTTON_BACK)) {
		game->enterState(dg->stateMenu);
	}

	if (in->isKeyPressed(Input::KEY_R)) {
		m_player->die();
	}

	if (m_deadTimer > 0.0f) {
		m_deadTimer += timer->getDelta();
		if (m_deadTimer >= m_deadDuration) {
			m_deadTimer = 0.0f;
			//m_player->respawn();
		}
		return;
	}

	m_cameraShake->update(container, timer);


	m_player->update(container, timer);
	m_tiles->updateAll(container, timer);
	//m_buzzsaws->updateAll(container, timer);
	m_collectables->updateAll(container, timer);
	m_decorations->updateAll(container, timer);
	m_traps->updateAll(container, timer);
	m_enemies->updateAll(container, timer);
	m_particles->updateAll(container, timer);
	//m_flowers->updateAll(container, timer);
	//m_bombs->updateAll(container, timer);
	//m_arrowBoxes->updateAll(container, timer);
	//m_arrows->updateAll(container, timer);

	m_collectables->pruneAll();
	m_decorations->pruneAll();
	m_enemies->pruneAll();
	m_particles->pruneAll();
	//m_bombs->pruneAll();

	doCollisions();


	if (m_died)  {
		if (m_lastLevel) {
			dg->m_sfxEnd->play();
			dg->m_music->setVolume(0.0f);
            Color* c = new Color(1.0f,1.0f,1.0f,1.0f);
            dg->enterState(dg->stateSummary, new FadeToColourTransition(2.0f, c), new FadeFromColourTransition(2.0f, c));
		} else {
			reloadObjects();
		}
	}
}

void playerCollisionWithTile(Player* p, Object* otherObj, const Vector2<float>& vec) {

	//m_player->m_bounds->asRectangle()->setX(m_player->m_bounds->getMinX() + vec.m_x);
	//m_player->m_bounds->asRectangle()->setY(m_player->m_bounds->getMinY() + vec.m_y);
	if (vec.m_x < 0) {
		p->m_bounds->asRectangle()->setX(p->m_bounds->getMinX() + vec.m_x);
		//if (!m_grounded) {
			p->m_velocity->m_x = 0;
		//}
	} else if (vec.m_x > 0) {
		// push to the right? aka die.
		//die();
		//if (!m_grounded) {
			p->m_velocity->m_x = 0;
		//}
		p->m_bounds->asRectangle()->setX(p->m_bounds->getMinX() + vec.m_x);

		//if still colliding then trapped!
	}
	if (vec.m_y < 0) {
		p->m_bounds->asRectangle()->setY(p->m_bounds->getMinY() + vec.m_y);
		p->m_bounds->asRectangle()->setY(round(p->m_bounds->getMinY()));
		p->m_velocity->setY(0.0f);
		//groundedThisUpdate = true;
		p->m_grounded = true;
		p->m_jumpRotation = 0.0f;

		//DisappearingTile* dt = NULL;
		//dt = dynamic_cast<DisappearingTile*>(otherObj);
		//if (dt != NULL) {
		//	dt->m_startedDisappearing = true;
		//}
		//if (b->fallingBlock && !b->fallingBlockIsCollided) {
		//	b->fallingBlockIsCollided = true;
		//}
	}
	if (vec.m_y > 0) {
		p->m_velocity->setY(0.0f);
		p->m_bounds->asRectangle()->setY(p->m_bounds->getMinY() + vec.m_y);
	}
	p->onBoundsMoved();
}
void enemyCollisionWithTile(Enemy* p, Object* otherObj, const Vector2<float>& vec) {

	//m_player->m_bounds->asRectangle()->setX(m_player->m_bounds->getMinX() + vec.m_x);
	//m_player->m_bounds->asRectangle()->setY(m_player->m_bounds->getMinY() + vec.m_y);
	if (vec.m_x < 0) {
		p->m_bounds->asRectangle()->setX(p->m_bounds->getMinX() + vec.m_x - 1);
		//if (!m_grounded) {
			p->m_velocity->m_x = 0;
			p->m_facingLeft = !p->m_facingLeft;
		//}
	} else if (vec.m_x > 0) {
		// push to the right? aka die.
		//die();
		//if (!m_grounded) {
			p->m_velocity->m_x = 0;
		//}
		p->m_bounds->asRectangle()->setX(p->m_bounds->getMinX() + vec.m_x+1);
		p->m_facingLeft = !p->m_facingLeft;

		//if still colliding then trapped!
	}
	if (vec.m_y < 0) {
		p->m_bounds->asRectangle()->setY(p->m_bounds->getMinY() + vec.m_y);
		p->m_bounds->asRectangle()->setY(round(p->m_bounds->getMinY()));
		p->m_velocity->setY(0.0f);
		//groundedThisUpdate = true;
		p->m_grounded = true;
		//p->m_jumpRotation = 0.0f;

		//DisappearingTile* dt = NULL;
		//dt = dynamic_cast<DisappearingTile*>(otherObj);
		//if (dt != NULL) {
		//	dt->m_startedDisappearing = true;
		//}
		//if (b->fallingBlock && !b->fallingBlockIsCollided) {
		//	b->fallingBlockIsCollided = true;
		//}
	}
	if (vec.m_y > 0) {
		p->m_velocity->setY(0.0f);
		p->m_bounds->asRectangle()->setY(p->m_bounds->getMinY() + vec.m_y);
	}
	p->onBoundsMoved();
}

void InGameState::doCollisions() {
	m_player->m_grounded = false;

	PoolIterator<Trap*> it_traps = m_traps->newiteratorref();
	while (it_traps.hasNext()) {
		Trap* t = it_traps.next();
		if (t->m_bounds->collides(m_player->mainBounds())) {
			m_player->die();
		}

		if (t->m_type == Trap::TYPE_BLADE) {
			float bladeEndX = t->m_bounds->getCenterX();
			float bladeEndY = t->m_bounds->getCenterY();
			MathUtil::moveAngle<float>(bladeEndX, bladeEndY, t->m_bladeRotation, 12);
			if (Shape<float>::collision_rectangleLine(
				m_player->mainBounds()->getMinX(),
				m_player->mainBounds()->getMinY(),
				m_player->mainBounds()->getWidth(),
				m_player->mainBounds()->getHeight(),
				t->m_bounds->getCenterX(), t->m_bounds->getCenterY(), 
				bladeEndX, bladeEndY)
				) {
				m_player->die();
			}
		}

	}

	PoolIterator<Enemy*> it_enemies = m_enemies->newiteratorref();
	while (it_enemies.hasNext()) {
		Enemy* t = it_enemies.next();
		if (t->m_bounds->collides(m_player->mainBounds())) {
			m_player->die();
		}
	}



	PoolIterator<Tile*>* it_tiles = m_tiles->iterator();
	while (it_tiles->hasNext())
	{
		Tile* t = it_tiles->next();
		if (!t->solid) { continue; }
		if (t->m_bounds->collides(m_player->mainBounds()))
		{
			const Vector2<float>& vec = Shape<float>::collision_rectangleRectangleVec(
				m_player->mainBounds()->getMinX(), m_player->mainBounds()->getMinY(), m_player->mainBounds()->getWidth(), m_player->mainBounds()->getHeight(),
				t->m_bounds->getMinX(), t->m_bounds->getMinY(), t->m_bounds->getWidth(), t->m_bounds->getHeight()
			);
			playerCollisionWithTile(m_player, t, vec);
		}
        
        PoolIterator<Enemy*> it_enemies = m_enemies->newiteratorref();
        while (it_enemies.hasNext()) {
            Enemy* e = it_enemies.next();
            if (t->m_bounds->collides(e->m_bounds))
            {
                const Vector2<float>& vec = Shape<float>::collision_rectangleRectangleVec(
                  e->m_bounds->getMinX(), e->m_bounds->getMinY(), e->m_bounds->getWidth(), e->m_bounds->getHeight(),
                  t->m_bounds->getMinX(), t->m_bounds->getMinY(), t->m_bounds->getWidth(), t->m_bounds->getHeight()
                  );
                enemyCollisionWithTile(e, t, vec);
            }
        }
    }




    PoolIterator<Collectable*> it_collectables = m_collectables->newiteratorref();
    while (it_collectables.hasNext()) {
    	Collectable* c = it_collectables.next();
    	if (c->m_bounds->collides(m_player->m_bounds) && c->m_visible) {
    		c->collected();
    	}
    }

    if (m_collectables->sizeActive() == 1 && m_collectables->getActive()->get(0)->m_type == Collectable::TYPE_GOAL) {
    	m_collectables->getActive()->get(0)->m_visible = true;
    }
    else if (m_collectables->sizeActive() == 0 || ARK2D::getInput()->isKeyPressed(Input::KEY_P)) {// next level
        DefaultGame* dg = DefaultGame::getInstance();
        dg->m_sfxNextWave->play();

    	nextLevel();
    	reloadObjects();
    }
    if (ARK2D::getInput()->isKeyPressed(Input::KEY_O)) {
    	m_levelNumber -= 2;

    	nextLevel();
    	reloadObjects();
    }
}

void InGameState::render(GameContainer* container, StateBasedGame* game, Renderer* r) {
	r->setDrawColor(Color::white);
	//r->drawString("In Game State", 20, 20);

	r->pushMatrix();
	r->translate(m_cameraShake->getXOffset(), m_cameraShake->getYOffset());

	r->translate(
		(container->getWidth() - float(m_map->getWidth()))*0.5f, 
		(container->getHeight() - float(m_map->getHeight()))*0.5f
	);

	float scaleVal = float(container->getWidth() - 16) / float(m_map->getWidth());
	if (m_map->getHeight() * scaleVal >= container->getHeight() - 16) {
		scaleVal = float(container->getHeight() - 16) / float(m_map->getHeight());
	}

	float extraRot = 0.0f;
    extraRot = DefaultGame::getInstance()->juiceRotation(0.5f, m_levelRotationTimer, m_levelRotationDuration);

    r->translate(m_map->getWidth()*0.5f, m_map->getHeight()*0.5f);
    r->scale(scaleVal, scaleVal);
    r->rotate(m_levelRotation + extraRot);
    r->translate(m_map->getWidth()*-0.5f, m_map->getHeight()*-0.5f);



	m_map->draw(0, 0);
	m_tiles->renderAll(container, r);
	m_collectables->renderAll(container, r);
	m_decorations->renderAll(container, r);
	m_player->render(container, r);
	m_traps->renderAll(container, r);
	m_enemies->renderAll(container, r);
	m_particles->renderAll(container, r);

	r->popMatrix();

	float alpha = 0.0f; 
	if (m_levelStartTimer > 0.0f) {
		alpha = Easing::easebetween(Easing::QUADRATIC_OUT, m_levelStartTimer, 1.0f, 0.0f, m_levelStartDuration);
	}
	r->setDrawColorf(1.0f, 1.0f, 1.0f, alpha);
	r->fillRect(0.0f, 0.0f, container->getWidth(), container->getHeight());

	
}

InGameState::~InGameState() {

}

