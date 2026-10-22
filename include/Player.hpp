#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <Collision.hpp>
#include <Juki2D/Body2D.hpp>
#include <Juki2D/Input.hpp>
#include <Juki2D/TextureStore.hpp>
#include <Juki2D/SpriteAnimator.hpp>
#include <vector>

class Player {
private:
	sf::Sprite visual;
	Juki2D::Body2D body;
	Juki2D::SpriteAnimator animator;

	float moveSpeed = 250.f;
	float jumpForce = 600.f;

	const sf::Texture* idleTexture = nullptr;
	const sf::Texture* walkTexture = nullptr;

	Juki2D::AnimationClip idleClip;
	Juki2D::AnimationClip walkClip;

	bool walking = false;
	bool facingRight = true;
public:
	Player(Juki2D::TextureStore& textures);

	sf::FloatRect getBounds() const;

	void handleInput(const Juki2D::Input&);
	void update(float dt, const std::vector<sf::FloatRect>& obstacles);
	void render(sf::RenderWindow& window);
};