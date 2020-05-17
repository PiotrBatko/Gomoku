#ifndef _BOT_CONSOLE_PLAYER_HPP
#define _BOT_CONSOLE_PLAYER_HPP

#include "Player.hpp"

class Board;

class ConsolePlayer : public Player {
public:
	ConsolePlayer(const Board* gameBoard);
	virtual ~ConsolePlayer();

	bool MakeMove(std::size_t& x, std::size_t& y) override;

private:
	unsigned int turnCount;
};

#endif
