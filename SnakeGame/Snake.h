#pragma once
#include <SFML/Graphics.hpp>
#include "Constants.h"
#include <vector>

struct Game;

enum class PlayerDirection
{
	Right = 0,
	Up = 1,
	Left = 2,
	Down = 3
};

enum class SnakePart
{
	Head,
	Body,
	Tail
};

// MASK FOR SEGMENTS CONNECTION 
// USED TO RIGHT TEXTURE FOR SEGMENTS
enum ConnectionMask : uint8_t
{
	NONE = 0b0000,
	UP = 0b0001,
	RIGHT = 0b0010,
	DOWN = 0b0100,
	LEFT = 0b1000,
	VERTICAL = UP | DOWN,
	HORIZONTAL = RIGHT | LEFT
};

struct Snake
{
	Position2D snakePosition;
	PlayerDirection snakeDirection{};
	sf::Sprite snakeSprite;
	uint8_t connectionMask = 0; // маска для связей
	bool needsUpdate = true; // Флаг обновления текстуры


};

struct SnakeTextures 
{
	sf::Texture HeadUp;
	sf::Texture HeadRight;
	sf::Texture HeadDown;
	sf::Texture HeadLeft;

	sf::Texture BodyVertical;
	sf::Texture BodyHorizontal;
	sf::Texture BodyTopRight;    
	sf::Texture BodyBottomRight; 
	sf::Texture BodyBottomLeft;  
	sf::Texture BodyTopLeft;     

	sf::Texture TailUp;
	sf::Texture TailRight;
	sf::Texture TailDown;
	sf::Texture TailLeft;
};

extern std::vector<Snake> snakeSegments;


void InitSnake(Game& game);
void UpdateSnake(float deltaTime, Game& game);
void DrawSnake(sf::RenderWindow& window);
void UpdateSnakeConnection();
uint8_t CalculateConnectionMask(size_t index);
sf::Texture* GetTextureForMask(uint8_t mask, SnakePart part, Game& game);
float GetRotationForMask(uint8_t mask, SnakePart part);
void SpawnSnake(Game& game);