#pragma once
#include <Juki2D/FontStore.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdint>
#include <limits>

namespace Juki2D {
	enum class TextAlignment {
		Left,
		Center,
		Right
	};

	class Text : public sf::Drawable {
	private:
		std::shared_ptr<const sf::Font> font;
		sf::String string;
		mutable std::vector<sf::Text> lines;
		mutable std::vector<sf::Text> visibleLines;
		mutable bool visibilityDirty = true;
		std::size_t visibleCharacters = std::numeric_limits<std::size_t>::max();
		std::uint32_t style = sf::Text::Regular;
		sf::Vector2f position = { 0.f, 0.f };
		mutable sf::Vector2f layoutSize = { 0.f, 0.f };
		sf::Color color = sf::Color::White;

		unsigned size = 24;
		float wrapWidth = 0.f;
		TextAlignment alignment = TextAlignment::Left;

		float measure(const sf::String& value) const;
		mutable bool layoutDirty = true;

		void ensureLayout() const;
		void rebuild() const;
		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	public:
		Text(FontStore& fonts, const std::string& path, const sf::String& string = "", unsigned size = 24);
		Text(const Text&) = delete;
		Text& operator=(const Text&) = delete;

		void setString(const sf::String& value);
		void setStyle(std::uint32_t value);
		void setVisibleCharacters(std::size_t value);
		void setPosition(sf::Vector2f value);
		void setSize(unsigned value);
		void setColor(sf::Color value);
		void setWrapWidth(float value);
		void setAlignment(TextAlignment value);

		sf::FloatRect getBounds() const;
		sf::Vector2f getLayoutSize() const;
	};
}
