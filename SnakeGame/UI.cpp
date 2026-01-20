#include "game.h"
#include "UI.h"
#include <cassert>

void initText(UI& ui, Game& game)
{
	assert(ui.font.loadFromFile(RESOURCES_PATH + "\\Fonts\\stencilbtrusbyme.otf"));
	//score
	ui.scoreText.setFont(ui.font);
	ui.scoreText.setCharacterSize(20);
	ui.scoreText.setFillColor(sf::Color::Red);
	ui.scoreText.setPosition(30.f, 20.f);
	//PlayerRecordInGame
	ui.PlayerRecord.setFont(ui.font);
	ui.PlayerRecord.setCharacterSize(20);
	ui.PlayerRecord.setFillColor(sf::Color::Blue);
	ui.PlayerRecord.setPosition(250.f, 20.f);
	ui.PlayerRecord.setString("Record: " + std::to_string(game.playerRecord));
	//mainleaderboard
	ui.BackToMenu.setFont(ui.font);
	ui.BackToMenu.setCharacterSize(30);
	ui.BackToMenu.setFillColor(sf::Color::White);
	ui.BackToMenu.setPosition(20.f,
		SCREEN_HEIGHT - ui.BackToMenu.getGlobalBounds().height - 40.f);
	ui.BackToMenu.setString("Press Backspace to back to menu");
}
void GameOVERUI(UI& ui, Game& game)
{
	game.playinStateMusic.stop();
	game.isSnakeDeath = true;
	game.isGameFinished = true;
	game.timeSinceGameFinish = 0.f;

	ui.gameOverText.setString("Game Over");
	ui.gameOverText.setFont(ui.font);
	ui.gameOverText.setCharacterSize(100);
	ui.gameOverText.setFillColor(sf::Color::White);


	sf::FloatRect textRect = ui.gameOverText.getLocalBounds();

	ui.gameOverText.setOrigin(
		textRect.left + textRect.width / 2.f,
		textRect.top + textRect.height / 2.f
	);

	// Просто ставим текст в центр экрана
	ui.gameOverText.setPosition(SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f);
}
void UpdateLeaderboardGameOver (UI& ui, int playerScore, std::string name)
{

	const std::string PLAYER_NAME = name;
	ui.recordsTable[PLAYER_NAME] = playerScore;

	SaveRecords(ui.recordsTable);
	std::string leaderBoardText = GetLeaderboardString(ui.recordsTable);
	ui.gameOverText.setString(leaderBoardText);
	ui.gameOverText.setCharacterSize(18);
	ui.gameOverText.setFont(ui.font);
	ui.gameOverText.setOrigin(0.f, 0.f); 
	ui.gameOverText.setPosition(50.f, 100.f); 

}
void DrawPlayingUI(UI& ui, Game& game, sf::RenderWindow& window)
{

	

	window.draw(ui.scoreText);
	window.draw(ui.PlayerRecord);
}
std::string GetLeaderboardString(const std::unordered_map<std::string, int>& records)
{

	std::multimap<int, std::string> sortedRecords;
	for (const auto& item : records)
	{
		sortedRecords.insert({ item.second, item.first });
	}

	std::string result = "===== LEADERBOARD =====\n";

	int position = 1;

	for (auto it = sortedRecords.rbegin(); it != sortedRecords.rend() && position <= 5; ++it, ++position)
	{
		int score = it->first;
		const std::string& name = it->second;
		std::string line = std::to_string(position) + ". " + name;
		auto dotsNeeded = dotsNeed - line.length();
		for (int i = 0; i < dotsNeeded; ++i)
			line += ".";
		line += " " + std::to_string(score);

		result += line + "\n";
	}

	result += "=======================\n";
	return result;
}
void InitializeLeaderBoard(UI& ui)
{
	ui.recordsTable.clear();
	std::vector<std::string> names = {
		"Alice", "Bob", "Carol", "Dave", "Eve"
	};

	for (const auto& name : names)
	{
		int randomScore = 1 + (rand() % ScorePlayers);
		ui.recordsTable[name] = randomScore;
	}
}

