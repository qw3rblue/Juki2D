#pragma once
#include <Juki2D/SceneManager.hpp>
#include <Juki2D/UI.hpp>
#include <memory>
#include <string>

namespace Juki2D {
	class SplashScene : public Scene {
	private:
		SceneManager& scenes;
		std::unique_ptr<Scene> nextScene;
		sf::Texture texture;
		sf::Sprite logo;
		UICanvas canvas;
		sf::RectangleShape background;
		float elapsed = 0.f;
		float fadeInTime;
		float holdTime;
		float fadeOutTime;

	public:
		SplashScene(SceneManager& scenes, const std::string& logoPath,
			std::unique_ptr<Scene> nextScene, float fadeInTime = 0.8f,
			float holdTime = 1.2f, float fadeOutTime = 0.8f);
		void update(float dt, const Input& input) override;
		void render(sf::RenderWindow& window) override;
	};
}
