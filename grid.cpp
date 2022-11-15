#include "grid.h"
#include "Player.h"
#include <iostream>
#include <iomanip>

grid::grid()
{

}

grid::~grid()
{

}

void grid::ClearLegalMoves(int LegalMoves[][8])
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (LegalMoves[i][j] == 1)
            {
                LegalMoves[i][j] = 0;
            }
        }
    }
}

void grid::CalculateLegalMoves(int grid[][8], int LegalMoves[][8], Player& FirstPlayer)
{
    for (int x = 0; x < 8; x++)
    {
        for (int y = 0; y < 8; y++)
        {
            if (grid[x][y] == 0)
            {
                bool nw = AdjacentSupport(-1, -1, x, y, grid, FirstPlayer);
                bool nn = AdjacentSupport(-1, 0, x, y, grid, FirstPlayer);
                bool ne = AdjacentSupport(-1, 1, x, y, grid, FirstPlayer);

                bool ww = AdjacentSupport(0, -1, x, y, grid, FirstPlayer);
                bool ee = AdjacentSupport(0, 1, x, y, grid, FirstPlayer);

                bool sw = AdjacentSupport(1, -1, x, y, grid, FirstPlayer);
                bool ss = AdjacentSupport(1, 0, x, y, grid, FirstPlayer);
                bool se = AdjacentSupport(1, 1, x, y, grid, FirstPlayer);

                if (nw || nn || ne || ww || ee || sw || ss || se)
                {
                    LegalMoves[x][y] = 1;
                }
            }
        }
    }
}

void grid::flipTokens(int x, int y, int grid[][8], Player& FirstPlayer)
{
    FlipLine(-1, -1, x, y, grid, FirstPlayer);
    FlipLine(-1, 0, x, y, grid, FirstPlayer);
    FlipLine(-1, 1, x, y, grid, FirstPlayer);

    FlipLine(0, -1, x, y, grid, FirstPlayer);
    FlipLine(0, 1, x, y, grid, FirstPlayer);

    FlipLine(1, -1, x, y, grid, FirstPlayer);
    FlipLine(1, 0, x, y, grid, FirstPlayer);
    FlipLine(1, 1, x, y, grid, FirstPlayer);
}

bool grid::FlipLine(int dr, int dc, int r, int c, int grid[][8], Player& FirstPlayer)
{
    int OppositeTileNmber = FirstPlayer.IsActive() ? 2 : 1;
    int TileNumber = FirstPlayer.IsActive() ? 1 : 2;

    if ((r + dr < 0) || (r + dr > 7))
    {
        return false;
    }

    if ((c + dc < 0) || (c + dc > 7))
    {
        return false;
    }

    if (grid[r + dr][c + dc] == 0)
    {
        return false;
    }

    if (grid[r + dr][c + dc] == TileNumber)
    {
        return true;
    }
    else
    {
        if (FlipLine(dr, dc, r + dr, c + dc, grid, FirstPlayer))
        {
            grid[r + dr][c + dc] = TileNumber;
            return true;
        }
        else
        {
            return false;
        }
    }
}

bool grid::AdjacentSupport(int dr, int dc, int r, int c, int grid[][8], Player& FirstPlayer)
{
    int OppositeTileNmber = FirstPlayer.IsActive() ? 2 : 1;
    int TileNumber = FirstPlayer.IsActive() ? 1 : 2;

    std::cout << FirstPlayer.IsActive() << std::endl;

    if ((r + dr < 0) || (r + dr > 7))
    {
        return false;
    }

    if ((c + dc < 0) || (c + dc > 7))
    {
        return false;
    }

    if (grid[r + dr][c + dc] != OppositeTileNmber)
    {
        return false;
    }

    if ((r + dr + dr < 0) || (r + dr + dr > 7))
    {
        return false;
    }

    if ((c + dc + dc < 0) || (c + dc + dc > 7))
    {
        return false;
    }

    return CheckLineMatch(TileNumber, dr, dc, r + dr + dr, c + dc + dc, grid, FirstPlayer);
}

bool grid::CheckLineMatch(int TileNumber, int dr, int dc, int r, int c, int grid[][8], Player& FirstPlayer)
{
    int OppositeTileNmber = FirstPlayer.IsActive() ? 2 : 1;

    if (grid[r][c] == TileNumber)
    {
        return true;
    }

    if (grid[r][c] == 0)
    {
        return false;
    }

    if ((r + dr < 0) || (r + dr > 7))
    {
        return false;
    }

    if ((c + dc < 0) || (c + dc > 7))
    {
        return false;
    }

    return CheckLineMatch(TileNumber, dr, dc, r + dr, c + dc, grid, FirstPlayer);
}