#include "Player.hpp"
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <Juki2D/Input.hpp>
#include <Juki2D/SpriteAnimator.hpp>
#include <vector>

Player::Player(Juki2D::TextureStore& textures)
    : visual(textures.get("assets/sprites/orc/Orc_Idle.png")),
    body({ 100.f, 100.f }, { 40.f, 60.f })
{
    idleTexture = &textures.get("assets/sprites/orc/Orc_Idle.png");
    walkTexture = &textures.get("assets/sprites/orc/Orc_Walk.png");

    idleClip.looping = true;
    walkClip.looping = true;

    for (int i = 0; i < 6; ++i) {
        idleClip.frames.push_back({
            sf::IntRect({ i * 100, 0 },{ 100, 100 }),
            0.15f
            });
    }

    for (int i = 0; i < 8; ++i) {
        walkClip.frames.push_back({
            sf::IntRect({ i * 100, 0 },{ 100, 100 }),
            0.1f
            });
    }

    visual.setScale({ 3.f, 3.f });
    visual.setOrigin({ 50.f, 57.f });
    animator.play(idleClip, visual);
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

    if (velocity.x < 0.f) {
        facingRight = false;
    }
    else if (velocity.x > 0.f) {
        facingRight = true;
    }

    visual.setScale({ facingRight ? 3.f : -3.f, 3.f });

    const bool shouldWalk = velocity.x != 0.f;

    if (shouldWalk != walking) {
        walking = shouldWalk;

        if (walking) {
            visual.setTexture(*walkTexture);
            animator.play(walkClip, visual);
        }
        else {
            visual.setTexture(*idleTexture);
            animator.play(idleClip, visual);
        }
    }
}

void Player::update(
    float dt,
    const std::vector<sf::FloatRect>& obstacles
) {
    body.update(dt, obstacles);
    animator.update(dt, visual);
}

sf::FloatRect Player::getBounds() const {
    return body.getBounds();
}

void Player::render(sf::RenderWindow& window) {
    const auto bounds = body.getBounds();

    visual.setPosition({
        bounds.position.x + bounds.size.x / 2.f,
        bounds.position.y + bounds.size.y
        });

    sf::RectangleShape outline(bounds.size);
    outline.setPosition(bounds.position);
    outline.setFillColor(sf::Color::Transparent);
    outline.setOutlineColor(sf::Color::Red);
    outline.setOutlineThickness(1.f);

    window.draw(outline);
    window.draw(visual);
}