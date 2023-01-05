#include "BasicEntities.h"

class Player : public FunctionalEntity
{
public:
    using FunctionalEntity::FunctionalEntity;

    void Update(Game* game, float deltaTime) override
    {
        FunctionalEntity::Update(game, deltaTime);

        // Do the rest of the input code.
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
            byte value = std::min(255, int(255 * (tTime - attacks[i].startTime) / attacks[i].length));
            game->Draw({ i + 1, game->screenDim.y - 1 }, ' ', { value, value, value }, { value, value, value });
        }
    }
};