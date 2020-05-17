#include "exit.hpp"
#include "actor.hpp"
#include "room.hpp"
#include <list>
#include <memory>

Exit::Exit(double x, double y, std::shared_ptr<Exit> destination) : Actor(x, y, 12, 64) {
	this->destination = destination;
}

Exit::Exit(double x, double y, double width, double height, std::shared_ptr<Exit> destination) : Actor(x, y, width, height) {
	this->destination = destination;
}
