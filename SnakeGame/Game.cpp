#include "Game.h"
#include <cassert>

void InitGame(Game& game)
{
    assert(game.AppleTexture.loadFromFile(RESOURCES_PATH + "apple.png"));

    if (game.playinStateMusic.getDuration().asSeconds() == 0) {
        if (game.playinStateMusic.openFromFile(RESOURCES_PATH + "PlayingState.ogg")) {
            game.playinStateMusic.setLoop(true);
        }
    }

    if (game.isMusicOn) {
        if (game.playinStateMusic.getStatus() != sf::Music::Playing)
            game.playinStateMusic.play();
    }
    else {
        game.playinStateMusic.stop();
    }
    assert(game.Sound1.loadFromFile(RESOURCES_PATH + "AppleEat.wav"));
    assert(game.Sound2.loadFromFile(RESOURCES_PATH + "Lose.wav"));


    game.EatAppleSound.setBuffer(game.Sound1);
    game.DeathSound.setBuffer(game.Sound2);

    game.background.setFillColor(sf::Color::Green);
    game.background.setSize(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
    game.background.setPosition(0.f, 0.f);

    game.wallTop.setFillColor(sf::Color::Yellow);
    game.wallTop.setSize(sf::Vector2f(SCREEN_WIDTH, WALL_THICKNESS * 3));
    game.wallTop.setPosition(0.f, 0.f);

    game.wallBottom.setFillColor(sf::Color::Yellow);
    game.wallBottom.setSize(sf::Vector2f(SCREEN_WIDTH, WALL_THICKNESS));
    game.wallBottom.setPosition(0.f, SCREEN_HEIGHT - WALL_THICKNESS);

    game.wallLeft.setSize({ WALL_THICKNESS, SCREEN_HEIGHT });
    game.wallLeft.setFillColor(sf::Color::Yellow);
    game.wallLeft.setPosition(0.f, 0.f);

    game.wallRight.setSize({ WALL_THICKNESS, SCREEN_HEIGHT });
    game.wallRight.setFillColor(sf::Color::Yellow);
    game.wallRight.setPosition(SCREEN_WIDTH - WALL_THICKNESS, 0.f);

    InitSnake(game);
    initText(game.ui, game);
    LoadRecords(game.ui);
    SetGameSettings(game);
    PushGameState(game, GameState::Menu);
}

void StartPlayinState(Game& game)
{
    game.Math.snakeRect.size = { PLAYER_SIZE, PLAYER_SIZE };

    if (!game.isGamePause) {
        game.applePosition = GetRandomPositionInField();
    }
    game.Math.appleCircle.position = game.applePosition;
    game.Math.appleCircle.radius = APPLE_SIZE / 2.f;
    game.AppleSprite.setTexture(game.AppleTexture);
    SetSpriteRelativeOrigin(game.AppleSprite, 0.5f, 0.5f);
    game.AppleSprite.setPosition(game.applePosition.x, game.applePosition.y);
    game.ui.scoreText.setString("Apples eaten: " + std::to_string(game.scoreEatenApples));

}

void UpdatePlayingState(Game& game, float deltaTime)
{
    if (game.isGameFinished)
    {
        UpdateGameOverState(game, deltaTime);
        return; // дальше не идет, пока не закончится UpdateGameOverState. (3 секунды)
    }

    game.stepTimer += deltaTime;
    if (game.stepTimer >= game.stepDelay)
    {
        game.stepTimer -= game.stepDelay;


        UpdateSnake(deltaTime, game);

        game.Math.snakeRect.position = {
            snakeSegments[0].snakePosition.x - PLAYER_SIZE / 2.f,
            snakeSegments[0].snakePosition.y - PLAYER_SIZE / 2.f
        };
        game.Math.snakeRect.size = { PLAYER_SIZE, PLAYER_SIZE };

        game.Math.appleCircle.position = game.applePosition;
        game.Math.appleCircle.radius = APPLE_SIZE / 2.f;

        if (DoShapesCollide(game.Math.snakeRect, game.Math.appleCircle))
        {
            if (game.isSoundOn)
            {
                game.EatAppleSound.play();
            }
            snakeSegments.emplace_back(snakeSegments.back());


            if (game.isEasy == true)
            {
                ++game.numEatenApples;
                ++game.scoreEatenApples;
            }

            else if (game.isMiddle == true)
            {
                game.numEatenApples += 2;
                game.scoreEatenApples += 2;
            }

            else if (game.isHard == true)
            {
                game.numEatenApples += 3;
                game.scoreEatenApples += 3;
            }

            else if (game.isVeryHard == true)
            {
                game.numEatenApples += 4;
                game.scoreEatenApples += 4;
            }

            else if (game.isHardcore == true)
            {
                game.numEatenApples += 4;
                game.scoreEatenApples += 4;
            }
            else
            {
                game.numEatenApples += 1;
                game.scoreEatenApples += 1;

            }

            if (game.scoreEatenApples > game.playerRecord)
            {
                game.playerRecord = game.scoreEatenApples;
                ++game.playerRecord;
            }
            game.ui.scoreText.setString("Apples eaten: " + std::to_string(game.scoreEatenApples));
            game.applePosition = GetRandomPositionInField();
            game.AppleSprite.setPosition(game.applePosition.x, game.applePosition.y);
        }

        if (snakeSegments[0].snakePosition.x - PLAYER_SIZE / 2.f < FIELD_LEFT ||
            snakeSegments[0].snakePosition.x + PLAYER_SIZE / 2.f > FIELD_RIGHT ||
            snakeSegments[0].snakePosition.y - PLAYER_SIZE / 2.f < FIELD_TOP ||
            snakeSegments[0].snakePosition.y + PLAYER_SIZE / 2.f > FIELD_BOTTOM)
        {
            if (game.isSoundOn)
            {
                game.DeathSound.play();
            }
            SwitchGameState(game, GameState::GameOver);

        }

        for (size_t i = 1; i < snakeSegments.size(); ++i)
        {
            if (snakeSegments[0].snakePosition.x == snakeSegments[i].snakePosition.x &&
                snakeSegments[0].snakePosition.y == snakeSegments[i].snakePosition.y)
            {
                game.DeathSound.play();
                SwitchGameState(game, GameState::GameOver);
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Pause))
        {
            game.isGamePause = true;
            SwitchGameState(game, GameState::PauseMenu);
        }

    }
}

// управление и активная логика тика. 
void UpdateGame(Game& game, float deltaTime)
{
    GameState gameState = GetCurrentGameState(game);
    switch (gameState)
    {
    case GameState::Playing:
        UpdatePlayingState(game, deltaTime);
        break;
    case GameState::GameOver:
        UpdateGameOverState(game, deltaTime);
        break;
    case GameState::Menu:
        UpdateMenuState(game, deltaTime);
        break;
    case GameState::LeaderBoard:
        UpdateLeaderBoard(game, deltaTime);
        break;
    case GameState::PauseMenu:
        PauseMenuUpdate(game, deltaTime);
        break;
    case GameState::Options:
        UpdateOptionsState(game, deltaTime);
        break;
    case GameState::Diffcult:
        UpdateDiffState(game, deltaTime);
        break;
    case GameState::Cin:
        UpdateCinState(game, deltaTime);
        break;
    }
}

void StartGameOverState(Game& game)
{
    game.playinStateMusic.stop();
    game.isSnakeDeath = true;
    game.isGameFinished = true;
    game.timeSinceGameFinish = 0.f;
}

void UpdateGameOverState(Game& game, float deltaTime)
{
    game.playinStateMusic.stop();
    if (game.timeSinceGameFinish <= PAUSE_LENGTH)
    {
        game.timeSinceGameFinish += deltaTime;
        game.background.setFillColor(sf::Color::Red);
    }
    else
    {
        game.background.setFillColor(sf::Color::Green);

        if (game.scoreEatenApples >= 2)
        {
            SwitchGameState(game, GameState::Cin);
        }
        else
        {
            SwitchGameState(game, GameState::PauseMenu);
        }
    }
}

void DrawGame(Game& game, sf::RenderWindow& window, std::string name)
{
    window.clear();

    window.draw(game.background);

    window.draw(game.wallBottom);
    window.draw(game.wallLeft);
    window.draw(game.wallRight);
    window.draw(game.wallTop);

    GameState state = GetCurrentGameState(game);

    if (state == GameState::Playing)
    {
        DrawSnake(window);
        window.draw(game.AppleSprite);
        DrawPlayingUI(game.ui, game, window);
    }

    else if (state == GameState::GameOver)
    {
        window.draw(game.ui.gameOverText);
    }

    else if (state == GameState::Menu)
    {
        DrawMenuUI(game.ui, window);
    }

    else if (state == GameState::LeaderBoard)
    {
        UpdateLeaderboardGameOver(game.ui, game.playerRecord, name);
        window.draw(game.ui.gameOverText);
        window.draw(game.ui.gameOverScoreText);
        DrawLeaderBoardUI(game.ui, window);
    }

    else if (state == GameState::PauseMenu)
    {
        UpdateLeaderboardGameOver(game.ui, game.playerRecord, name);
        window.draw(game.ui.gameOverText);
        window.draw(game.ui.gameOverScoreText);
        DrawPause(game.ui, window);
    }
    else if (state == GameState::Options)
    {
        DrawOptionsUI(game.ui, window);
    }

    else if (state == GameState::Diffcult)
    {
        DrawDiffUI(game.ui, window);
    }

    else if (state == GameState::Cin)
    {
        DrawCinUI(game.ui, game, window);
    }

}

// GAME SETTINGS

void SetGameSettings(Game& game)
{
    game.numEatenApples = SnakeLenght;
    game.isGameFinished = false;
    game.isSnakeDeath = false;
    game.timeSinceGameFinish = 0.f;
    game.stepTimer = 0.f;
    game.scoreEatenApples = 0;

    game.isEasy = game.gameMode & static_cast <uint32_t> (GameSettingBits::EasyMode);
    game.isMiddle = game.gameMode & static_cast <uint32_t> (GameSettingBits::MiddleMode);
    game.isHard = game.gameMode & static_cast <uint32_t> (GameSettingBits::HardMode);
    game.isVeryHard = game.gameMode & static_cast <uint32_t> (GameSettingBits::VeryHardMode);
    game.isHardcore = game.gameMode & static_cast <uint32_t> (GameSettingBits::HardcoreMode);

    if (game.isEasy == true)
    {
        game.speed = 15.f;
    }

    else if (game.isMiddle == true)
    {
        game.speed = 20.f;
    }

    else if (game.isHard == true)
    {
        game.speed = 30.f;
    }

    else if (game.isVeryHard == true)
    {
        game.speed = 50.f;
    }

    else if (game.isHardcore == true)
    {
        game.speed = 65.f;
    }
    else
    {
        game.speed = 20.f;
    }

    // TRANSFORM SNAKE SPEED AFTER DIFFUCULT CHANGES (FRAME-LATENCY. EASY 200 MS, HARD 100 MS AND ETC) 
    game.stepDelay = std::max(0.05f, 0.3f - (game.speed * 0.01f));
}

////////////////// LIFO

void PushGameState(Game& game, GameState state)
{
    GameState oldState = GetCurrentGameState(game);
    SwitchGameStateInternal(game, oldState, state);
    game.gameStateStack.push_back(state);
}

void PopGameState(Game& game)
{
    if (game.gameStateStack.empty()) return;

    GameState oldState = game.gameStateStack.back();
    game.gameStateStack.pop_back();
    GameState newState = GetCurrentGameState(game);

    SwitchGameStateInternal(game, oldState, newState);

}

void SwitchGameState(Game& game, GameState newState)
{
    if (!game.gameStateStack.empty())
    {
        GameState oldState = game.gameStateStack.back();
        game.gameStateStack.pop_back();
        game.gameStateStack.push_back(newState);
        SwitchGameStateInternal(game, oldState, newState);
    }

    else
    {
        PushGameState(game, newState);
    }
}

GameState GetCurrentGameState(const Game& game)
{
    return game.gameStateStack.empty() ? GameState::None :
        game.gameStateStack.back();
}

void SwitchGameStateInternal(Game& game, GameState oldState, GameState newState)
{
    switch (oldState)
    {
    case GameState::Playing: break;
    case GameState::GameOver: break;
    case GameState::Menu: break;
    case GameState::LeaderBoard: break;
    case GameState::PauseMenu: break;
    case GameState::Diffcult: break;
    case GameState::Options: break;
    case GameState::Cin: break;

    }

    switch (newState)
    {
    case GameState::Playing: StartPlayinState(game); break;
    case GameState::GameOver: GameOVERUI(game.ui, game); break;
    case GameState::Menu: StartMenuState(game.ui); break;
    case GameState::LeaderBoard: initText(game.ui, game); break;
    case GameState::PauseMenu: PauseState(game.ui, game); break;
    case GameState::Diffcult: StartDiffState(game.ui, game); break;
    case GameState::Options: OptionsState(game.ui, game); break;
    case GameState::Cin: CinState(game.ui, game); break;
    }
}

// СЕРИАЛИЗАЦИЯ И ДЕСЕРИАЛИЗАЦИЯ
void LoadRecords(UI& ui)
{
    std::ifstream file(SAVE_FILE);
    if (file.is_open())
    {
        ui.recordsTable.clear();
        std::string name;
        int score;
        while (file >> name >> score) //
        {
            ui.recordsTable[name] = score;
        }
        file.close();
        std::cout << "Records loaded!" << std::endl;
    }
    else
    {
        std::cout << "Save file not found, creating new..." << std::endl;
        InitializeLeaderBoard(ui); //
    }
}

void SaveRecords(const std::unordered_map<std::string, int>& records)
{
    std::ofstream file(SAVE_FILE);
    if (file.is_open())
    {
        for (const auto& entry : records)
        {
            file << entry.first << " " << entry.second << std::endl;
        }
        file.close();
        std::cout << "Records saved!" << std::endl;
    }
}


void DeInit(Game& game, UI& ui)
{
    snakeSegments.clear();
    ui.menuItems.clear();
    ui.optionItems.clear();
    ui.diffItems.clear();
}

