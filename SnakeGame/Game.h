#pragma once
#include "Constants.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Snake.h"
#include "UI.h"
#include <unordered_map>
#include <string>
#include <fstream>
#include <iostream> 
#include "Math.h"

const std::string SAVE_FILE = "records.txt";


enum class GameSettingBits
{
	EasyMode = 1 << 0,
	MiddleMode = 1 << 1,
	HardMode = 1 << 2,
	VeryHardMode = 1 << 3,
	HardcoreMode = 1 << 4,
};

enum class GameState
{
	None = 0,
	Menu,
	Playing,
	GameOver,
	LeaderBoard,
	PauseMenu,
	Options,
	Diffcult,
	Cin,
};

struct Game
{
	math Math;
	SnakeTextures snakeTexture;
	UI ui;

	sf::RectangleShape background;
	sf::RectangleShape wallTop;
	sf::RectangleShape wallBottom;
	sf::RectangleShape wallLeft;
	sf::RectangleShape wallRight;
	sf::Texture Tfields;
	sf::Sprite Sfields;

	uint32_t gameMode = 0;

	bool isGameFinished = false;

	float timeSinceGameFinish = 0.f;
	float gameFinishTime = 0.f;
	float stepTimer = 0.0f;
	float stepDelay = 0.3f;
	float speed = 0.f;

	std::vector<GameState> gameStateStack;

	Position2D applePosition;
	sf::Sprite AppleSprite;
	sf::Texture AppleTexture;
	int numEatenApples = 0;
	int scoreEatenApples = 0;
	int playerRecord = 0;

	sf::Music playinStateMusic;
	sf::SoundBuffer Sound1;
	sf::Sound EatAppleSound;
	sf::SoundBuffer Sound2;
	sf::Sound DeathSound;

	bool isMusicOn = true;
	bool isSoundOn = true;
	bool isGamePause = false;
	bool isSnakeDeath = false;
	bool isInputActive = false;

	// флажки для уровни сложности
	bool isEasy = false;
	bool isMiddle = false;
	bool isHard = false;
	bool isVeryHard = false;
	bool isHardcore = false;


	std::string tempPlayerName = "";
};

void InitGame(Game& game);
void UpdateGame(Game& game, float deltaTime);
void DrawGame(Game& game, sf::RenderWindow& window, std::string name);

// States
void StartGameOverState(Game& game);
void StartPlayinState(Game& game);
void UpdateGameOverState(Game& game, float deltaTime);
void SetGameSettings(Game& game);
void UpdatePlayingState(Game& game, float deltaTime);

//СТЕК МАШИНА СОСТОЯНИЙ

void PushGameState(Game& game, GameState state);
void PopGameState(Game& game);
void SwitchGameState(Game& game, GameState newState);
GameState GetCurrentGameState(const Game& game);
void SwitchGameStateInternal(Game& game, GameState oldState, GameState newState);

// СЕРИАЛИЗАЦИЯ И ДЕСЕРИАЛИЗАЦИЯ
void LoadRecords(UI& ui);

// Функция СОХРАНЕНИЯ (вызывать при обновлении рекорда)
void SaveRecords(const std::unordered_map<std::string, int>& records);


void DeInit(Game& game, UI& ui);