// Go to https ://github.com/Matthew-12525/Create-Your-Own-Adventure for the unmodded experience. =]
/*
Hiya, this 'mod' was written by Jordan Baumann(me),
however this all wouldn't of existed without Matthew's original version of the game(link above^^^).
I hope that you enjoy this experience, and if you want, maybe you'll even mod this mod! =]
*/


#include "Combat.h"


#pragma region Variables

bool restart = true;
bool specialFightEnding = false;
vector<Entity*> specialFightEndingMonsters(0);
Settings currentSettings;

//Constant variables:
//Attacks
//The syntax for a status effect is :
//StatusEffect(InflictionType.YOURINFLICTION, how long you want it to last)
//The syntax for an attacks is :
//Attack([Status effects], [chance of each status effect happening], damage, damage randomness(how far from the original value the actual value can be), [self inflictions], [self infliction procs], self damage, self damage randomness, [summons], turns to do, name)
//First up is the attacks that are required to be early.
Attack fireBreath{ { {BURNING, 2} }, { 100 }, 0, 0, {}, {}, 0, 0, {}, 3, "fire breath" };
Attack heavyBite{ {}, {}, 50, 0, {}, {}, 0, 0, {}, 4, "heavy bite" };
//Enemies that must be declared early.
Entity joshroHead{ 25, 50, {fireBreath, heavyBite}, "Joshro Head", 0.5f };
//Normal attacks.
Attack clubBash{ { {STUN, 2} }, { 100 }, 25, 10, {}, {}, 0, 0, {}, 3, "club bash" };
Attack punch{ {}, {}, 15, 15, {}, {}, 0, 0, {}, 1, "punch" };
Attack heavyPunch{ { {STUN, 2} }, { 75 }, 25, 25, {}, {}, 0, 0, {}, 2, "heavy punch" };
Attack quickStab{ { {POISON, 3} }, { 50 }, 5, 5, {}, {}, 0, 0, {}, 1, "quick stab" };
Attack rockThrow{ { {STUN, 1} }, { 25 }, 5, 5, {}, {}, 0, 0, {}, 1, "rock throw" };
Attack slimeHug{ { {DEADLY_HUG, 3} }, { 100 }, 0, 0, {}, {}, 0, 0, {}, 1, "slime hug" };
Attack slimeSpike{ { {BLEED, 3} }, { 100 }, 5, 0, {}, {}, 0, 0, {}, 1, "slime spike" };
Attack arrowShoot{ { {BURNING, 3}, {BURNING, 3}, {POISON, 8} }, { 50, 50, 100 }, 35, 10, {}, {}, 0, 0, {}, 3, "shoot arrow" };
Attack chokeHold{ { {STUN, 1} }, { 95 }, 5, 5, {}, {}, 0, 0, {}, 1, "choke hold" };
Attack deepCut{ { {BLEED, 15}, {BLEED, 15}, {BLEED, 15} }, { 100, 50, 25 }, 0, 0, {}, {}, 0, 0, {}, 1, "deep cut" };
Attack finisher{ {}, {}, 20, 20, {}, {}, 0, 0, {}, 1, "finisher" };
Attack heavyBlow{ {}, {}, 100, 0, {}, {}, 0, 0, {}, 5, "heavy blow" };
Attack quickAttack{ {}, {}, 35, 0, {}, {}, 0, 0, {}, 2, "quick attack" };
Attack heaviestBlow{ {}, {}, 125, 0, {}, {}, 0, 0, {}, 6, "heaviest blow" };
Attack splash{ { {WET, 5} }, { 100 }, 3, 3, {}, {}, 0, 0, {}, 1, "splash" };
Attack quickClubBash{ { {STUN, 2} }, { 75 }, 10, 10, {}, {}, 0, 0, {}, 2, "quick club bash" };
Attack bite{ { {POISON, 4}, {BLEED, 4} }, { 5, 5 }, 5, 5, {}, {}, 0, 0, {}, 2, "bite" };
Attack scratch{ { {BLEED, 4} }, { 25 }, 15, 5, {}, {}, 0, 0, {}, 1, "scratch" };
Attack spare{ {}, {}, 0, 0, {}, {}, 0, 0, {}, 1, "spare" };
Attack growHead{ {}, {}, 0, 0, {}, {}, 0, 0, { &joshroHead }, 4, "grow head" };
Attack ultraFireBreath{ { {BURNING, 3} }, { 100 }, 0, 0, {}, {}, 0, 0, {}, 1, "ultra fire breath" };
Attack strengthen{ { {STRENGTHEN, 2} }, {100}, 0, 0, { {STRENGTHEN, 4} }, {100}, 0, 0, {}, 1, "strengthen" };
Attack healify{ {}, {}, -25, 25, {}, {}, -50, 50, {}, 1, "healify" };

