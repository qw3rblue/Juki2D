#pragma once
#include <SFML/Graphics/Texture.hpp>
#include <string>
#include <unordered_map>
#include <memory>

namespace Juki2D {
	class TextureStore {
	private:
		std::unordered_map<std::string, std::unique_ptr<sf::Texture>> textures;
	public:
		const sf::Texture& get(const std::string& path);
	};
}