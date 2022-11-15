#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include "Player.h"
#include "grid.h"
#include <vector>
#include <iostream>
#include <iomanip>
#include <sstream>

std::vector<std::vector<sf::RectangleShape>> tileMap;

double ScreenSizeX = 800;
double ScreenSizeY = 600;

bool IsFinished = false;
bool ShouldRestart = false;

Player FirstPlayer, SecondPlayer;

grid gamegrid;

int LegalMoves[8][8] = {
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
};

int Tablegrid[8][8] = {
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 2, 1, 0, 0, 0,
    0, 0, 0, 1, 2, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
};

void SetNeededPlayersActivity(Player& fPlayer, Player& sPlayer);
void CheckInput(sf::RenderWindow& window);
void CheckForCompletion();
void AddScoreToActivePlayer(Player& fPlayer, Player& sPlayer);
bool IsCurrentPlayersTurnPosible();
void ShowWinner(sf::Font& font, sf::RenderWindow& window);
void Restartgame();

int main()
{
    sf::RenderWindow window(sf::VideoMode(ScreenSizeX, ScreenSizeY), "SFML works!");
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);
    sf::RectangleShape backgroud;
    backgroud.setSize({ 800.f, 600.f });
    backgroud.setFillColor(sf::Color::White);

    sf::Font font;
    if (!font.loadFromFile("C:\\Users\\37061\\source\\repos\\Reversi(SFML)\\Reversi(SFML)\\Rubaile.otf"))
    {
        std::cout << "Can't find the file!!!" << std::endl;
    }

    sf::Text FirstPersonsScore("Black Score: ", font, 35);
    sf::Text SecondPersonsScore("Red Score: ", font, 35);

    FirstPersonsScore.setFillColor(sf::Color::Black);
    SecondPersonsScore.setFillColor(sf::Color::Black);

    FirstPersonsScore.setPosition(500.f, 100.f);
    SecondPersonsScore.setPosition(500.f, 150.f);

    //restart ir quit
    sf::Text RestartText("Restart", font, 35);
    sf::Text QuitText("Quit", font, 35);
    RestartText.setFillColor(sf::Color::Black);
    QuitText.setFillColor(sf::Color::Black);
    RestartText.setPosition(550.f, 550);
    QuitText.setPosition(700.f, 550.f);

    FirstPlayer.SetIsActive(true);

    int CurrentPlayerIdx = 1;

    gamegrid.ClearLegalMoves(LegalMoves);
    gamegrid.CalculateLegalMoves(Tablegrid, LegalMoves, FirstPlayer);

    //creting tile map
    tileMap.resize(100, std::vector<sf::RectangleShape>());
    for (int x = 0; x < 8; x++)
    {
        tileMap[x].resize(100, sf::RectangleShape());
        for (int y = 0; y < 8; y++)
        {
            tileMap[x][y].setSize(sf::Vector2f(60, 60));
            tileMap[x][y].setFillColor(sf::Color::White);
            tileMap[x][y].setOutlineThickness(1.0f);
            tileMap[x][y].setOutlineColor(sf::Color::Black);
            tileMap[x][y].setPosition(x * 60, y * 60);
        }
    }

    bool IsMouseClicked = false;

    //update()
    while (window.isOpen() && !IsFinished)
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();

        window.draw(backgroud);

        for (int x = 0; x < 8; x++)
        {
            for (int y = 0; y < 8; y++)
            {
                window.draw(tileMap[x][y]);
            }
        }

        if (event.type == sf::Event::MouseButtonPressed && IsMouseClicked == false)
        {
            CheckInput(window);
            //restart ir quit
            float mouseX = sf::Mouse::getPosition(window).x * (window.getDefaultView().getSize().x / ScreenSizeX);
            float mouseY = sf::Mouse::getPosition(window).y * (window.getDefaultView().getSize().y / ScreenSizeY);

            if (RestartText.getGlobalBounds().contains({ mouseX , mouseY }))
            {
                ShouldRestart = true;
            }
            if (QuitText.getGlobalBounds().contains({ mouseX , mouseY }))
            {
                return 0;
            }

            IsMouseClicked = true;
        }

        for (int x = 0; x < 8; x++)
        {
            for (int y = 0; y < 8; y++)
            {
                if (Tablegrid[x][y] == 1)
                {
                    sf::CircleShape circle(30);
                    circle.setFillColor(sf::Color::Black);
                    circle.setPosition(tileMap[x][y].getPosition());
                    window.draw(circle);
                }
                else if (Tablegrid[x][y] == 2)
                {
                    sf::CircleShape circle(30);
                    circle.setFillColor(sf::Color::Red);
                    circle.setPosition(tileMap[x][y].getPosition());
                    window.draw(circle);
                }
            }
        }

        for (int x = 0; x < 8; x++)
        {
            for (int y = 0; y < 8; y++)
            {
                if (LegalMoves[x][y] == 1 && Tablegrid[x][y] == 0)
                {
                    sf::CircleShape circle(10);
                    circle.setFillColor(sf::Color(220, 220, 220));
                    sf::Vector2f tempPos = tileMap[x][y].getPosition();
                    tempPos.x += 20.0f;
                    tempPos.y += 20.f;
                    circle.setPosition(tempPos);
                    window.draw(circle);
                }
            }
        }

        //draw score
        std::ostringstream sFirstPersonsScore;
        sFirstPersonsScore << "Black Score: " << FirstPlayer.getScore();
        FirstPersonsScore.setString(sFirstPersonsScore.str());
        std::ostringstream sSecondPersonsScore;
        sSecondPersonsScore << "Red Score: " << SecondPlayer.getScore();
        SecondPersonsScore.setString(sSecondPersonsScore.str());

        //draw texts
        window.draw(FirstPersonsScore);
        window.draw(SecondPersonsScore);

        window.draw(RestartText);
        window.draw(QuitText);

        window.display();

        if (event.type == sf::Event::MouseButtonReleased && IsMouseClicked == true)
        {
            IsMouseClicked = false;

            gamegrid.ClearLegalMoves(LegalMoves);
            gamegrid.CalculateLegalMoves(Tablegrid, LegalMoves, FirstPlayer);

            if (!IsCurrentPlayersTurnPosible())
            {
                gamegrid.ClearLegalMoves(LegalMoves);
                gamegrid.CalculateLegalMoves(Tablegrid, LegalMoves, FirstPlayer);

                SetNeededPlayersActivity(FirstPlayer, SecondPlayer);
            }
        }

        CheckForCompletion();

        if (ShouldRestart)
        {
            Restartgame();
            ShouldRestart = false;
        }
    }

    if (IsFinished)
    {
        ShowWinner(font, window);
    }

    return 0;
}

