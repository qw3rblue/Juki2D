#pragma once
#include <string>

#include <SFML/Graphics.hpp>
#include <Juki2D/Input.hpp>

namespace Juki2D {
	class Application {

	public:
		Application(sf::Vector2u size, std::string title);
		virtual ~Application() = default;
		void run();
		const Input& getInput() const;

	protected:
		virtual void onUpdate(float elapsed) = 0;
		virtual void onRender(sf::RenderWindow& window) = 0;

	private:
		sf::RenderWindow window;
		Juki2D::Input input;
	};
}