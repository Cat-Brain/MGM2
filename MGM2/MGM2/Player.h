#include "BasicEntities.h"

class Player : public FunctionalEntity
{
public:
    using FunctionalEntity::FunctionalEntity;

    void Start() override
    {
        for (int i = 0; i < attacks.size(); i++)
            attacks[i].startTime = tTime;
    }

    void Update(Game* game, float deltaTime) override
    {
        FunctionalEntity::Update(game, deltaTime);
        
        if (game->input.leftMouse.held)
            printf("l");
        if (game->input.leftMouse.held && attacks.size() >= 1 && tTime - attacks[0].startTime > attacks[0].length)
            attacks[0].startTime = tTime;
    }

    bool TUpdate(Game* game, float deltaTime, float bigDeltaTime) override
    {
        Vec2 dir;
        dir.x = int(game->input.d.held) - int(game->input.a.held);
        dir.y = int(game->input.w.held) - int(game->input.s.held);

        TryMove(game, dir);

        game->playerPos = pos;

        return (game->input.d.held ^ game->input.a.held) || (game->input.w.held ^ game->input.s.held);
    }

    void UIUpdate(Game* game, float deltaTime) override
    {
        for (int i = 0; i < attacks.size(); i++)
        {
            byte value = std::min(254, int(255 * (tTime - attacks[i].startTime) / attacks[i].length));
            if (value < 254)
                game->Draw({ i + 1, game->screenDim.y - 1 }, ' ', { value, value, value }, { value, value, value });
            else
            {
                value = (int((tTime - attacks[i].startTime) * 10) + i) % 6  * 50;
                game->Draw({ i + 1, game->screenDim.y - 1 }, ' ', { 254, 254, value }, { 254, 254, value });
            }
        }
    }
};