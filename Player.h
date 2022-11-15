#pragma once

class Player
{
public:
	Player();
	~Player();

	int getScore();
	bool IsActive();
	void SetIsActive(bool isactive);
	void SetScore(int AddScore);

private:
	int Score = 2;
	bool isActive = false;
};
