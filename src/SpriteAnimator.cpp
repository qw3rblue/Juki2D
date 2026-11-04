#include <cmath>
#include <stdexcept>
#include <Juki2D/SpriteAnimator.hpp>

void Juki2D::SpriteAnimator::play(
	const AnimationClip& clip,
	sf::Sprite& sprite
) {
	if (clip.frames.empty()) {
		throw std::invalid_argument("Animation clip has no frames.");
	}

	for (const auto& animationFrame : clip.frames) {
		if (!std::isfinite(animationFrame.duration)
			|| animationFrame.duration <= 0.f) {
			throw std::invalid_argument(
				"Frame duration must be finite and greater than zero."
			);
		}

		if (animationFrame.rectangle.size.x <= 0
			|| animationFrame.rectangle.size.y <= 0) {
			throw std::invalid_argument(
				"Frame dimensions must be greater than zero."
			);
		}
	}

	currentClip = clip;
	frame = 0;
	accum = 0.f;
	playing = true;

	sprite.setTextureRect(currentClip.frames[frame].rectangle);
}

void Juki2D::SpriteAnimator::update(float dt, sf::Sprite& sprite) {
	if (!playing || currentClip.frames.empty()) {
		return;
	}

	if (dt <= 0.f || !std::isfinite(dt)) {
		return;
	}

	accum += dt;

	while (accum >= currentClip.frames[frame].duration) {
		accum -= currentClip.frames[frame].duration;

		if (frame + 1 < currentClip.frames.size()) {
			++frame;
		}
		else if (currentClip.looping) {
			frame = 0;
		}
		else {
			playing = false;
			accum = 0.f;
			break;
		}
	}

	sprite.setTextureRect(currentClip.frames[frame].rectangle);
}

void Juki2D::SpriteAnimator::stop(sf::Sprite& sprite) {
	playing = false;
	frame = 0;
	accum = 0.f;

	if (!currentClip.frames.empty()) {
		sprite.setTextureRect(currentClip.frames[0].rectangle);
	}
}

bool Juki2D::SpriteAnimator::isPlaying() const {
	return playing;
}

void Juki2D::SpriteAnimator::resume() {
	if (!currentClip.frames.empty()) {
		playing = true;
	}
}

void Juki2D::SpriteAnimator::pause() {
	playing = false;
}