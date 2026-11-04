#pragma once
#include <Juki2D/AnimationClip.hpp>
#include <cstddef>

namespace Juki2D {
	class SpriteAnimator {
	private:
		AnimationClip currentClip;
		std::size_t frame = 0;
		float accum = 0;
		bool playing = false;
	public:
		void play(const AnimationClip& clip, sf::Sprite& sprite);
		void update(float dt, sf::Sprite& sprite);
		void stop(sf::Sprite& sprite);
		void pause();
		void resume();
		bool isPlaying() const;
	};
}