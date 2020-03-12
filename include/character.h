#ifndef CHARACTER_H
#define CHARACTER_H

#include <memory>
#include <iostream>
#include <list>

#include "actor.h"

/*
* The parent class for all movable characters
*/
class Character : public Actor, public std::enable_shared_from_this<Character> {
	protected:
		int maxHealth;
		float health;
		int mass;
		double maxSpeed, maxJump;


public:
		Character(double x, double y, double width, double height, double mass, double maxSpeed, int maxHealth, float maxJump);

		virtual void damage(float d);

		void heal(int healAmount);

		void setMaxSpeed(double maxSpeed) { this->maxSpeed = maxSpeed; };

		void setMass(double mass) { this->mass = mass; };

		void setHealth(double health) { this->health = health; };

		const int getHealth(void) { return this->health; };

		void setMaxHealth(double health) { this->maxHealth = health; };

		const int getMaxHealth(void) { return this->maxHealth; };

        bool jumping = true;

        float initialJump;

        void jump(void);

		void goLeft(void);

		void goRight(void);

		void update(const float &dt) override;

};

#endif
