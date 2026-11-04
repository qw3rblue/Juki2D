#pragma once
#include <algorithm>
#include <cmath>

namespace Juki2D {
	class ScreenFade {
	private:
		float elapsed = 0.f;
		float duration = 0.6f;
		float start = 1.f;
		float target = 0.f;
		float opacity = 1.f;

	public:
		void fadeIn() {
			start = 1.f;
			target = 0.f;
			opacity = start;
			elapsed = 0.f;
		}

		void fadeOut() {
			start = 0.f;
			target = 1.f;
			opacity = start;
			elapsed = 0.f;
		}

		void update(float dt) {
			if (!std::isfinite(dt) || dt <= 0.f) {
				return;
			}

			elapsed = std::min(duration, elapsed + dt);
			const float progress = elapsed / duration;
			const float blend = progress * progress * (3.f - 2.f * progress);
			opacity = start + (target - start) * blend;
		}

		bool isActive() const {
			return elapsed < duration;
		}

		float getOpacity() const {
			return opacity;
		}
	};
}
