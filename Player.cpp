#include "Player.h"

Player::Player()
{

}

Player::~Player()
{

}

int Player::getScore()
{
	return Score;
}

bool Player::IsActive()
{
	return isActive;
}

void Player::SetIsActive(bool isactive)
{
	isActive = isactive;
}

void Player::SetScore(int AddScore)
{
	Score = AddScore;
}