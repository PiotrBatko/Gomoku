#ifndef _BOT_PB_HPP
#define _BOT_PB_HPP

#include "../Player.hpp"

class Board;

namespace PB {

class BotPB : public Player {
public:
	BotPB(const Board* gameBoard);
	virtual ~BotPB();

	bool MakeMove(std::size_t& x, std::size_t& y) override;
};

}

#endif
