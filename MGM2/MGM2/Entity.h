#include "Combat.h"

class Entity
{
public:
    Vec2 pos;
    vector<StatusEffect> inflictions;
    vector<int> inflictionAttackers;
    int health;
    int maxHealth;
    int activeAttack;
    vector<Attack> attacks;
    string name;
    float leech;
    bool summoned;
    bool ally;
    Term::RGB color;

    Entity(Term::RGB color, int health, int maxHealth, vector<Attack> attacks, string name, float leech = 0.0f) :
        inflictions(vector<StatusEffect>()), inflictionAttackers(vector<int>()),
        color(color), health(health), maxHealth(maxHealth), activeAttack(0),
        attacks(attacks), name(name), leech(leech), summoned(false), ally(false)
    { }

    Entity() : Entity({ 255, 255, 255 }, 0, 0, {}, "", 0.0f) { }

    Attack* CurrentAttack()
    {
        return &attacks[activeAttack];
    }

    void FindNewAttack()
    {
        CurrentAttack()->timeSinceStart = 0;
        activeAttack = RandRange(0, static_cast<int>(attacks.size()) - 1);
        CurrentAttack()->timeSinceStart = 1;
    }

    int FindDamageReduction()
    {
        int damageReduction = 0;
        for (int i = 0; i < inflictions.size(); i++)
            damageReduction += inflictions[i].Reduction();
        return damageReduction;
    }

    virtual void Update(Game* game, float deltaTime)
    {

    }

    virtual void DUpdate(Game* game, float deltaTime)
    {
        game->Draw(game->ToScreenSpace(pos), '+', color);
    }

    TurnReturn TakeTurn(int currentIndex)
    {
        vector<Entity*> newSummons = vector<Entity*>();

        Attack* currentAttack = CurrentAttack();

        if (currentAttack->length <= currentAttack->timeSinceStart)
        {
            AttackHit attackHit = currentAttack->RollDamage(currentIndex, FindDamageReduction());
            newSummons = currentAttack->summons;

            ApplyHit(attackHit.selfHit);

            FindNewAttack();

            return TurnReturn(attackHit.hit, newSummons);
        }
        else
            currentAttack->timeSinceStart++;
        return TurnReturn(Hit(0, {}, 0), {});
    }

    void ApplyHit(Hit hit)
    {
        health -= hit.damage;

        inflictions.reserve(inflictions.size() + hit.inflictions.size());
        inflictions.insert(inflictions.end(), hit.inflictions.begin(), hit.inflictions.end());

        inflictionAttackers.reserve(inflictionAttackers.size() + hit.inflictions.size());
        inflictionAttackers.insert(inflictionAttackers.end(), hit.inflictions.size(), hit.attacker);
    }

    InflictionResults UpdateInflictions()
    {
        int destroyedThisFrame = 0;

        vector<int> orinalInflictionAttackers(inflictionAttackers);

        vector<int> damageFromSources(inflictionAttackers.size());

        for (int i = 0; i < orinalInflictionAttackers.size(); i++)
        {
            int damage = inflictions[static_cast<size_t>(i) - destroyedThisFrame].Update();
            health -= damage;
            damageFromSources[i] += damage;

            if (inflictions[static_cast<size_t>(i) - destroyedThisFrame].shouldBeDestroyed)
            {
                inflictions.erase(i - destroyedThisFrame + inflictions.begin());
                inflictionAttackers.erase(i - destroyedThisFrame + inflictionAttackers.begin());
                destroyedThisFrame++;
            }
        }
        return InflictionResults(orinalInflictionAttackers, damageFromSources);
    }

    bool IsStunned()
    {
        for (int i = 0; i < inflictions.size(); i++)
            if (inflictions[i].effect.effect == STUN)
                return true;
        return false;
    }
};