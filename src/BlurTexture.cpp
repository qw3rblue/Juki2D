#include <Juki2D/BlurTexture.hpp>
#include <algorithm>
#include <cmath>

namespace {
	constexpr auto fragment = R"(
#version 120
uniform sampler2D source;
uniform vec2 direction;

void main() {
	vec2 position = gl_TexCoord[0].xy;
	vec4 color = texture2D(source, position) * 0.227027;
	color += texture2D(source, position + direction * 1.384615) * 0.316216;
	color += texture2D(source, position - direction * 1.384615) * 0.316216;
	color += texture2D(source, position + direction * 3.230769) * 0.070270;
	color += texture2D(source, position - direction * 3.230769) * 0.070270;
	gl_FragColor = color;
}
)";
}

bool Juki2D::BlurTexture::generate(const sf::Texture& source, float radius) {
	const auto sourceSize = source.getSize();

	if (sourceSize.x == 0 || sourceSize.y == 0 || !std::isfinite(radius) || radius < 0.f) {
		return false;
	}

	if (!shaderReady) {
		if (!sf::Shader::isAvailable() || !shader.loadFromMemory(fragment, sf::Shader::Type::Fragment)) {
			return false;
		}
		shaderReady = true;
	}

	const float scale = std::min(1.f, 960.f / static_cast<float>(std::max(sourceSize.x, sourceSize.y)));
	const sf::Vector2u size = {
		std::max(1u, static_cast<unsigned>(sourceSize.x * scale)),
		std::max(1u, static_cast<unsigned>(sourceSize.y * scale))
	};

	if ((horizontal.getSize() != size && !horizontal.resize(size))
		|| (vertical.getSize() != size && !vertical.resize(size))) {
		return false;
	}

	horizontal.setSmooth(true);
	vertical.setSmooth(true);
	shader.setUniform("source", sf::Shader::CurrentTexture);
	shader.setUniform("direction", sf::Glsl::Vec2(radius / sourceSize.x, 0.f));

	sf::RenderStates states;
	states.shader = &shader;
	states.blendMode = sf::BlendNone;
	sf::Sprite original(source);
	original.setScale({
		static_cast<float>(size.x) / sourceSize.x,
		static_cast<float>(size.y) / sourceSize.y
	});
	horizontal.clear(sf::Color::Transparent);
	horizontal.draw(original, states);
	horizontal.display();

	shader.setUniform("direction", sf::Glsl::Vec2(0.f, radius / sourceSize.y));
	sf::Sprite intermediate(horizontal.getTexture());
	vertical.clear(sf::Color::Transparent);
	vertical.draw(intermediate, states);
	vertical.display();
	return true;
}

const sf::Texture& Juki2D::BlurTexture::getTexture() const {
	return vertical.getTexture();
}
