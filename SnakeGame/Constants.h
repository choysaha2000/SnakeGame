#pragma once
#include <string>

const std::string RESOURCES_PATH = "Resources/";

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 800;

struct Vector2D { float x = 0; float y = 0; };
typedef Vector2D Position2D;

// MAX SEGMENTS FOR SNAKE
const int SNAKE_SEGMENTS = 50;
const float PLAYER_SIZE = 20.f;
const float APPLE_SIZE = 1.f;
const float PAUSE_LENGTH = 3.f;

// WALL. 
const float WALL_THICKNESS = PLAYER_SIZE;
const float FIELD_LEFT = WALL_THICKNESS;
const float FIELD_TOP = WALL_THICKNESS * 3;
const float FIELD_RIGHT = SCREEN_WIDTH - WALL_THICKNESS;
const float FIELD_BOTTOM = SCREEN_HEIGHT - WALL_THICKNESS;

// ARRAY FOR TRASFORMING BIT MASK (UP,RIGHT, DOWN, LEFT)
// 255 -UNACCEPTABLE
static const uint8_t MASK_TO_TYPE[16] = {
    255, 0, 1, 255, 2, 255, 255, 255,
    3, 255, 255, 255, 255, 255, 255, 255
};

// MASK->BODYTEXTURE INDEX. 4-9 CORRESPOND TO ANGULAR AND STRAIGHT SEGMENTS
// 255 -UNACCEPTABLE
static const uint8_t BODY_MASK_TO_TYPE[16] = {
    255, 255, 255, 4, 255, 5, 6, 255,
    255, 7, 8, 255, 9, 255, 255, 255
};
// HOW MANY DOTS AFTER NAME IN LEADERBOARD YOU SEE IN GAME
const int dotsNeed = 100;
const int ScorePlayers = 10;
const int SnakeLenght = 4; // STANDART IN NEW GAME
