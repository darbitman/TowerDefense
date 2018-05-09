#pragma once
#include "creature.h"



class Drone : public Creature
{
public:
  Drone();
  Drone(int, int, char, int, int, int, int, DroneType);

		virtual const Coord& get_coord() const;
		virtual char get_img();
		virtual int get_health();
		virtual int get_attack();
		virtual int get_speed();
		virtual int get_exp();
    Coord get_previous_coord() const;

    bool is_backtrack(const Coord& pos) const;

    void set_previous_coord(Coord pos);
		virtual void set_coord(int, int);
		virtual void set_coord( Coord );
		virtual void set_img(char);
		virtual void set_health( int );
		virtual void set_attack( int );
		virtual void set_speed( int );
		virtual void set_exp( int );

private:

};

