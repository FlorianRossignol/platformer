#pragma once
#include <string>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>


class Game;
class b2Body;

namespace sf {
	class RenderWindow;
}

class Boss
{
public:
	const std::string name;
	void Init();
	void Render(sf::RenderWindow& window);
	void Update(float dt);
private:
	
	sf::Texture Idle_texture_;
	sf::Sprite sprite_;
};
