#include <Juki2D/FontStore.hpp>
#include <stdexcept>

std::shared_ptr<const sf::Font> Juki2D::FontStore::get(const std::string& path) {
    if (auto it = fonts.find(path); it != fonts.end()) {
        return it->second;
    };

    auto font = std::make_shared<sf::Font>();

    if (!font->openFromFile(path)) { 
        throw std::runtime_error("Failed to load font: " + path);
    };

    fonts.emplace(path, font);
    return font;
}