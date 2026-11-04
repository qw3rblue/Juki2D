#include <Juki2D/SplashScene.hpp>
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <stdexcept>
#include <utility>

Juki2D::SplashScene::SplashScene(SceneManager& scenes, const std::string& logoPath,
	std::unique_ptr<Scene> nextScene, float fadeInTime, float holdTime, float fadeOutTime)
	: scenes(scenes), nextScene(std::move(nextScene)), texture(logoPath), logo(texture),
	fadeInTime(fadeInTime), holdTime(holdTime), fadeOutTime(fadeOutTime)
{
	if (!this->nextScene) {
		throw std::invalid_argument("Splash screen requires a following scene.");
	}

	if (!std::isfinite(fadeInTime) || !std::isfinite(holdTime) || !std::isfinite(fadeOutTime)
		|| fadeInTime <= 0.f || holdTime < 0.f || fadeOutTime <= 0.f) {
		throw std::invalid_argument("Splash fade times must be positive and the hold time nonnegative.");
	}

	const auto screen = canvas.getBounds();
	const auto imageSize = texture.getSize();
	const float scale = std::min(screen.size.x / imageSize.x, screen.size.y / imageSize.y);
	texture.setSmooth(true);
	logo.setScale({ scale, scale });
	logo.setPosition({
		(screen.size.x - imageSize.x * scale) / 2.f,
		(screen.size.y - imageSize.y * scale) / 2.f
	});
	logo.setColor(sf::Color(255, 255, 255, 0));
	background.setSize(screen.size);
	background.setFillColor(sf::Color::Black);
}

void Juki2D::SplashScene::update(float dt, const Input&) {
	if (!std::isfinite(dt) || dt <= 0.f) {
		return;
	}

	const float duration = fadeInTime + holdTime + fadeOutTime;
	elapsed = std::min(elapsed + dt, duration);

	if (elapsed >= duration && nextScene) {
		scenes.requestChange(std::move(nextScene));
	}
}

void Juki2D::SplashScene::render(sf::RenderWindow& window) {
	canvas.apply(window);
	float opacity = 1.f;

	if (elapsed < fadeInTime) {
		opacity = elapsed / fadeInTime;
	}
	else if (elapsed > fadeInTime + holdTime) {
		opacity = 1.f - (elapsed - fadeInTime - holdTime) / fadeOutTime;
	}

	opacity = std::clamp(opacity, 0.f, 1.f);
	opacity = opacity * opacity * (3.f - 2.f * opacity);
	logo.setColor(sf::Color(255, 255, 255, static_cast<std::uint8_t>(255.f * opacity)));
	window.draw(background);
	window.draw(logo);
}
