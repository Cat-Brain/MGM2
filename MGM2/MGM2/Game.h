#include "Includes.h"

class Entity;
class Player;
class Game
{
public:
	Term::Terminal terminal;
	Term::Window window;
	Vec2 screenDim;
	vector<Entity*> entities;
	Input input;
	Player* player;
	Vec2 playerPos;

	Game() : terminal(true, true, true, true), window(0, 0)
	{
		std::tuple<size_t, size_t> unrefinedScreenDim = Term::get_size();
		screenDim.x = static_cast<int>(std::get<1>(unrefinedScreenDim));
		screenDim.y = static_cast<int>(std::get<0>(unrefinedScreenDim));
		window = Term::Window(screenDim.x, screenDim.y);
	}

	void Update();
};