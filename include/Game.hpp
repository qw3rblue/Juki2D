#pragma once

#include <SFML/Graphics.hpp>
#include <Juki2D/Application.hpp>
#include <Juki2D/TextureStore.hpp>
#include <vector>
#include <Player.hpp>

class Game : public Juki2D::Application{
protected:
	void onUpdate(float dt) override;
	void onRender(sf::RenderWindow& window) override;
private:
	sf::View camera;
	std::vector<sf::RectangleShape> platforms;
	std::vector<sf::FloatRect> obstacleBounds;

	Juki2D::TextureStore textures;
	sf::Sprite background;
	Player player;

	void updateCamera();
public:
	Game();
};