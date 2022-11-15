#pragma once
#include "Player.h"

class grid
{
public:
	grid();
	~grid();

	void ClearLegalMoves(int LegalMoves[][8]);
	void CalculateLegalMoves(int grid[][8], int LegalMoves[][8], Player& FirstPlayer);
	void flipTokens(int x, int y, int grid[][8], Player& FirstPlayer);
	bool FlipLine(int dr, int dc, int r, int c, int grid[][8], Player& FirstPlayer);
	bool AdjacentSupport(int dr, int dc, int r, int c, int grid[][8], Player& FirstPlayer);
	bool CheckLineMatch(int TileNumber, int dr, int dc, int r, int c, int grid[][8], Player& FirstPlayer);

private:
	
};
