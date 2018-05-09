#include "drone.h"


Drone::Drone(){
  this->xy.layer = 1;
  previous.layer = 1;
  this->xy.x = 0;
	this->xy.y = 0;
	this->img = (char)157;
	this->health = 10;
	this->attack = 1;
	this->speed = 2;
	this->exp = 0;
  drone_type = GOBLIN;
}

Drone::Drone(int x, int y, char img, int health, int attack, int speed, int exp, DroneType dt) {
  this->xy.layer = 1;
	this->xy.x = x;
	this->xy.y = y;
	this->img = img;
	this->health = health;
	this->attack = attack;
	this->speed = speed;
	this->exp = exp;
  drone_type = dt;
}



Coord Drone::get_previous_coord() const
{
  return previous;
}

int Drone::get_health() {
	return health;
}

int Drone::get_attack() {
	return attack;
}

int Drone::get_speed() {
	return speed;
}

int Drone::get_exp() {
	return exp;
}



void Drone::set_previous_coord(Coord pos)
{
  previous.x = pos.x;
  previous.y = pos.y;
}

void Drone::set_health( int health ){
	this->health = health;
}

void Drone::set_attack( int attack ){
	this->attack = attack;
}

void Drone::set_speed( int speed ){
	this->speed = speed;
}

void Drone::set_exp( int exp ){
	this->exp = exp;
}
