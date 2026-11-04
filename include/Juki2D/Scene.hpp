#pragma once
#include <Juki2D/Input.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

namespace Juki2D {
class Scene {
public:
    virtual ~Scene() = default;
    virtual void update(float dt, const Input& input) = 0;
    virtual void render(sf::RenderWindow& window) = 0;
};
}
