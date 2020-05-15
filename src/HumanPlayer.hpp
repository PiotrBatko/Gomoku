#ifndef _BOT_HUMAN_PLAYER_HPP
#define _BOT_HUMAN_PLAYER_HPP

#include "Player.hpp"

class Board;

class HumanPlayer : public Player {
public:
	HumanPlayer(const Board* gameBoard);
	virtual ~HumanPlayer();

	bool MakeMove(std::size_t& x, std::size_t& y) override;

private:
	unsigned int turnCount;
};

#endif
