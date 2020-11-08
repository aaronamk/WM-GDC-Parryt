#include <SFML/Graphics.hpp>
#include <list>
#include <memory>
#include <iostream>

#include "view.hpp"
#include "player_view.hpp"
#include "character.hpp"


PlayerView::PlayerView(std::shared_ptr<MasterLogic> logic, std::shared_ptr<Character> character)
		: View(logic) {
	this->logic = logic;
	this->character = character;

	// set window
	this->window = std::make_shared<sf::RenderWindow>
			(sf::VideoMode(1280, 720, 32), "Parryt", sf::Style::Titlebar | sf::Style::Close);

	// set view to center on the character
	sf::View view = this->window->getView();
	view.setSize(sf::Vector2f(64, 36));
	view.setCenter(sf::Vector2f(this->character->getBody()->GetPosition().x,
                               -this->character->getBody()->GetPosition().y));
	this->window->setView(view);
}


// TODO: tie this method to the physics loop
void PlayerView::pollInput(void) {
	// mouse
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) ;

	// keyboard
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) ;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) this->character->goLeft();
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) /*jump down from platform*/;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) this->character->goRight();
}


void PlayerView::listen(void) {
	sf::Event event;
	while (window->pollEvent(event)) {
		switch (event.type) {
			case sf::Event::Closed:
				window->close();
				this->logic->terminate();
				break;
			case sf::Event::KeyPressed:
				switch (event.key.code) {
					case sf::Keyboard::Space:
						this->character->jump();
						break;
				}
				break;
		}
	}
}


void PlayerView::update(const float &dt) {
	this->pollInput();
	this->listen();

	// clear screen
	window->clear(sf::Color::Black);

	// draw actors
	for (auto actor : this->logic->getCurrentRoom().getActorList()) actor->draw(window);

	// screen follow character
	sf::Vector2f characterPosition = sf::Vector2f(this->character->getBody()->GetPosition().x,
	                                             -this->character->getBody()->GetPosition().y);
	sf::View view = this->window->getView();
	sf::Vector2f newCenter(view.getCenter().x, characterPosition.y);
	if (characterPosition.x + 10 < view.getCenter().x) newCenter.x -= 0.15;
	else if (characterPosition.x - 10 > view.getCenter().x) newCenter.x += 0.15;

	view.setCenter(newCenter);
	this->window->setView(view);

	// make test sprite
	sf::Texture texture;
	texture.loadFromFile("../resources/running-sequence.png");
	sf::Sprite sprite(texture, sf::IntRect(0,0,64,64));
	sprite.setScale(0.1,0.1);
	this->window->draw(sprite);

	// display screen
	window->display();
}
