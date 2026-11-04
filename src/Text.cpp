#include <Juki2D/Text.hpp>
#include <algorithm>
#include <cmath>
#include <stdexcept>

Juki2D::Text::Text(FontStore& fonts, const std::string& path, const sf::String& string, unsigned size)
	: font(fonts.get(path)), string(string), size(size)
{
	if (size == 0) {
		throw std::invalid_argument("Text size must be greater than zero.");
	}

	layoutDirty = true;
}

void Juki2D::Text::setString(const sf::String& value) {
	if (string != value) {
		string = value;
		layoutDirty = true;
	}
}

void Juki2D::Text::setStyle(std::uint32_t value) {
	if (style != value) {
		style = value;
		layoutDirty = true;
	}
}

void Juki2D::Text::setVisibleCharacters(std::size_t value) {
	if (visibleCharacters != value) {
		visibleCharacters = value;
		visibilityDirty = true;
	}
}

void Juki2D::Text::setPosition(sf::Vector2f value) {
	position = value;
}

void Juki2D::Text::setSize(unsigned value) {
	if (value == 0) {
		throw std::invalid_argument("Text size must be greater than zero.");
	}

	if (size != value) {
		size = value;
		layoutDirty = true;
	}
}

void Juki2D::Text::setColor(sf::Color value) {
	if (color == value) {
		return;
	}
	color = value;
	visibilityDirty = true;

	for (auto& line : lines) {
		line.setFillColor(color);
	}
}

void Juki2D::Text::setWrapWidth(float value) {
	if (!std::isfinite(value) || value < 0.f) {
		throw std::invalid_argument("Text wrap width must be finite and nonnegative.");
	}

	if (wrapWidth != value) {
		wrapWidth = value;
		layoutDirty = true;
	}
}

void Juki2D::Text::setAlignment(TextAlignment value) {
	if (alignment != value) {
		alignment = value;
		layoutDirty = true;
	}
}

float Juki2D::Text::measure(const sf::String& value) const {
	sf::Text sample(*font, value, size);
	sample.setStyle(style);
	const auto bounds = sample.getLocalBounds();

	return std::max(bounds.position.x + bounds.size.x, sample.findCharacterPos(value.getSize()).x)
		- std::min(0.f, bounds.position.x);
}

void Juki2D::Text::ensureLayout() const {
	if (layoutDirty) {
		rebuild();
	}
}

void Juki2D::Text::rebuild() const {
	std::vector<sf::String> wrapped;
	sf::String paragraph;

	auto wrapParagraph = [&]() {
		if (wrapWidth == 0.f || paragraph.isEmpty() || measure(paragraph) <= wrapWidth) {
			wrapped.push_back(paragraph);
			return;
		}

		std::size_t start = 0;

		while (start < paragraph.getSize()) {
			std::size_t end = start;
			std::size_t lastSpace = sf::String::InvalidPos;

			while (end < paragraph.getSize()) {
				const auto candidate = paragraph.substring(start, end - start + 1);

				if (end > start && measure(candidate) > wrapWidth) {
					break;
				}

				if (paragraph[end] == U' ') {
					lastSpace = end;
				}

				++end;
			}

			std::size_t next = end;

			if (end < paragraph.getSize() && lastSpace != sf::String::InvalidPos && lastSpace > start) {
				end = lastSpace;
				next = lastSpace + 1;
			}

			wrapped.push_back(paragraph.substring(start, end - start));
			start = next;

			while (start < paragraph.getSize() && paragraph[start] == U' ') {
				++start;
			}
		}
	};

	for (const auto character : string) {
		if (character == U'\r') {
			continue;
		}

		if (character == U'\n') {
			wrapParagraph();
			paragraph.clear();
		}
		else if (character == U'\t') {
			paragraph += "    ";
		}
		else {
			paragraph += character;
		}
	}

	wrapParagraph();
	lines.clear();
	float width = wrapWidth;

	for (const auto& line : wrapped) {
		width = std::max(width, measure(line));
	}

	const float lineSpacing = font->getLineSpacing(size);
	float y = 0.f;

	for (const auto& value : wrapped) {
		sf::Text line(*font, value, size);
		line.setStyle(style);
		float x = 0.f;
		const float lineWidth = measure(value);

		if (alignment == TextAlignment::Center) {
			x = (width - lineWidth) / 2.f;
		}
		else if (alignment == TextAlignment::Right) {
			x = width - lineWidth;
		}

		line.setPosition({ x - std::min(0.f, line.getLocalBounds().position.x), y });
		line.setFillColor(color);
		lines.push_back(std::move(line));
		y += lineSpacing;
	}

	layoutSize = { width, y };
	layoutDirty = false;
	visibilityDirty = true;
}

sf::FloatRect Juki2D::Text::getBounds() const {
	ensureLayout();
	sf::FloatRect result(position, { 0.f, 0.f });
	bool found = false;

	for (const auto& line : lines) {
		const auto bounds = line.getGlobalBounds();

		if (bounds.size.x == 0.f || bounds.size.y == 0.f) {
			continue;
		}

		const sf::Vector2f minimum = bounds.position + position;
		const sf::Vector2f maximum = minimum + bounds.size;

		if (!found) {
			result = { minimum, bounds.size };
			found = true;
		}
		else {
			const sf::Vector2f right = result.position + result.size;
			result.position = { std::min(result.position.x, minimum.x), std::min(result.position.y, minimum.y) };
			result.size = { std::max(right.x, maximum.x) - result.position.x, std::max(right.y, maximum.y) - result.position.y };
		}
	}

	return result;
}

sf::Vector2f Juki2D::Text::getLayoutSize() const {
	ensureLayout();
	return layoutSize;
}

void Juki2D::Text::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	ensureLayout();
	states.transform.translate(position);

	if (visibleCharacters >= string.getSize()) {
		for (const auto& line : lines) {
			target.draw(line, states);
		}
		return;
	}

	if (visibilityDirty) {
		visibleLines.clear();
		auto remaining = visibleCharacters;

		for (const auto& line : lines) {
			if (remaining == 0) {
				break;
			}

			const auto count = std::min(remaining, line.getString().getSize());
			visibleLines.push_back(line);
			visibleLines.back().setString(line.getString().substring(0, count));
			remaining -= count;
		}

		visibilityDirty = false;
	}

	for (const auto& line : visibleLines) {
		target.draw(line, states);
	}
}
