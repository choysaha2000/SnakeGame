#pragma once
#include <string>

const std::string RESOURCES_PATH = "Resources/";

// пюгпеьемхе щйпюмю
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 800;

// бейрнп он нях у х с
struct Vector2D { float x = 0; float y = 0; };
typedef Vector2D Position2D;

// люйяхлюкэмне йнкхвеярбн яецлемрнб глех
const int SNAKE_SEGMENTS = 50;
const float PLAYER_SIZE = 20.f;
const float APPLE_SIZE = 1.f;
const float PAUSE_LENGTH = 3.f;

// яремю. пюявер яремш он йпюъл
const float WALL_THICKNESS = PLAYER_SIZE;
const float FIELD_LEFT = WALL_THICKNESS;
const float FIELD_TOP = WALL_THICKNESS * 3;
const float FIELD_RIGHT = SCREEN_WIDTH - WALL_THICKNESS;
const float FIELD_BOTTOM = SCREEN_HEIGHT - WALL_THICKNESS;

// люяяхб дкъ опенапюгнбюмхъ ахрнбни люяйх (UP,RIGHT,DOWN,LEFT)
//255 нгмювюер медндносярхлсч йнлахмюжхч 
//хмдейяш яннрберябсчр онпъдйс оепевхякемхх.
static const uint8_t MASK_TO_TYPE[16] = {
    255, 0, 1, 255, 2, 255, 255, 255,
    3, 255, 255, 255, 255, 255, 255, 255
};

// люяйю -> хмдейя рейярспш рекю. 4-9 яннрбеярбсчр сцкнбшл х опълшл яецлемрюл
// 255 юмюкнцхвмн. 
static const uint8_t BODY_MASK_TO_TYPE[16] = {
    255, 255, 255, 4, 255, 5, 6, 255,
    255, 7, 8, 255, 9, 255, 255, 255
};
// йнкхвеярбн рнвей оняке хлемх б LEADERBOARD
const int dotsNeed = 100;
const int ScorePlayers = 10;
const int SnakeLenght = 4; // ярюмдюпр 4. 

