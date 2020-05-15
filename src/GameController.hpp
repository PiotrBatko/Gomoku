#ifndef _GAME_CONTROLLER_HPP_
#define _GAME_CONTROLLER_HPP_

#include <utility>

#include "Field.hpp"

class Board;
class Player;
namespace sf {
	class RectangleShape;
	class RenderWindow;
	class CircleShape;
}

class GameController {
public:
	GameController();
	virtual ~GameController();

	bool Run();

private:
	bool makePlayerMove(Board& board,
			 Player* const player,
			 std::size_t& x,
			 std::size_t& y,
			 const Field field);

	void drawGameBoard(Board& board,
			  std::size_t FieldWidthInPixels,
			  std::size_t FieldHeightInPixels,
			  sf::RectangleShape& fieldView,
			  sf::RenderWindow& window,
			  sf::CircleShape& whiteBlockView,
			  sf::CircleShape& blackBlockView);

	bool Initialize();

	bool createPlayer(const int playerTypeId, Player*& player, const Board& board);
};

#endif
