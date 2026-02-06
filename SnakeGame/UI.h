#pragma once
#include <SFML/Graphics.hpp>
#include "Constants.h"
#include <vector>
#include <unordered_map>
#include <fstream>

struct Game;

enum class MenuItem {
	Start,
	Leaderboard,
	Diffucult,
	Options,
	Exit,
	Count
};

enum class Options
{
	Sound,
	Music,
	Back,
	Count
};

enum class Diffucult
{
	Easy,
	Easy2,
	Middle,
	Hard,
	VeryHard,
	Back,
	Count
};

enum class Cin
{
	YES,
	NO,
	Count
};

struct UI
{
	sf::Font font;


	sf::Text scoreText;
	//sf::Text controlsHintText;

	// game over
	sf::Text gameOverText;
	sf::Text gameOverScoreText;

	// pause
	//sf::Text BacktoMenuControl;

	// pause, ingame text
	sf::Text PlayerRecord;

	// menu
	sf::Text menuText;
	sf::Text startText;
	sf::Text leaderboard;
	sf::Text exitText;

	// pause
	sf::Text PauseMenuPlay;
	sf::Text PauseMenuMenu;
	sf::Text PauseMenuElement;

	// leaderboard
	sf::Text MainText;
	sf::Text BackToMenu;
	std::unordered_map<std::string, int> recordsTable;

	// menuItems
	std::vector<std::pair<sf::Text, MenuItem>> menuItems;
	int menuIndex = 0;

	// Options Items
	std::vector<std::pair<sf::Text, Options>> optionItems;
	int optionIndex = 0;
	sf::Text OptionsElem;

	// diffItems
	std::vector<std::pair<sf::Text, Diffucult>> diffItems;
	int diffIndex = 0;
	int selectedDiffIndex = 2;
	sf::Text diffElem;

	// cin 
	std::vector<std::pair<sf::Text, Cin>> cinItems;
	int cinIndex = 0;
	sf::Text cinElem;
};



void initText(UI& ui, Game& game);
void DrawPlayingUI(UI& ui, Game& game, sf::RenderWindow& window);

std::string GetLeaderboardString(const std::unordered_map<std::string, int>& records);
void InitializeLeaderBoard(UI& ui);

void UpdateLeaderboardGameOver(UI& ui, int playerScore, std::string name);
void UpdateMenuState(Game& game, float deltaTime);
void GameOVERUI(UI& ui, Game& game);
void UpdateLeaderBoard(Game& game, float deltaTime);
void DrawLeaderBoardUI(UI& ui, sf::RenderWindow& window);
void StartMenuState(UI& ui);
void DrawMenuUI(UI& ui, sf::RenderWindow& window);

void PauseState(UI& ui, Game& game);
void PauseMenuUpdate(Game& game, float deltaTime);
void DrawPause(UI& ui, sf::RenderWindow& window);

void OptionsState(UI& ui, Game& game);
void UpdateOptionsState(Game& game, float deltaTime);
void DrawOptionsUI(UI& ui, sf::RenderWindow& window);

void StartDiffState(UI& ui, Game& game);
void UpdateDiffState(Game& game, float deltaTime);
void DrawDiffUI(UI& ui, sf::RenderWindow& window);

void CinState(UI& ui, Game& game);
void UpdateCinState(Game& game, float deltaTime);
void DrawCinUI(UI& ui, Game& game, sf::RenderWindow& window);

