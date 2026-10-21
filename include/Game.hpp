#pragma once
#include <SFML/Graphics.hpp>
#include <Juki2D/Application.hpp>
#include <vector>
#include "Player.hpp"

class Game : public Juki2D::Application{
protected:
	void onUpdate(float dt) override;
	void onRender(sf::RenderWindow& window) override;
private:
	sf::View camera;
	std::vector<sf::RectangleShape> platforms;

	Player player;

	void updateCamera();
public:
	Game();
};