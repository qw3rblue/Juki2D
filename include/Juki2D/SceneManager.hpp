#pragma once
#include <Juki2D/Scene.hpp>
#include <memory>

namespace Juki2D {
class SceneManager {
public:
    void requestChange(std::unique_ptr<Scene> scene);
    void update(float dt, const Input& input);
    void render(sf::RenderWindow& window);

private:
    void applyPending();
    std::unique_ptr<Scene> active;
    std::unique_ptr<Scene> pending;
};
}
