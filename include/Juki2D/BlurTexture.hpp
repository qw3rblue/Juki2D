#pragma once
#include <SFML/Graphics.hpp>

namespace Juki2D {
	class BlurTexture {
	private:
		sf::RenderTexture horizontal;
		sf::RenderTexture vertical;
		sf::Shader shader;
		bool shaderReady = false;

	public:
		bool generate(const sf::Texture& source, float radius = 6.f);
		const sf::Texture& getTexture() const;
	};
}
