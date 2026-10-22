#include <Juki2D/TextureStore.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <stdexcept>
#include <utility>

const sf::Texture& Juki2D::TextureStore::get(const std::string& path) {
	const auto found = textures.find(path);

	if (found != textures.end()) {
		return *found->second;
	}

	auto texture = std::make_unique<sf::Texture>();

	if (!texture->loadFromFile(path)) {
		throw std::runtime_error("Failed to load texture: " + path);
	}

	const auto inserted = textures.emplace(path, std::move(texture));

	return *inserted.first->second;
}