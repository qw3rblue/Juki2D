#pragma once
#include <SFML/Graphics/Font.hpp>
#include <memory>
#include <string>
#include <unordered_map>

namespace Juki2D {
class FontStore {
public:
    std::shared_ptr<const sf::Font> get(const std::string& path);
private:
    std::unordered_map<std::string, std::shared_ptr<sf::Font>> fonts;
};
}
