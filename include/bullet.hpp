#ifndef BULLET_HPP
#define BULLET_HPP


#include <box2d/box2d.h>

#include "actor.hpp"

/**
 * Draw the screen for the player
 */
class Bullet : public Actor {
public:
	static constexpr float RADIUS = 0.7;

	Bullet(b2Vec2 position, float damage);

	void update(void) override;
	
	/**
	 * Tell the actor it collided with the given actor
	 *
	 * @param a Actor it collided with
	 */
	virtual void onCollision(Actor &a);

	void draw(std::shared_ptr<sf::RenderWindow> window) override;


private:
	b2CircleShape shape;
	float damage;
	int age;
	sf::CircleShape drawable;
	sf::Texture texture;
	sf::Sprite sprite;
};


#endif