void SetNeededPlayersActivity(Player& fPlayer, Player& sPlayer)
{
    if (fPlayer.IsActive())
    {
        fPlayer.SetIsActive(false);
        sPlayer.SetIsActive(true);
    }
    else if (sPlayer.IsActive())
    {
        sPlayer.SetIsActive(false);
        fPlayer.SetIsActive(true);
    }
}

void CheckInput(sf::RenderWindow& window)
{
    float mouseX = sf::Mouse::getPosition(window).x * (window.getDefaultView().getSize().x / ScreenSizeX);
    float mouseY = sf::Mouse::getPosition(window).y * (window.getDefaultView().getSize().y / ScreenSizeY);

    if (mouseX >= tileMap[0][0].getPosition().x && mouseY >= tileMap[0][0].getPosition().y)
    {
        if (mouseX <= (tileMap[7][7].getPosition().x + 50) && mouseY <= (tileMap[7][7].getPosition().y + 50))
        {
            bool CheckTileMap = true;

            for (int x = 0; x < 8 && CheckTileMap; x++)
            {
                for (int y = 0; y < 8 && CheckTileMap; y++)
                {
                    if (tileMap[x][y].getGlobalBounds().contains({ mouseX , mouseY }) && Tablegrid[x][y] == 0 && LegalMoves[x][y] == 1)
                    {
                        int i = (FirstPlayer.IsActive()) ? 1 : 2;
                        Tablegrid[x][y] = i;

                        
                        gamegrid.flipTokens(x, y, Tablegrid, FirstPlayer);

                        AddScoreToActivePlayer(FirstPlayer, SecondPlayer);

                        CheckTileMap = false;

                        mouseX = 800;
                        mouseY = 600;

                        SetNeededPlayersActivity(FirstPlayer, SecondPlayer);

                        break;
                    }
                }
            }
        }
    }
}

