#include "Snake.h"
#include "Game.h"
#include <cassert>
#include <cmath>

std::vector<Snake> snakeSegments;


namespace {
    bool LoadSnakeTextures(SnakeTextures& textures)
    {
        assert(textures.HeadUp.loadFromFile(RESOURCES_PATH + "head_up.png"));
        assert(textures.HeadRight.loadFromFile(RESOURCES_PATH + "head_right.png"));
        assert(textures.HeadDown.loadFromFile(RESOURCES_PATH + "head_down.png"));
        assert(textures.HeadLeft.loadFromFile(RESOURCES_PATH + "head_left.png"));
        assert(textures.BodyVertical.loadFromFile(RESOURCES_PATH + "body_vertical.png"));
        assert(textures.BodyHorizontal.loadFromFile(RESOURCES_PATH + "body_horizontal.png"));
        assert(textures.BodyTopRight.loadFromFile(RESOURCES_PATH + "body_topright.png"));
        assert(textures.BodyBottomRight.loadFromFile(RESOURCES_PATH + "body_bottomright.png"));
        assert(textures.BodyBottomLeft.loadFromFile(RESOURCES_PATH + "body_bottomleft.png"));
        assert(textures.BodyTopLeft.loadFromFile(RESOURCES_PATH + "body_topleft.png"));
        assert(textures.TailUp.loadFromFile(RESOURCES_PATH + "tail_up.png"));
        assert(textures.TailRight.loadFromFile(RESOURCES_PATH + "tail_right.png"));
        assert(textures.TailDown.loadFromFile(RESOURCES_PATH + "tail_down.png"));
        assert(textures.TailLeft.loadFromFile(RESOURCES_PATH + "tail_left.png"));
        return true;
    }
}

uint8_t CalculateConnectionMask(size_t index)
{
    uint8_t mask = 0;

    if (index > 0) {
        const auto& prev = snakeSegments[index - 1];
        const auto& curr = snakeSegments[index];

        if (prev.snakePosition.x < curr.snakePosition.x) mask |= LEFT;
        else if (prev.snakePosition.x > curr.snakePosition.x) mask |= RIGHT;
        else if (prev.snakePosition.y < curr.snakePosition.y) mask |= UP;
        else if (prev.snakePosition.y > curr.snakePosition.y) mask |= DOWN;
    }

    if (index < snakeSegments.size() - 1) {
        const auto& next = snakeSegments[index + 1];
        const auto& curr = snakeSegments[index];

        if (next.snakePosition.x < curr.snakePosition.x) mask |= LEFT;
        else if (next.snakePosition.x > curr.snakePosition.x) mask |= RIGHT;
        else if (next.snakePosition.y < curr.snakePosition.y) mask |= UP;
        else if (next.snakePosition.y > curr.snakePosition.y) mask |= DOWN;
    }

    return mask;
}

void UpdateSnakeConnection()
{
    for (size_t i = 0; i < snakeSegments.size(); ++i)
    {
        uint8_t newMask = CalculateConnectionMask(i);
        if (snakeSegments[i].connectionMask != newMask)
        {
            snakeSegments[i].connectionMask = newMask;
            snakeSegments[i].needsUpdate = true;
        }
    }
}

sf::Texture* GetTextureForMask(uint8_t mask, SnakePart part, Game& game) {
    switch (part) {
    case SnakePart::Head: {
        static sf::Texture* headTextures[4] = {
            &game.snakeTexture.HeadRight,
            &game.snakeTexture.HeadUp,    // 1
            &game.snakeTexture.HeadLeft,  // 2 
            &game.snakeTexture.HeadDown   // 3
        };
        return headTextures[static_cast<int>(snakeSegments[0].snakeDirection)];
    }

    case SnakePart::Body: {
        uint8_t type = BODY_MASK_TO_TYPE[mask & 0x0F];
        if (type == 255) type = 5; // body vertical

        static sf::Texture* bodyTextures[6] = {
            &game.snakeTexture.BodyTopRight,
            &game.snakeTexture.BodyVertical,
            &game.snakeTexture.BodyBottomRight,
            &game.snakeTexture.BodyTopLeft,
            &game.snakeTexture.BodyHorizontal,
            &game.snakeTexture.BodyBottomLeft
        };
        return type >= 4 ? bodyTextures[type - 4] : &game.snakeTexture.BodyVertical;
    }

    case SnakePart::Tail: {
        uint8_t type = MASK_TO_TYPE[mask & 0x0F];
        if (type == 255) type = 2; // tail down

        static sf::Texture* tailTextures[4] = {
            &game.snakeTexture.TailUp,
            &game.snakeTexture.TailRight,
            &game.snakeTexture.TailDown,
            &game.snakeTexture.TailLeft
        };
        return type < 4 ? tailTextures[type] : &game.snakeTexture.TailDown;
    }
    }
    return nullptr; // default
}

