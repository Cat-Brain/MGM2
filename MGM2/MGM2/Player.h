#include "BasicEntities.h"

class Player : public FunctionalEntity
{
public:
    using FunctionalEntity::FunctionalEntity;

    bool TUpdate(Game* game, float deltaTime, float bigDeltaTime) override
    {
        pos.x += int(game->input.d.held) - int(game->input.a.held);
        pos.y += int(game->input.w.held) - int(game->input.s.held);

        game->playerPos = pos;

        return (game->input.d.held ^ game->input.a.held) || (game->input.w.held ^ game->input.s.held);
    }
};