void CheckForCompletion()
{
    for (int x = 0; x < 8; x++)
    {
        for (int y = 0; y < 8; y++)
        {
            if (Tablegrid[x][y] == 0)
            {
                return;
            }
        }
    }

    IsFinished = true;
}

void AddScoreToActivePlayer(Player& fPlayer, Player& sPlayer)
{
    int Score = 0;
    int WholeScore = 0;

    int PlayersIdx = (fPlayer.IsActive() ? 1 : 2);

    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (Tablegrid[i][j] == PlayersIdx)
            {
                Score++;
            }

            if (Tablegrid[i][j] == 1 || Tablegrid[i][j] == 2)
            {
                WholeScore++;
            }
        }
    }

    if (fPlayer.IsActive())
    {
        fPlayer.SetScore(Score);
        sPlayer.SetScore((WholeScore - Score));
    }
    else
    {
        fPlayer.SetScore((WholeScore - Score));
        sPlayer.SetScore(Score);
    }
}

bool IsCurrentPlayersTurnPosible()
{
    int count = 0;

    for (int x = 0; x < 8; x++)
    {
        for (int y = 0; y < 8; y++)
        {
            if (LegalMoves[x][y] == 1)
            {
                count++;
            }
        }
    }

    if (count == 0)
    {
        return false;
    }
    else
    {
        return true;
    }
}

void ShowWinner(sf::Font& font, sf::RenderWindow& window)
{
    int BlackScore = 0;
    int RedScore = 0;

    for (int x = 0; x < 8; x++)
    {
        for (int y = 0; y < 8; y++)
        {
            if (Tablegrid[x][y] == 1)
            {
                BlackScore++;
            }
            else if (Tablegrid[x][y] == 2)
            {
                RedScore++;
            }
        }
    }

    std::ostringstream Winner;

    if (BlackScore > RedScore)
    {
        Winner << "Black WON!!!";
    }
    else if (BlackScore < RedScore)
    {
        Winner << "Red WON!!!";
    }
    else
    {
        Winner << "TIE!!!";
    }

    sf::Text winner(Winner.str(), font, 72);
    winner.setFillColor(sf::Color::Black);
    winner.setPosition(250, 250);

    sf::RectangleShape backgroud;
    backgroud.setSize({ 800.f, 600.f });
    backgroud.setFillColor(sf::Color::White);

    window.clear();
    window.draw(backgroud);
    window.draw(winner);
    window.display();

    system("pause");
}

void Restartgame()
{
    FirstPlayer.SetScore(2);
    FirstPlayer.SetIsActive(true);

    SecondPlayer.SetScore(2);

    gamegrid.ClearLegalMoves(LegalMoves);

    for (int x = 0; x < 8; x++)
    {
        for (int y = 0; y < 8; y++)
        {
            if ((x ==3 && y == 3) || (x == 4 && y == 4))
            {
                Tablegrid[x][y] = 2;
            }
            else if ((x == 3 && y == 4) || (x == 4 && y == 3))
            {
                Tablegrid[x][y] = 1;
            }
            else
            {
                Tablegrid[x][y] = 0;
            }
        }
    }
}