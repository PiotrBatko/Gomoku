#ifndef _BOT_CM_HPP
#define _BOT_CM_HPP

#include "../Player.hpp"

class Board;

namespace CM {

class BotCM : public Player {
public:
	BotCM(Board* gameBoard);
	virtual ~BotCM();

	bool MakeMove(std::size_t& x, std::size_t& y) override;
};

}

#endif
