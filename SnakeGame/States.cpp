#include "game.h"
#include "UI.h"
#include <cassert>

void PauseState(UI& ui, Game& game)
{
	ui.PauseMenuPlay.setFont(ui.font);
	ui.PauseMenuPlay.setCharacterSize(30);
	ui.PauseMenuPlay.setFillColor(sf::Color::White);
	ui.PauseMenuPlay.setString("Space to play");
	ui.PauseMenuPlay.setPosition(
		(SCREEN_WIDTH - ui.PauseMenuPlay.getGlobalBounds().width) / 2.f,
		SCREEN_HEIGHT - ui.PauseMenuPlay.getGlobalBounds().height - 50.f
	);
	ui.PauseMenuMenu.setFont(ui.font);
	ui.PauseMenuMenu.setCharacterSize(30);
	ui.PauseMenuMenu.setFillColor(sf::Color::White);
	ui.PauseMenuMenu.setString("Backspace to menu");
	ui.PauseMenuMenu.setPosition(
		(SCREEN_WIDTH - ui.PauseMenuMenu.getGlobalBounds().width) / 2.f,
		ui.PauseMenuPlay.getPosition().y - ui.PauseMenuMenu.getGlobalBounds().height - 20.f
	);

	ui.PauseMenuElement.setFont(ui.font);
	ui.PauseMenuElement.setCharacterSize(50);
	ui.PauseMenuElement.setFillColor(sf::Color::White);
	ui.PauseMenuElement.setString("Your score: " + std::to_string(game.scoreEatenApples));


	ui.PauseMenuElement.setPosition(
		(SCREEN_WIDTH - ui.PauseMenuElement.getGlobalBounds().width) / 2.f,
		100.f
	);
}
void PauseMenuUpdate(Game& game, float deltaTime)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		if (game.isSnakeDeath == true)
		{
			InitGame(game);
			SwitchGameState(game, GameState::Playing);
		}
		else { SwitchGameState(game, GameState::Playing); }

	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Backspace))
	{
		SwitchGameState(game, GameState::Menu);
	}
}
void DrawPause(UI& ui, sf::RenderWindow& window)
{
	window.draw(ui.PauseMenuMenu);
	window.draw(ui.PauseMenuPlay);
	window.draw(ui.PauseMenuElement);
}

