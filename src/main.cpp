#include <Game.hpp>

#include <iostream>
#include<filesystem>

int main()
{
	std::cout << std::filesystem::current_path() << '\n';

	Game game;
	game.run();

	return 0;
}
