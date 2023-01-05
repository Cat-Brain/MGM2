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