// Cin
void CinState(UI& ui, Game& game)
{

	game.isInputActive = false;
	game.tempPlayerName = "";

	ui.cinItems.clear();
	sf::Text YES, NO;
	auto setTextParameters = [&ui](sf::Text& itemText, const std::string& title, int
		fontSize, sf::Color color = sf::Color::Transparent)
		{
			itemText.setString(title);
			itemText.setFont(ui.font);
			itemText.setCharacterSize(fontSize);
			if (color != sf::Color::Transparent)
			{
				itemText.setFillColor(color);
			}
		};
	setTextParameters(ui.cinElem, "PRESS YOUR NAME", 50, sf::Color::Red);
	ui.cinElem.setPosition(
		(SCREEN_WIDTH - ui.cinElem.getGlobalBounds().width) / 2.f,
		SCREEN_HEIGHT / 2.f - 100.f);

	setTextParameters(YES, "YES", 30, sf::Color::White);
	setTextParameters(NO, "NO", 30, sf::Color::White);

	YES.setPosition((SCREEN_WIDTH - YES.getGlobalBounds().width) / 2.f, SCREEN_HEIGHT / 2.f);
	NO.setPosition((SCREEN_WIDTH - NO.getGlobalBounds().width) / 2.f, SCREEN_HEIGHT / 2.f + 50.f);

	ui.cinItems.push_back({ YES, Cin::YES });
	ui.cinItems.push_back({ NO, Cin::NO });
	ui.cinIndex = 0;




}
void UpdateCinState(Game& game, float deltaTime)
{

	static float inputTimer = 0.0f;
	inputTimer += deltaTime;


	if (game.isInputActive)
	{

		if (inputTimer > 0.2f && sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
		{
			std::string finalName = game.tempPlayerName;
			if (finalName.empty()) finalName = "Unknown";


			UpdateLeaderboardGameOver(game.ui, game.scoreEatenApples, finalName);
			SwitchGameState(game, GameState::PauseMenu);
			inputTimer = 0.0f;
		}
		return;
	}


	if (inputTimer < 0.2f) return;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
		game.ui.cinIndex++;
		if (game.ui.cinIndex >= (int)game.ui.cinItems.size()) game.ui.cinIndex = 0;
		inputTimer = 0.0f;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
		game.ui.cinIndex--;
		if (game.ui.cinIndex < 0) game.ui.cinIndex = (int)game.ui.cinItems.size() - 1;
		inputTimer = 0.0f;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
		Cin selectedItem = game.ui.cinItems[game.ui.cinIndex].second;

		switch (selectedItem) {
		case Cin::YES:

			game.isInputActive = true;
			game.tempPlayerName = "";
			break;

		case Cin::NO:

			UpdateLeaderboardGameOver(game.ui, game.scoreEatenApples, "Player");
			SwitchGameState(game, GameState::PauseMenu);
			break;
		}
		inputTimer = 0.0f;
	}
}
void DrawCinUI(UI& ui, Game& game, sf::RenderWindow& window)
{
	if (game.isInputActive)
	{
		// FIELD OF ENTER NAME
		ui.cinElem.setString("ENTER NAME: " + game.tempPlayerName + "_");
		ui.cinElem.setFillColor(sf::Color::White);

		// SET POSITION ON CENTER
		ui.cinElem.setPosition(
			(SCREEN_WIDTH - ui.cinElem.getGlobalBounds().width) / 2.f,
			SCREEN_HEIGHT / 2.f
		);
		window.draw(ui.cinElem);
	}
	else
	{

		ui.cinElem.setString("PRESS YOUR NAME");
		ui.cinElem.setFillColor(sf::Color::Red);
		ui.cinElem.setPosition(
			(SCREEN_WIDTH - ui.cinElem.getGlobalBounds().width) / 2.f,
			SCREEN_HEIGHT / 2.f - 100.f);

		window.draw(ui.cinElem);

		for (int i = 0; i < ui.cinItems.size(); ++i) {
			sf::Text& text = ui.cinItems[i].first;
			if (i == ui.cinIndex) text.setFillColor(sf::Color::Yellow);
			else text.setFillColor(sf::Color::White);
			window.draw(text);
		}
	}
}

