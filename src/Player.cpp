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

void Player::resolveCollision(sf::FloatRect object, Collision::Side side) {
    switch (side) {

    case Collision::Side::Top:
        body.setPosition({
            body.getPosition().x,
            object.position.y - body.getSize().y
            });

        velocity.y = 0.f;
        grounded = true;
        break;
    case Collision::Side::Bottom:
        body.setPosition({
            body.getPosition().x,
            object.position.y + object.size.y
            });

        velocity.y = 0.f;
        grounded = false;
        break;
    case Collision::Side::Left:
        body.setPosition({
            object.position.x - body.getSize().x,
            body.getPosition().y
            });

        velocity.x = 0.f;
        break;
    case Collision::Side::Right:
        body.setPosition({
            object.position.x + object.size.x,
            body.getPosition().y
            });

        velocity.x = 0.f;
        break;
    }
}

void Player::update(float dt) {
    grounded = false;
    velocity.y += gravity * dt;
}

void Player::move(sf::Vector2f displacement) {
    body.move(displacement);
}

void Player::respondToCollision(sf::Vector2f normal) {
    if (velocity.x * normal.x < 0.f) {
        velocity.x = 0.f;
    }

    if (velocity.y * normal.y < 0.f) {
        velocity.y = 0.f;
    }

    if (normal.y < 0.f) {
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