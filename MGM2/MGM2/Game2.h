#include "Player.h"

void Game::Update()
{
	input.Update();

	steady_clock::time_point currentTime = high_resolution_clock::now();
	float deltaTime = (currentTime - startTime - lastUpdate).count() / 1000000000.0f;
	tTime += deltaTime;
	lastUpdate = currentTime - startTime;

	for (Entity* entity : entities)
		entity->Update(this, deltaTime);

	window.clear();
	for (int x = 1; x < screenDim.x; x++)
		for (int y = 1; y < screenDim.y; y++)
		{
			int x2 = JMod(x - 1 - playerPos.x, screenDim.x - 1) + 1;
			int y2 = JMod(y - 1 + playerPos.y, screenDim.y - 1) + 1;
			window.set_char(x2, y2, ' ');
			window.set_fg(x2, y2, { x2 % 2u, 0, 0 });
			window.set_bg(x2, y2, { x2 % 2u, 0, 0 });
			//window.set_fg(x2, y2, { 0, 0, static_cast<byte>(x) });
			//window.set_bg(x2, y2, { 0, static_cast<byte>(x), 0 });
			//window.set_fg(x2, y2, { static_cast<byte>(x * 255 / screenDim.x), static_cast<byte>(y * 255 / screenDim.y), 0 });
			//window.set_bg(x2, y2, { 0, 0, static_cast<byte>(x + y) * 5u % 255u });
		}

	for (Entity* entity : entities)
		entity->DUpdate(this, deltaTime);
	for (Entity* entity : entities)
		entity->UIUpdate(this, deltaTime);
	std::cout << window.render(1, 1, true);

	shouldRun ^= Input::FindKey(VK_ESCAPE);
}