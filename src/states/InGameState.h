
#ifndef ARK2D_DEFAULTGAME_INGAMESTATE_H_
#define ARK2D_DEFAULTGAME_INGAMESTATE_H_

#include <ARK2D/Core/State/GameState.h>

class DefaultGame;

class Object;
class BlankObject;
class Player;
class Pickup;
class Explosion;
class Enemy;
class Enemy;
class Tile;
class Trap;
class Checkpoint;
class MyParticle;
class Collectable;
class Decoration;

class InGameState : public GameState {
	public:

		Player* m_player;
		Pool<Player*>* m_players;

		Pool<Enemy*>* m_enemies;
		Pool<Tile*>* m_tiles;
		Pool<Trap*>* m_traps;
		Pool<Collectable*>* m_collectables;
		Pool<Decoration*>* m_decorations;
		//Pool<Buzzsaw*>* m_buzzsaws;
		Pool<Checkpoint*>* m_checkpoint;
		Pool<MyParticle*>* m_particles;

		Pool<Object*>* m_objects;

		CameraShake* m_cameraShake;
		TiledMap* m_map;

		float m_deadTimer;
		float m_deadDuration;

		Vector2<float>* m_playerStart;

		bool m_paused;

		bool m_died;

		int m_levelNumber;
		float m_levelStartTimer;
		float m_levelStartDuration;

		float m_levelRotation;
		float m_levelRotationTimer;
		float m_levelRotationDuration;
		bool m_lastLevel;
		
		InGameState();
		void enter(GameContainer* container, StateBasedGame* game, GameState* from);
		void leave(GameContainer* container, StateBasedGame* game, GameState* to);

		unsigned int id();
		void reset();
		void start();
		void nextLevel();
		void reloadObjects();
		void spawnBlood(float x, float y, float radius, int num);
		void init(GameContainer* container, StateBasedGame* game);
		void update(GameContainer* container, StateBasedGame* game, GameTimer* timer);
		void doCollisions();
		void render(GameContainer* container, StateBasedGame* game, Renderer* r);

		virtual ~InGameState();
};

#endif /* ARK2D_DEFAULTGAME_INGAMESTATE_H_ */
