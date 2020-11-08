#ifndef PARI_HPP
#define PARI_HPP


#include <box2d/box2d.h>
#include <SFML/Graphics.hpp>

#include "actor.hpp"
#include "character.hpp"


/**
 * The main character
 */
class Pari : public Character {
	public:
		static constexpr float WIDTH = 0.5;

		static constexpr float HEIGHT = 1.9;

		Pari(b2Vec2 position);

		void draw(std::shared_ptr<sf::RenderWindow> window) override;


	private:
		sf::Texture texture;
		sf::Sprite sprite;
};


#endif
