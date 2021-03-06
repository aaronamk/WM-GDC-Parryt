#include <box2d/box2d.h>
#include <SFML/Graphics.hpp>

#include "pari.hpp"
#include "sprite_sheet.hpp"


Pari::Pari(b2Vec2 position) : Character(position) {
	this->allegiance = glob::Allegiance::parrot;
	this->priority = 100;
	this->setTargetable(true);

	this->acceleration = 150;
	this->deceleration = 200;
	this->jumpImpulse = 300;
	this->maxSpeed = 20;
	this->health = this->maxHealth = 10000; // he thicc
	this->maxJumps = 2;
	this->parryDuration = std::chrono::milliseconds(200);
	this->parryRechargeDuration = std::chrono::seconds(1);
	// set parry start so Pari can parry right away
	this->parryStart = std::chrono::steady_clock::now() - this->parryRechargeDuration;

	// fix shape to body
	this->shape.SetAsBox(this->WIDTH, this->HEIGHT);
	this->fixtureDef.shape = &this->shape;
	this->fixtureDef.density = 2.4f;
	this->fixtureDef.friction = 0.0f;

	// load pari spritesheet
	this->spriteSheet = std::make_unique<SpriteSheet>("../resources/pari.png", sf::Vector2i(64, 64));
	this->spriteSheet->setLoop(this->standLoop);

	// set drawable
	this->sprite = sf::Sprite(texture, sf::IntRect(0,0,64,64));

	// set old drawable
	this->drawable.setOrigin(this->WIDTH, this->HEIGHT);
	this->drawable.setFillColor(sf::Color::Green);
	this->drawable.setSize(sf::Vector2f(this->WIDTH * 2, this->HEIGHT * 2));
}

bool Pari::damage(float damage) {
	if (this->isParrying()) return false;
	Character::damage(damage);
	return true;
}


bool Pari::jump(void) {
	bool jumped = Character::jump();
	this->spriteSheet->setLoop(this->fallLoop);
	if (jumped) this->spriteSheet->setOneShot(this->jumpLoop);
	// prevents too many jumps
	return jumped;
}


bool Pari::parry(float angle) {
	if (!this->canParry()) return false;

	this->parryAngle = angle;
	this->parryStart = std::chrono::steady_clock::now();
	return true;
}


void Pari::onCollision(Actor &a) {
	if (a.getAllegiance() == glob::Allegiance::neutral && a.shouldCollide(*this)) this->jumpCounter = 0;

	if (this->isParrying()) {
		// deflect contact object
		b2Vec2 projectileVelocity = a.getBody()->GetLinearVelocity();
		double projectileSpeed = sqrt(pow(projectileVelocity.x, 2) + pow(projectileVelocity.y, 2));
		a.getBody()->SetLinearVelocity(b2Vec2(projectileSpeed * cos(this->parryAngle),
		                                      projectileSpeed * sin(this->parryAngle)));
		a.setAllegiance(glob::Allegiance::parrot);

		// recoil Pari
		this->getBody()->SetLinearVelocity(b2Vec2(-0.05 * projectileSpeed * cos(this->parryAngle),
		                                          -0.05 * projectileSpeed * sin(this->parryAngle)));
	}
}


void Pari::draw(std::shared_ptr<sf::RenderWindow> window) {
	// old drawable
	this->drawable.setPosition(this->getBody()->GetPosition().x,
	                          -this->getBody()->GetPosition().y);
	this->drawHealthBar(window, 5);

	//window->draw(drawable);

	// set animation
	Loop newLoop;
	if (abs(this->body->GetLinearVelocity().x) > 0.1) {
		newLoop = this->runLoop; //hardcoding bad
	}
	else newLoop = this->standLoop;

	// use mirrored sprite if facing left
	this->spriteSheet->setMirrored(this->lookDir == glob::Dir::left);
	//TODO: ^this check could be removed if all characters were given a spritesheet

	if (newLoop != this->spriteSheet->getLoop()) {
		this->spriteSheet->setLoop(newLoop);
		this->spriteSheet->restart();
	}

	this->spriteSheet->getSprite().setPosition(this->body->GetPosition().x - 32 * glob::scale,
	                                          -this->body->GetPosition().y - 32 * glob::scale - 0.5);

	window->draw(this->spriteSheet->getSprite());
}