// Options
void OptionsState(UI& ui, Game& game)
{
	ui.optionItems.clear();
	sf::Text tSound, tMusic, tBack;
	auto setTextParameters = [&ui](sf::Text& itemText, const std::string& title, int
		fontSize, sf::Color color = sf::Color::Transparent)
		{
			itemText.setString(title);
			itemText.setFont(ui.font);
			itemText.setCharacterSize(fontSize);
			if (color != sf::Color::Transparent)
			{
				itemText.setFillColor(color);
			}
		};
	setTextParameters(ui.OptionsElem, "OPTIONS", 50, sf::Color::Red);
	ui.menuText.setPosition(
		(SCREEN_WIDTH - ui.menuText.getGlobalBounds().width) / 2.f,
		SCREEN_HEIGHT / 2.f - 100.f);

	setTextParameters(tSound, "SOUND", 30, sf::Color::White);
	setTextParameters(tMusic, "MUSIC", 30, sf::Color::White);
	setTextParameters(tBack, "BACK", 30, sf::Color::White);

	tSound.setPosition((SCREEN_WIDTH - tSound.getGlobalBounds().width) / 2.f, SCREEN_HEIGHT / 2.f);
	tMusic.setPosition((SCREEN_WIDTH - tMusic.getGlobalBounds().width) / 2.f, SCREEN_HEIGHT / 2.f + 50.f);
	tBack.setPosition((SCREEN_WIDTH - tBack.getGlobalBounds().width) / 2.f, SCREEN_HEIGHT / 2.f + 100.f);

	ui.optionItems.push_back({ tSound, Options::Sound });
	ui.optionItems.push_back({ tMusic, Options::Music });
	ui.optionItems.push_back({ tBack, Options::Back });

	ui.optionIndex = 0;
}
void UpdateOptionsState(Game& game, float deltaTime)
{
	static float inputTimer = 0.0f;
	inputTimer += deltaTime;
	if (inputTimer < 0.2f) return;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
		game.ui.optionIndex++;
		if (game.ui.optionIndex >= (int)game.ui.menuItems.size())
			game.ui.optionIndex = 0;
		inputTimer = 0.0f;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
		game.ui.optionIndex--;
		if (game.ui.optionIndex < 0)
			game.ui.optionIndex = (int)game.ui.menuItems.size() - 1;
		inputTimer = 0.0f;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {

		Options selectedItem = game.ui.optionItems[game.ui.optionIndex].second;

		switch (selectedItem) {
		case Options::Sound:
			game.isSoundOn = !game.isSoundOn;
			break;
		case Options::Music:
			game.isMusicOn = !game.isMusicOn;
			if (game.isMusicOn) game.playinStateMusic.play();
			else game.playinStateMusic.stop();
			break;
		case Options::Back:
			SwitchGameState(game, GameState::Menu);
			break;
		}
		inputTimer = 0.0f;
	}
}
void DrawOptionsUI(UI& ui, sf::RenderWindow& window)
{
	window.draw(ui.OptionsElem);

	for (int i = 0; i < ui.optionItems.size(); ++i) {
		sf::Text& text = ui.optionItems[i].first;


		if (i == ui.optionIndex) {
			text.setFillColor(sf::Color::Yellow);
		}
		else {
			text.setFillColor(sf::Color::White);
		}
		window.draw(text);
	}
}

// Menu 
void StartMenuState(UI& ui)
{
	ui.menuItems.clear();

	sf::Text tStart, tDiff, tOptions, tLeader, tExit;

	auto setTextParameters = [&ui](sf::Text& itemText, const std::string& title, int
		fontSize, sf::Color color = sf::Color::Transparent)
		{
			itemText.setString(title);
			itemText.setFont(ui.font);
			itemText.setCharacterSize(fontSize);
			if (color != sf::Color::Transparent)
			{
				itemText.setFillColor(color);
			}
		};

	setTextParameters(ui.menuText, "SNAKE GAME", 50, sf::Color::Red);
	ui.menuText.setPosition(
		(SCREEN_WIDTH - ui.menuText.getGlobalBounds().width) / 2.f,
		SCREEN_HEIGHT / 2.f - 100.f);

	setTextParameters(tStart, "START", 30, sf::Color::White);
	setTextParameters(tDiff, "DIFFUCULT", 30, sf::Color::White);
	setTextParameters(tOptions, "OPTION", 30, sf::Color::White);
	setTextParameters(tLeader, "LEADERBOARD", 30, sf::Color::White);
	setTextParameters(tExit, "EXIT", 30, sf::Color::White);


	tStart.setPosition((SCREEN_WIDTH - tStart.getGlobalBounds().width) / 2.f, SCREEN_HEIGHT / 2.f);
	tDiff.setPosition((SCREEN_WIDTH - tDiff.getGlobalBounds().width) / 2.f, SCREEN_HEIGHT / 2.f + 50.f);
	tOptions.setPosition((SCREEN_WIDTH - tOptions.getGlobalBounds().width) / 2.f, SCREEN_HEIGHT / 2.f + 100.f);
	tLeader.setPosition((SCREEN_WIDTH - tLeader.getGlobalBounds().width) / 2.f, SCREEN_HEIGHT / 2.f + 150.f);
	tExit.setPosition((SCREEN_WIDTH - tExit.getGlobalBounds().width) / 2.f, SCREEN_HEIGHT / 2.f + 200.f);

	ui.menuItems.push_back({ tStart, MenuItem::Start });
	ui.menuItems.push_back({ tDiff, MenuItem::Diffucult });
	ui.menuItems.push_back({ tOptions, MenuItem::Options });
	ui.menuItems.push_back({ tLeader, MenuItem::Leaderboard });
	ui.menuItems.push_back({ tExit, MenuItem::Exit });

	ui.menuIndex = 0; // START INDEX IN ZERO ELEMENT 

}
void UpdateMenuState(Game& game, float deltaTime)
{

	static float inputTimer = 0.0f;
	inputTimer += deltaTime;
	if (inputTimer < 0.2f) return;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
		game.ui.menuIndex++;
		if (game.ui.menuIndex >= (int)game.ui.menuItems.size())
			game.ui.menuIndex = 0;
		inputTimer = 0.0f;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
		game.ui.menuIndex--;
		if (game.ui.menuIndex < 0)
			game.ui.menuIndex = (int)game.ui.menuItems.size() - 1;
		inputTimer = 0.0f;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {

		MenuItem selectedItem = game.ui.menuItems[game.ui.menuIndex].second;

		switch (selectedItem) {
		case MenuItem::Start:
			InitGame(game);
			SwitchGameState(game, GameState::Playing);
			break;
		case MenuItem::Leaderboard:
			SwitchGameState(game, GameState::LeaderBoard);
			break;
		case MenuItem::Options:
			SwitchGameState(game, GameState::Options);
			break;
		case MenuItem::Diffucult:
			SwitchGameState(game, GameState::Diffcult);
			break;
		case MenuItem::Exit:
			exit(0);
			break;
		}
		inputTimer = 0.0f;
	}
}
void DrawMenuUI(UI& ui, sf::RenderWindow& window)
{
	window.draw(ui.menuText);

	for (int i = 0; i < ui.menuItems.size(); ++i) {
		sf::Text& text = ui.menuItems[i].first;


		if (i == ui.menuIndex) {
			text.setFillColor(sf::Color::Yellow);
		}
		else {
			text.setFillColor(sf::Color::White);
		}

		window.draw(text);
	}
}


