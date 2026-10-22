#include <SFML/Graphics.hpp>

#include <vector>

namespace Juki2D {
	struct AnimationFrame {
		sf::IntRect rectangle;
		float duration;
	};

	struct AnimationClip {
		std::vector<AnimationFrame> frames;
		bool looping = true;
	};
}