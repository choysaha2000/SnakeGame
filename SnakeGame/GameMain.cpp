// ©2023, XYZ School. All rights reserved.
// Authored by Aleksandr Rybalka (polterageist@gmail.com)

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Game.h"
#include "Constants.h"
#include "Snake.h"
#include <cstdlib>
#include <ctime>


int main()
{
    Game game; 

    int seed = (int)time(nullptr);
    srand(seed);

    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Snake!");

 
    InitGame(game);

    // Init game clocks
    sf::Clock gameClock;
    float lastTime = gameClock.getElapsedTime().asSeconds();
   
    game.gameFinishTime = 0.0f;


    while (window.isOpen())
    {
        float currentTime = gameClock.getElapsedTime().asSeconds();
        float deltaTime = currentTime - lastTime;
        lastTime = currentTime;
   

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::TextEntered) 
            {
                if (GetCurrentGameState(game) == GameState::Cin && game.isInputActive)
                {

                    // cin ivent
                    if (event.text.unicode < 128)
                    {
                        char enteredChar = static_cast<char>(event.text.unicode);

                        if (enteredChar == 8)
                        {
                            if (!game.tempPlayerName.empty())
                                game.tempPlayerName.pop_back();
                        }
                
                        else if (enteredChar >= 32 && enteredChar <= 126)
                        {
                            if (game.tempPlayerName.size() < 10)
                                game.tempPlayerName += enteredChar;
                        }
                    }
                }
            }
        }
        if (event.type == sf::Event::Closed)
        {
            window.close();
            break;
        }
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
        {
            window.close();
            break;
        }

        UpdateGame(game, deltaTime);

        DrawGame(game, window, game.tempPlayerName);

        window.display();
    }

    DeInit(game, game.ui);

    return 0;
}