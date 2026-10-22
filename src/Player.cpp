#include "Player.hpp"
#include <SFML/Window/Keyboard.hpp>
#include <Juki2D/Input.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <vector>

Player::Player(Juki2D::TextureStore& textures)
    : body({ 100, 100 }, { 40, 60 }),
    visual(textures.get("assets/azi.png"))
{
    const sf::Vector2u textureSize = visual.getTexture().getSize();
    const sf::Vector2f bodySize = body.getBounds().size;

    visual.setScale({
        bodySize.x / static_cast<float>(textureSize.x),
        bodySize.y / static_cast<float>(textureSize.y)
        });
}

void Player::handleInput(const Juki2D::Input& input) {
    sf::Vector2f velocity = body.getVelocity();
    velocity.x = 0.f;

    if (input.isHeld(sf::Keyboard::Key::A)) {
        velocity.x = -moveSpeed;
    }

    if (input.isHeld(sf::Keyboard::Key::D)) {
        velocity.x = moveSpeed;
    }

    if (input.wasPressed(sf::Keyboard::Key::Space)
        && body.isGrounded()) {
        velocity.y = -jumpForce;
    }

    body.setVelocity(velocity);
}

void Player::update(float dt, const std::vector<sf::FloatRect>& obstacles) {
    body.update(dt, obstacles);
}

sf::FloatRect Player::getBounds() const {
    return body.getBounds();
}

void Player::render(sf::RenderWindow& window) {
    visual.setPosition(body.getBounds().position);
    window.draw(visual);
}