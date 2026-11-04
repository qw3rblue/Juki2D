#include <Juki2D/SceneManager.hpp>
#include <stdexcept>
#include <utility>

void Juki2D::SceneManager::requestChange(std::unique_ptr<Scene> scene) {
    if (!scene) {
        throw std::invalid_argument("Cannot switch to a null scene.");
    }
    pending = std::move(scene);
}

void Juki2D::SceneManager::applyPending() {
    if (pending) {
        active = std::move(pending);
    }
}

void Juki2D::SceneManager::update(float dt, const Input& input) {
    applyPending();
    if (active) {
        active->update(dt, input);
    }
    // The old scene's update has returned before it is destroyed.
    applyPending();
}

void Juki2D::SceneManager::render(sf::RenderWindow& window) {
    if (active) {
        active->render(window);
    }
}