//The syntax for enemies is :
//Enemy(start health, max health, [attack1, attack2, ...], "name", leech amount 0 to 1 work best
Entity joshrosBody{ 300, 300, { growHead }, "Joshro's Body", 0.0 };
Entity ogre{ 100, 100, { clubBash, punch }, "Ogre", 0.0 };
Entity goblin{ 100, 100, { quickStab, rockThrow }, "Goblin", 0.0 };
Entity slime{ 25, 50, { slimeHug }, "Pet Slime", 1.0 };
Entity troll{ 125, 125, { quickClubBash, splash }, "troll", 0.0 };
Entity mutant{ 200, 200, { punch, heavyPunch }, "mutant", 0.0 };
Entity rat{ 100, 200, { bite, scratch }, "Rat", 0.25 };
Entity babyRat{ 25, 50, { bite, scratch, splash }, "Baby Rat", 0.5 };
Entity guard{ 200, 200, { heavyBlow, quickAttack }, "Unloyal Guard", 0.0 };

Weapon bow{ { arrowShoot, chokeHold }, "Bow", 0.0 };
Weapon axe{ { deepCut, finisher }, "Axe", 0.0 };
Weapon sword{ { heavyBlow, quickAttack }, "Sword", 0.0 };
Weapon ogreInABottle{ { clubBash, punch }, "Ogre in a Bottle", 0.5 };
Weapon python{ { heaviestBlow, quickAttack }, "Python", 0.5 };


Player player;

#pragma endregion





#pragma region Functions
void FindSettings()
{
    string prompt = Input("How many seconds do you want to wait after key events('default' = 1) ");
    int result = -1;
    if (IsNumber(prompt))
        result = stoi(prompt);
    else if (prompt == "default")
        result = 1;
    while (result == -1)
    {
        prompt = Input("It has to be a number or 'default'. How many seconds do you want to wait after key events('default' = 1) ");
        if (IsNumber(prompt))
            result = stoi(prompt);
        else if (prompt == "default")
            result = 1;
    }
    currentSettings = Settings(result);
}





