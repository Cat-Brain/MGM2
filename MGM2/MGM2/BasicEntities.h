#include "Entity.h"

class FunctionalEntity : public Entity
{
public:
	float lastTime, timePer;

	FunctionalEntity(float timePer, Term::RGB color, int health, int maxHealth, vector<Attack> attacks, string name, float leech) :
		Entity(color, health, maxHealth, attacks, name, leech), lastTime(tTime), timePer(timePer) { }

	virtual bool TUpdate(Game* game, float deltaTime, float bigDeltaTime) { return true; }

	void Update(Game* game, float deltaTime) override
	{
		if (tTime - lastTime > timePer)
			if(TUpdate(game, deltaTime, tTime - lastTime)) lastTime = tTime;
	}
};

class EnemyBase : public FunctionalEntity
{
public:
	EnemyBase(float timePer, Term::RGB color, int health, int maxHealth, vector<Attack> attacks, string name, float leech) :
		FunctionalEntity(timePer, color, health, maxHealth, attacks, name, leech) { }

	bool TUpdate(Game* game, float deltaTime, float bigDeltaTime) override
	{
		if (pos.Squistance(game->playerPos) > 1)
		{
			pos += pos.DirTo(game->playerPos);
			return true;
		}
		return false;
	}
};