float GetRotationForMask(uint8_t mask, SnakePart part)
{
    if (part == SnakePart::Tail) {
        return 180.f;
    }
    return 0.f;
}

void InitSnake(Game& game)
{
    snakeSegments.clear();
    snakeSegments.resize(SnakeLenght);

    static bool texturesLoaded = false;
    if (!texturesLoaded) {
        LoadSnakeTextures(game.snakeTexture);
        texturesLoaded = true;
    }

    float startX = SCREEN_WIDTH / 2.f;
    float startY = SCREEN_HEIGHT / 2.f;


    for (size_t i = 0; i < snakeSegments.size(); ++i) {
        snakeSegments[i].snakePosition = {
            startX - static_cast<float>(i) * PLAYER_SIZE,
            startY
        };
        snakeSegments[i].snakeDirection = PlayerDirection::Right;
        snakeSegments[i].needsUpdate = true;
    }

    UpdateSnakeConnection();

    // texture - size - origin - position
    for (size_t i = 0; i < snakeSegments.size(); ++i) {
        SnakePart part = (i == 0) ? SnakePart::Head :
            (i == snakeSegments.size() - 1) ? SnakePart::Tail :
            SnakePart::Body;

        // first set texture
        snakeSegments[i].snakeSprite.setTexture(
            *GetTextureForMask(snakeSegments[i].connectionMask, part, game),
            true
        );


        SetSpriteSize(snakeSegments[i].snakeSprite, PLAYER_SIZE, PLAYER_SIZE);

        // then set origin
        SetSpriteRelativeOrigin(snakeSegments[i].snakeSprite, 0.5f, 0.5f);

        // then rotation and position
        snakeSegments[i].snakeSprite.setRotation(
            GetRotationForMask(snakeSegments[i].connectionMask, part)
        );
        snakeSegments[i].snakeSprite.setPosition(
            snakeSegments[i].snakePosition.x,
            snakeSegments[i].snakePosition.y
        );

        snakeSegments[i].needsUpdate = false;
    }
}

void UpdateSnake(float deltaTime, Game& game) {
    if (snakeSegments.empty()) return;

    // key sets
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) &&
        snakeSegments[0].snakeDirection != PlayerDirection::Left)
        snakeSegments[0].snakeDirection = PlayerDirection::Right;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) &&
        snakeSegments[0].snakeDirection != PlayerDirection::Right)
        snakeSegments[0].snakeDirection = PlayerDirection::Left;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) &&
        snakeSegments[0].snakeDirection != PlayerDirection::Down)
        snakeSegments[0].snakeDirection = PlayerDirection::Up;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) &&
        snakeSegments[0].snakeDirection != PlayerDirection::Up)
        snakeSegments[0].snakeDirection = PlayerDirection::Down;

    // segments moving
    for (size_t i = snakeSegments.size() - 1; i > 0; --i) {
        snakeSegments[i].snakePosition = snakeSegments[i - 1].snakePosition;
    }

    // head moving
    switch (snakeSegments[0].snakeDirection) {
    case PlayerDirection::Right: snakeSegments[0].snakePosition.x += PLAYER_SIZE; break;
    case PlayerDirection::Left:  snakeSegments[0].snakePosition.x -= PLAYER_SIZE; break;
    case PlayerDirection::Up:    snakeSegments[0].snakePosition.y -= PLAYER_SIZE; break;
    case PlayerDirection::Down:  snakeSegments[0].snakePosition.y += PLAYER_SIZE; break;
    }

    // refresh position and connection
    for (size_t i = 0; i < snakeSegments.size(); ++i) {
        snakeSegments[i].snakeSprite.setPosition(
            snakeSegments[i].snakePosition.x,
            snakeSegments[i].snakePosition.y
        );
    }

    // refresh mask connection
    UpdateSnakeConnection();

    // refresh textur only for changed segments
    for (size_t i = 0; i < snakeSegments.size(); ++i) {
        if (snakeSegments[i].needsUpdate) {
            SnakePart part = (i == 0) ? SnakePart::Head :
                (i == snakeSegments.size() - 1) ? SnakePart::Tail :
                SnakePart::Body;

            snakeSegments[i].snakeSprite.setTexture(
                *GetTextureForMask(snakeSegments[i].connectionMask, part, game),
                true
            );
            snakeSegments[i].snakeSprite.setRotation(
                GetRotationForMask(snakeSegments[i].connectionMask, part)
            );
            snakeSegments[i].needsUpdate = false;
        }
    }
}

void DrawSnake(sf::RenderWindow& window) {
    for (const auto& segment : snakeSegments) {
        window.draw(segment.snakeSprite);
    }
}

void SpawnSnake(Game& game)
{
    game.Math.snakeRect.position = {
    snakeSegments[0].snakePosition.x - PLAYER_SIZE / 2.f,
    snakeSegments[0].snakePosition.y - PLAYER_SIZE / 2.f
    };
}