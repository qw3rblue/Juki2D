#pragma once
#include <Juki2D/Input.hpp>
#include <Juki2D/Text.hpp>
#include <optional>
#include <span>

namespace Juki2D {
	sf::ConvexShape makeRoundedRectangle(sf::Vector2f size, float radius);
	enum class Anchor {
		TopLeft,
		TopCenter,
		TopRight,
		CenterLeft,
		Center,
		CenterRight,
		BottomLeft,
		BottomCenter,
		BottomRight
	};

	enum class LayoutDirection {
		Horizontal,
		Vertical
	};

	class UILayout {
	public:
		static sf::FloatRect anchor(const sf::FloatRect& parent, sf::Vector2f size,
			Anchor anchor, sf::Vector2f offset = { 0.f, 0.f }, float padding = 0.f);
		static std::vector<sf::FloatRect> stack(const sf::FloatRect& parent,
			std::span<const sf::Vector2f> sizes, LayoutDirection direction,
			float spacing = 8.f, float padding = 0.f, TextAlignment alignment = TextAlignment::Left);
	};

	class UICanvas {
	private:
		sf::Vector2f size;
		sf::Vector2u windowSize = { 0, 0 };
		sf::View view;
		sf::IntRect viewport;

	public:
		explicit UICanvas(sf::Vector2f size = { 1280.f, 720.f });
		void setWindowSize(sf::Vector2u value);
		void apply(sf::RenderWindow& window);
		std::optional<sf::Vector2f> mapPixel(sf::Vector2i pixel) const;
		sf::FloatRect getBounds() const;
	};

	struct ButtonStyle {
		sf::Color normal = { 35, 65, 85 };
		sf::Color hovered = { 45, 105, 125 };
		sf::Color pressed = { 25, 140, 150 };
		sf::Color disabled = { 45, 48, 55 };
		sf::Color text = sf::Color::White;
		float cornerRadius = 0.f;
		sf::Color outline = sf::Color::Transparent;
		float outlineThickness = 0.f;
	};

	class Button : public sf::Drawable {
	private:
		Text label;
		sf::ConvexShape background;
		sf::FloatRect bounds;
		ButtonStyle style;
		bool enabled = true;
		bool hovered = false;
		bool armed = false;
		bool clicked = false;

		void updateVisual();
		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	public:
		Button(FontStore& fonts, const std::string& fontPath, const sf::String& string);
		void setBounds(const sf::FloatRect& bounds);
		void setString(const sf::String& value);
		void setSize(unsigned value);
		void setStyle(const ButtonStyle& value);
		void setEnabled(bool value);
		void update(const Input& input, const UICanvas& canvas);
		sf::FloatRect getBounds() const;
		bool isHovered() const;
		bool isPressed() const;
		bool wasClicked() const;
	};
}
