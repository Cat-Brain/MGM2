#include "Infliction.h"



class Hit
{
public:
    int damage;
    vector<StatusEffect> inflictions;
    int attacker;

    Hit(int damage, vector<StatusEffect> inflictions, int attacker) :
        damage(damage), inflictions(inflictions), attacker(attacker)
    {}

    Hit() = default;
};



class AttackHit
{
public:
    Hit hit, selfHit;
    int unmodifiedDamage, unmodifiedSelfDamage;

    AttackHit(Hit hit, int unmodifiedDamage, Hit selfHit, int unmodifiedSelfDamage) :
        hit(hit), selfHit(selfHit), unmodifiedDamage(unmodifiedDamage), unmodifiedSelfDamage(unmodifiedSelfDamage)
    {}

    AttackHit() = default;
};



class Entity;
class Attack
{
public:
    vector<StatusEffect> procs;
    vector<int> procChances; // Percent
    int damage;
    int damageRand;
    vector<StatusEffect> selfProcs;
    vector<int> selfProcChances; // Percent
    int selfDamage;
    int selfDamageRand;
    int length;
    int timeSinceStart;
    string name;
    vector<Entity*> summons;

    Attack(vector<StatusEffect> procs, vector<int> procChances, int damage, int damageRand,
        vector<StatusEffect> selfProcs, vector<int> selfProcChances, int selfDamage, int selfDamageRand,
        vector<Entity*> summons, int length, string name) :
        procs(procs), procChances(procChances), damage(damage), damageRand(damageRand), selfProcs(selfProcs),
        selfProcChances(selfProcChances), selfDamage(selfDamage), selfDamageRand(selfDamageRand), summons(summons),
        length(length), name(name), timeSinceStart(0)
    { }

    Attack() = default;

    AttackHit RollDamage(int currentIndex, int damageReduction)
    {
        vector<StatusEffect> inflictions = vector<StatusEffect>();
        for (int i = 0; i < procs.size(); i++)
        {
            if (rand() % 100 + 1 <= procChances[i])
            {
                inflictions.push_back(procs[i]);
            }
        }
        vector<StatusEffect> selfInflictions = vector<StatusEffect>();
        for (int i = 0; i < selfProcs.size(); i++)
        {
            if (rand() % 100 + 1 <= selfProcChances[i])
            {
                selfInflictions.push_back(selfProcs[i]);
            }
        }

        int unModifiedDamage = RandRange(damage - damageRand, damage + damageRand);
        int unModifiedSelfDamage = RandRange(selfDamage - selfDamageRand, selfDamage + selfDamageRand);
        return AttackHit(Hit(unModifiedDamage < 0 ? unModifiedDamage - damageReduction : std::max(0, unModifiedDamage - damageReduction),
            inflictions, currentIndex), unModifiedDamage,
            Hit(unModifiedSelfDamage < 0 ? unModifiedSelfDamage + damageReduction : std::max(0, unModifiedSelfDamage + damageReduction),
                selfInflictions, currentIndex), unModifiedSelfDamage);
    }
};



class TurnReturn
{
public:
    Hit hit;
    vector<Entity*> summons;

    TurnReturn(Hit hit, vector<Entity*> summons) :
        hit(hit), summons(summons)
    { }

    TurnReturn() = default;
};



class InflictionResults
{
public:
    vector<int> originalAttackers;
    vector<int> damageFromSources;

    InflictionResults(vector<int> originalAttackers, vector<int> damageFromSources) :
        originalAttackers(originalAttackers), damageFromSources(damageFromSources)
    { }

    InflictionResults() = default;
};



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

    Entity(int health, int maxHealth, vector<Attack> attacks, string name, float leech) :
        inflictions(vector<StatusEffect>()), inflictionAttackers(vector<int>()),
        health(health), maxHealth(maxHealth), activeAttack(0),
        attacks(attacks), name(name), leech(leech), summoned(false), ally(false)
    { }

    Entity() : Entity(0, 0, {}, "", 0.0f) { }

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

    virtual void Update(Game* game)
    {

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



class Player : public Entity
{
public:
    using Entity::Entity;

    void Update(Game* game) override
    {
        pos.x += int(game->input.d.held) - int(game->input.a.held);
        pos.y += int(game->input.w.held) - int(game->input.s.held);

        game->playerPos = pos;
    }
};