void FightSequence(vector<Entity> enemyTypes, bool spareable, vector<vector<string>> specialEnding)
{
    specialFightEnding = false;
    bool fightOn = true;
    bool fightFrameOne = true;
    int target = 0;

    vector<Entity*> enemies = vector<Entity*>(enemyTypes.size());
    for (int i = 0; i < enemies.size(); i++)
        enemies[i] = new Entity(enemyTypes[i]);

    while (fightOn)
    {
        printf("");
        for (Entity* enemy : enemies)
            printf("%s's health: %i\n", enemy->name.c_str(), enemy->health);
        printf("Max's health: %i\n", player.health);

        if (fightFrameOne)
        {
            fightFrameOne = false;
            for (Entity* enemy : enemies)
                enemy->FindNewAttack();
            player.weapon.SwitchAttacks("Do you want to use ", false);
        }

        Sleep();
        printf("\n++++++++++++++++\n\n");

        bool spare = player.weapon.CurrentAttack()->name == "spare";
        string dialogue = "'dodge' or '" + string(spare? "spare" : "attack") + "' to continue your current attack or 'switch' your attack";
        if (enemies.size() >= 2)
            dialogue += " or 'change' targets from " + enemies[target]->name;
        dialogue += "? ";
        string prompt = Input(dialogue);
        while (prompt != "dodge" && ((prompt != "spare" && spare) || (prompt != "attack" && !spare)) && prompt != "switch" && (!(enemies.size() > 1 && prompt == "change")))
            prompt = Input("That won't work this time! " + dialogue);


        if (prompt == "dodge")
        {
            printf("\n");
            for (int i = 0; i < enemies.size(); i++)
            {
                if (!player.IsStunned())
                {
                    if (!enemies[i]->IsStunned())
                    {
                        int unblockedDamage = 0;
                        int blockedDamage = 0;
                        if (!enemies[i]->IsStunned())
                        {
                            TurnReturn tr = enemies[i]->TakeTurn(i);
                            for (int j = 0; j < tr.summons.size(); j++)
                            {
                                (tr.summons[j])->summoned = true;
                                printf("%s has birthed a new %s!\n", enemies[i]->name.c_str(), tr.summons[j]->name.c_str());
                                enemies.push_back(new Entity(*tr.summons[j]));
                            }

                            unblockedDamage += tr.hit.damage;
                            int damageDelt = tr.hit.damage / 2;
                            blockedDamage += damageDelt;
                            int heal = static_cast<int>(floorf(damageDelt * enemies[i]->leech));
                            if (heal != 0)
                            {
                                printf("%s heal's off of you for %i.\n", enemies[i]->name.c_str(), heal);
                                enemies[i]->health = std::min(enemies[i]->maxHealth, enemies[i]->health + heal);
                            }
                            player.ApplyHit(Hit(damageDelt, tr.hit.inflictions, i), true);
                            printf("You dodged the attack and took %i damage instead of taking %i damage!\n", blockedDamage, unblockedDamage);
                        }
                        else
                            printf("%s did not attack this round as they were stunned.\n", enemies[i]->name.c_str());
                    }
                    else
                    {
                        printf("%s did not attack this round as they were stunned.\n", enemies[i]->name.c_str());
                        printf("");
                    }
                }
                else
                {
                    if (!enemies[i]->IsStunned())
                    {
                        TurnReturn tr = enemies[i]->TakeTurn(i);
                        for (int j = 0; j < tr.summons.size(); j++)
                        {
                            (tr.summons[j])->summoned = true;
                            printf("%s has birthed a new %s!\n", enemies[i]->name.c_str(), tr.summons[j]->name.c_str());
                            enemies.push_back(new Entity(*tr.summons[j]));
                        }

                        player.ApplyHit(tr.hit, false);
                        int heal = static_cast<int>(floorf(tr.hit.damage * enemies[i]->leech));
                        if (heal != 0)
                        {
                            printf("%s heal's off of you for %i.\n", enemies[i]->name.c_str(), heal);
                            enemies[i]->health = std::min(enemies[i]->maxHealth, enemies[i]->health + heal);
                        }
                        printf("Becuase you were stunned you didn't block.\n");
                    }
                    else
                    {
                        printf("%s did not attack this round as they were stunned.\n", enemies[i]->name.c_str());
                        printf("");
                    }
                }
            }

            for (int i = 0; i < enemies.size(); i++)
            {
                InflictionResults ir = enemies[i]->UpdateInflictions();
                for (int j = 0; j < ir.damageFromSources.size(); j++)
                {
                    int heal = static_cast<int>(floorf(ir.damageFromSources[j] * player.weapon.leech));
                    if (heal != 0)
                    {
                        printf("You heal off of %s for %i because of %s.\n", enemies[i]->name.c_str(), heal, ir.names[j].c_str());
                        player.health = std::min(player.maxHealth, player.health + heal);
                    }
                }
            }

            InflictionResults ir = player.UpdateInflictions();
            for (int i = 0; i < ir.damageFromSources.size(); i++)
            {
                int heal = static_cast<int>(floorf(ir.damageFromSources[i] * enemies[ir.originalAttackers[i]]->leech));
                if (heal != 0)
                {
                    printf("%s heal's off of you for %i because of %s.\n", enemies[ir.originalAttackers[i]]->name.c_str(), heal, ir.names[i].c_str());
                    enemies[ir.originalAttackers[i]]->health = std::min(enemies[ir.originalAttackers[i]]->maxHealth, enemies[ir.originalAttackers[i]]->health + heal);
                }
            }
        }
        else if (prompt == "attack" || prompt == "spare")
        {
            printf("");
            for (int i = 0; i < enemies.size(); i++)
            {
                if (!enemies[i]->IsStunned())
                {
                    TurnReturn tr = enemies[i]->TakeTurn(i);
                    for (int j = 0; j < tr.summons.size(); j++)
                    {
                        (tr.summons[j])->summoned = true;
                        printf("%s has birthed a new %s!\n", enemies[i]->name.c_str(), (tr.summons[j])->name.c_str());
                        enemies.push_back(new Entity(*tr.summons[j]));
                    }

                    player.ApplyHit(tr.hit, false);
                    int heal = static_cast<int>(floorf(tr.hit.damage * enemies[i]->leech));
                    if (heal != 0)
                    {
                        printf("%s heal's off of you for %i.\n", enemies[i]->name.c_str(), heal);
                        enemies[i]->health = std::min(enemies[i]->maxHealth, enemies[i]->health + heal);
                    }
                }
                else
                {
                    printf("%s did not attack this round as they were stunned.\n", enemies[i]->name.c_str());
                }
            }

            for (int i = 0; i < enemies.size(); i++)
            {
                InflictionResults ir = enemies[i]->UpdateInflictions();
                for (int j = 0; j < ir.damageFromSources.size(); j++)
                {
                    int heal = static_cast<int>(floor(ir.damageFromSources[j] * player.weapon.leech));
                    if (heal != 0)
                    {
                        printf("You heal off of %s for %i because of %s.\n", enemies[i]->name.c_str(), heal, ir.names[j].c_str());
                        player.health = std::min(player.maxHealth, player.health + heal);
                    }
                }
            }

            if (!player.IsStunned())
            {
                if (spare)
                {
                    if (spareable)
                    {
                        bool spareSucceeds = RandRange(1, 3) == 3;
                        if (spareSucceeds)
                        {
                            printf("You attempt to spare and are successful!\n");
                            specialFightEnding = true;
                            specialFightEndingMonsters = enemies;
                            fightOn = false;
                            break;
                        }
                        printf("You attempt to spare and are unsuccessful.\n");
                    }
                    else
                        printf("It seems that this foe does not want to be spared.\n");
                }
                else
                {
                    TurnReturn tr = player.TakeTurn();
                    enemies[target]->ApplyHit(tr.hit);
                    player.health = std::min(player.maxHealth, player.health + static_cast<int>(floorf(tr.hit.damage * player.weapon.leech)));
                    printf("");
                }
            }
            else
                printf("Becuase you were stunned you didn't attack.\n");

            InflictionResults ir = player.UpdateInflictions();
            for (int i = 0; i < ir.damageFromSources.size(); i++)
            {
                int heal = static_cast<int>(floorf(ir.damageFromSources[i] * enemies[ir.originalAttackers[i]]->leech));
                if (heal != 0)
                {
                    printf("%s heal's off of you for %i because of %s.\n", enemies[ir.originalAttackers[i]]->name.c_str(), heal, ir.names[i].c_str());
                    enemies[ir.originalAttackers[i]]->health = std::min(enemies[ir.originalAttackers[i]]->maxHealth, enemies[ir.originalAttackers[i]]->health + heal);
                }
            }
        }
        else if(prompt == "switch")
            player.weapon.SwitchAttacks("", true);

        else
        {
            int answer = -1;
            while (answer < 0 || answer > enemies.size())
            {
                for (int i = 0; i < enemies.size(); i++)
                    printf("%s with %i health: '%i'\n", enemies[i]->name.c_str(), enemies[i]->health, i + 1);
                string prompt = Input("or 'nevermind'.\n");
                if (prompt == "nevermind")
                    answer = target;
                else if (IsNumber(prompt))
                    answer = stoi(prompt) - 1;
            }
            target = answer;
        }

        //                        \/ \/ \/ Just a failsafe, shouldn't normally come up. Will not say "You won!".
        if (player.health <= 0 || enemies.size() == 0)
        {
            fightOn = false;
            break;
        }

        int enemiesRemovedThisFrame = 0;
        for (int i = 0; i < enemies.size() + enemiesRemovedThisFrame; i++)
            if (enemies[i - enemiesRemovedThisFrame]->health <= 0)
            {
                printf("%s has been defeated!\n", enemies[i - enemiesRemovedThisFrame]->name.c_str());

                if (enemies.size() <= 1)
                {
                    printf("You won!\n");
                    fightOn = false;
                    break;
                }

                int inflictionsRemovedThisFrame = 0;
                for (int j = 0; j < player.inflictionAttackers.size() + inflictionsRemovedThisFrame; j++)
                {
                    if (player.inflictionAttackers[j - inflictionsRemovedThisFrame] == i - enemiesRemovedThisFrame)
                    {
                        printf("%s has worn off.\n", player.inflictions[j - inflictionsRemovedThisFrame].Name().c_str());
                        player.inflictionAttackers.erase(player.inflictionAttackers.begin() + j - inflictionsRemovedThisFrame);
                        player.inflictions.erase(player.inflictions.begin() + j - inflictionsRemovedThisFrame);
                        inflictionsRemovedThisFrame++;
                    }
                    else if (player.inflictionAttackers[j - inflictionsRemovedThisFrame] > i - enemiesRemovedThisFrame)
                        player.inflictionAttackers[j - inflictionsRemovedThisFrame]--;
                }
                enemies.erase(enemies.begin() + i - enemiesRemovedThisFrame);
                enemiesRemovedThisFrame++;
                if (target >= enemies.size())
                    target--;
            }

        vector<string> enemyNames = vector<string>(enemies.size());
        for (int i = 0; i < enemies.size(); i++)
            enemyNames[i] = enemies[i]->name;

        for (vector<string> option : specialEnding)
            if (option == enemyNames)
            {
                string printableCombinedEnemyNames = enemies[0]->name;
                if (enemies.size() == 2)
                {
                    if (enemies[0]->name == enemies[1]->name)
                        printableCombinedEnemyNames += "s";
                    else
                        printableCombinedEnemyNames += " and the " + enemies[1]->name;
                }
                else if (enemies.size() > 2)
                {
                    for (int i = 1; i < enemies.size() - 1; i++)
                        printableCombinedEnemyNames += ", the " + enemies[i]->name;
                    printableCombinedEnemyNames += ", and the " + enemies[enemies.size() - 1]->name;
                }

                bool hasHave = enemies.size() > 1; // false = has, true = have;

                printf("The %s %s chosen to stop fighting.\n", printableCombinedEnemyNames.c_str(), hasHave ? "have" : "has");

                fightOn = false;
                specialFightEnding = true;
                specialFightEndingMonsters = enemies;
                break;
            }

        bool allSummoned = true;
        for (Entity* enemy : enemies)
            allSummoned &= enemy->summoned;

        if (allSummoned)
        {
            string printableCombinedEnemyNames = enemies[0]->name;
            if (enemies.size() == 2)
            {
                if (enemies[0]->name == enemies[1]->name)
                    printableCombinedEnemyNames += "s";
                else
                    printableCombinedEnemyNames += " and the " + enemies[1]->name;
            }
            else if (enemies.size() > 2)
            {
                for (int i = 1; i < enemies.size() - 1; i++)
                    printableCombinedEnemyNames += ", " + enemies[i]->name;
                printableCombinedEnemyNames += ", and the" + enemies[enemies.size() - 1]->name;
            }


            bool hasHave = enemies.size() > 1; // false = has, true = have;

            printf("The %s %s chosen to stop fighting.\n", printableCombinedEnemyNames.c_str(), hasHave ? "have" : "has");
            fightOn = false;
            specialFightEnding = true;
            specialFightEndingMonsters = enemies;
            break;
        }
    }




    if (player.health <= 0)
    {
        End();
        return;
    }

    player.inflictions.clear();
    player.inflictionAttackers.clear();
    player.health = std::min(player.maxHealth, player.health + 50);
    printf("You end the fight with %i health.\n", player.health);
}
#pragma endregion





void Run() // Runs the game, this function is called once per playthrough.
{

}





int main()
{
    Term::Terminal term(false);
    Term::terminal_title("M");

    FindSettings();
    while (restart)
        Run();
}