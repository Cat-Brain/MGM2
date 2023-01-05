#include "Combat.h"

void Game::Update()
{
	input.Update();

	for (Entity* entity : entities)
		entity->Update(this);

	window.clear();
	for (int x = 1; x < screenDim.x; x++)
		for (int y = 1; y < screenDim.y; y++)
		{
			int x2 = JMod(x - 1 + playerPos.x, screenDim.x - 1) + 1;
			int y2 = JMod(y - 1 - playerPos.y, screenDim.y - 1) + 1;
			window.set_char(x2, y2, '#');
			window.set_fg(x2, y2, { static_cast<byte>(x * 255 / screenDim.x), static_cast<byte>(y * 255 / screenDim.y), 0 });
			window.set_bg(x2, y2, { 0, 0, static_cast<byte>(x + y) * 5u % 255u });
		}
	std::cout << window.render(1, 1, true);

	shouldRun ^= Input::FindKey(VK_ESCAPE);
}