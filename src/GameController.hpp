#ifndef _GAME_CONTROLLER_HPP_
#define _GAME_CONTROLLER_HPP_

#include <utility>

#include <SFML/Graphics/RenderWindow.hpp>

#include "Board.hpp"
#include "Coordinates.hpp"
#include "Field.hpp"

class Player;

class GameController {
public:
	GameController();
	virtual ~GameController();

	bool Run();

private:
	Coordinates makePlayerMove(Player* const player,
			 const Field field);

	void drawGameBoard();

	bool Initialize();

	bool createPlayer(const int playerTypeId, Player*& player);

	Board m_Board;

	Player * m_WhitePlayer = nullptr;
	Player * m_BlackPlayer = nullptr;

	// View related members
	static constexpr std::size_t FieldWidthInPixels = 25u;
	static constexpr std::size_t FieldHeightInPixels = FieldWidthInPixels;

	sf::RenderWindow m_Window;
};

#endif