void StartDiffState(UI& ui, Game& game)
{
	ui.diffItems.clear();

	sf::Text  tEasy, tEasy2, tMiddle, tHard, tVeryHard, tBack;

	auto setTextParameters = [&ui](sf::Text& itemText, const std::string& title, int
		fontSize, sf::Color color = sf::Color::Transparent)
		{
			itemText.setString(title);
			itemText.setFont(ui.font);
			itemText.setCharacterSize(fontSize);
			if (color != sf::Color::Transparent)
			{
				itemText.setFillColor(color);
			}
		};

	setTextParameters(ui.diffElem, "DIFFUCULT CHANGE", 50, sf::Color::Red);
	ui.menuText.setPosition(
		(SCREEN_WIDTH - ui.diffElem.getGlobalBounds().width) / 2.f,
		SCREEN_HEIGHT / 2.f - 100.f);

	setTextParameters(tEasy, "EASY", 30, sf::Color::White);
	setTextParameters(tEasy2, "MORE HARD THAN EASY", 30, sf::Color::White);
	setTextParameters(tMiddle, "MIDDLE", 30, sf::Color::White);
	setTextParameters(tHard, "HARD", 30, sf::Color::White);
	setTextParameters(tVeryHard, "VERY HARD", 30, sf::Color::White);
	setTextParameters(tBack, "BACK", 30, sf::Color::White);

	tEasy.setPosition((SCREEN_WIDTH - tEasy.getGlobalBounds().width) / 2.f, SCREEN_HEIGHT / 2.f);
	tEasy2.setPosition((SCREEN_WIDTH - tEasy2.getGlobalBounds().width) / 2.f, SCREEN_HEIGHT / 2.f + 50.f);
	tMiddle.setPosition((SCREEN_WIDTH - tMiddle.getGlobalBounds().width) / 2.f, SCREEN_HEIGHT / 2.f + 100.f);
	tHard.setPosition((SCREEN_WIDTH - tHard.getGlobalBounds().width) / 2.f, SCREEN_HEIGHT / 2.f + 150.f);
	tVeryHard.setPosition((SCREEN_WIDTH - tVeryHard.getGlobalBounds().width) / 2.f, SCREEN_HEIGHT / 2.f + 200.f);
	tBack.setPosition((SCREEN_WIDTH - tBack.getGlobalBounds().width) / 2.f, SCREEN_HEIGHT / 2.f + 300.f);

	ui.diffItems.push_back({ tEasy, Diffucult::Easy });
	ui.diffItems.push_back({ tEasy2, Diffucult::Easy2 });
	ui.diffItems.push_back({ tMiddle, Diffucult::Middle });
	ui.diffItems.push_back({ tHard, Diffucult::Hard });
	ui.diffItems.push_back({ tVeryHard, Diffucult::VeryHard });
	ui.diffItems.push_back({ tBack, Diffucult::Back });

	if (game.gameMode & (uint32_t)GameSettingBits::EasyMode) ui.selectedDiffIndex = 0;
	else if (game.gameMode & (uint32_t)GameSettingBits::MiddleMode) ui.selectedDiffIndex = 1;
	else if (game.gameMode & (uint32_t)GameSettingBits::HardMode) ui.selectedDiffIndex = 2;
	else if (game.gameMode & (uint32_t)GameSettingBits::VeryHardMode) ui.selectedDiffIndex = 3;
	else if (game.gameMode & (uint32_t)GameSettingBits::HardcoreMode) ui.selectedDiffIndex = 4;

	ui.diffIndex = 0;
}
void UpdateDiffState(Game& game, float deltaTime)
{
	static float inputTimer = 0.0f;
	inputTimer += deltaTime;
	if (inputTimer < 0.2f) return;

	bool anyKeyPressed = true;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
		game.ui.diffIndex++;
		if (game.ui.diffIndex >= (int)game.ui.diffItems.size())
			game.ui.diffIndex = 0;

		anyKeyPressed = true;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
		game.ui.diffIndex--;
		if (game.ui.diffIndex < 0)
			game.ui.diffIndex = (int)game.ui.diffItems.size() - 1;
		anyKeyPressed = true;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
	{
		Diffucult selectedItem = game.ui.diffItems[game.ui.diffIndex].second;


		if (selectedItem == Diffucult::Back)
		{
			SwitchGameState(game, GameState::Menu);
		}
		else
		{

			game.ui.selectedDiffIndex = game.ui.diffIndex;


			game.gameMode = 0;

			switch (selectedItem)
			{
			case Diffucult::Easy: game.gameMode |= (uint32_t)GameSettingBits::EasyMode; break;
			case Diffucult::Easy2: game.gameMode |= (uint32_t)GameSettingBits::MiddleMode; break;
			case Diffucult::Middle: game.gameMode |= (uint32_t)GameSettingBits::HardMode; break;
			case Diffucult::Hard: game.gameMode |= (uint32_t)GameSettingBits::VeryHardMode; break;
			case Diffucult::VeryHard: game.gameMode |= (uint32_t)GameSettingBits::HardcoreMode; break;
			}
			SetGameSettings(game);
			anyKeyPressed = true;
		}
	}
	if (anyKeyPressed) {
		inputTimer = 0.0f;
	}

}
void DrawDiffUI(UI& ui, sf::RenderWindow& window)
{
	window.draw(ui.diffElem);

	for (int i = 0; i < ui.diffItems.size(); ++i) {
		sf::Text& text = ui.diffItems[i].first;

		if (i == ui.diffIndex) {
			text.setFillColor(sf::Color::Yellow);
		}
		else if (i == ui.selectedDiffIndex)
		{
			text.setFillColor(sf::Color::Red);
		}
		else {
			text.setFillColor(sf::Color::White);
		}
		window.draw(text);
	}
}

// LeaderBoard
void UpdateLeaderBoard(Game& game, float deltaTime)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Backspace))
	{
		SwitchGameState(game, GameState::Menu);
	}
}
void DrawLeaderBoardUI(UI& ui, sf::RenderWindow& window)
{
	window.draw(ui.BackToMenu);
}

