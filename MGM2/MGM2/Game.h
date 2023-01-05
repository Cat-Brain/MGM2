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
	steady_clock::time_point lastUpdate;

	Game() : terminal(true, true, true, true), window(0, 0)
	{
		std::tuple<size_t, size_t> unrefinedScreenDim = Term::get_size();
		screenDim.x = static_cast<int>(std::get<1>(unrefinedScreenDim));
		screenDim.y = static_cast<int>(std::get<0>(unrefinedScreenDim));
		window = Term::Window(screenDim.x, screenDim.y);
		tTime = 0;
	}

	Vec2 ToScreenSpace(Vec2 pos)
	{
		return (pos - playerPos) * Vec2(1, -1) + screenDim / 2;
	}

	void Draw(Vec2 pos, char character, Term::RGB foreground)
	{
		if (pos.x > 0 && pos.x < screenDim.x && pos.y > 0 && pos.y < screenDim.y)
		{
			window.set_char(pos.x, pos.y, character);
			window.set_fg(pos.x, pos.y, foreground);
		}
	}

	void Draw(Vec2 pos, char character, Term::RGB foreground, Term::RGB background)
	{
		if (pos.x > 0 && pos.x < screenDim.x && pos.y > 0 && pos.y < screenDim.y)
		{
			window.set_char(pos.x, pos.y, character);
			window.set_fg(pos.x, pos.y, foreground);
			window.set_bg(pos.x, pos.y, background);
		}
	}

	void Update();
};