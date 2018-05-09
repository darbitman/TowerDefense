#include "player.h"

Player::Player()
{
	m_money = 200;
	m_lives = 5;
	m_score = 0;
}

uint32 Player::get_money() const
{
	return m_money;
}

uint32 Player::get_score() const
{
	return m_score;
}

void Player::set_money(uint32 money)
{
	m_money = money;
}

void Player::add_money(int32 money)
{
  m_money += money;
}

void Player::add_score(uint32 score)
{
  m_score += score;
}

uint32 Player::get_lives() const
{
  return m_lives;
}

void Player::remove_life()
{
	if(m_lives > 0)
		m_lives--;
}