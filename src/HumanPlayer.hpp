#ifndef _BOT_CM_HPP
#define _BOT_CM_HPP

#include "Player.hpp"

class Board;

class HumanPlayer : public Player {
public:
	HumanPlayer(Board* gameBoard);
	virtual ~HumanPlayer();

	bool MakeMove(std::size_t& x, std::size_t& y) override;
};

#endif
