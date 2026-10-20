#include "Player.hpp"
#include <SFML/Window/Keyboard.hpp>

Player::Player()
    : body(sf::Vector2f{ 40.f, 60.f }),
    velocity(0.f, 0.f)
{
    body.setFillColor(sf::Color::Green);
    body.setPosition({ 100.f, 100.f });
}

void Player::handleInput() {
    velocity.x = 0.f;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
        velocity.x = -moveSpeed;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
        velocity.x = moveSpeed;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)
        && grounded) {
        velocity.y = -jumpForce;
        grounded = false;
    }
}

void Player::landOn(float platformTop) {
    body.setPosition({
        body.getPosition().x,
        platformTop - body.getSize().y
    });

    velocity.y = 0.f;
    grounded = true;
}

void Player::update(float dt) {
    grounded = false;
    velocity.y += gravity * dt;
    body.move(velocity * dt);

    float floorY = 600.f;

    if (body.getPosition().y + body.getSize().y >= floorY
        && velocity.y >= 0.f) {

        body.setPosition({
            body.getPosition().x,
            floorY - body.getSize().y
            });

        velocity.y = 0.f;
        grounded = true;
    }
}

sf::FloatRect Player::getBounds() const {
    return body.getGlobalBounds();
}

sf::Vector2f Player::getVelocity() const {
    return velocity;
}

void Player::render(sf::RenderWindow& window) {
    window.draw(body);
}