#ifndef _BOT_RANDOMIZER_H_
#define _BOT_RANDOMIZER_H_

#include "Player.hpp"

class Board;

class BotRandomizer : public Player {
public:
	BotRandomizer(const Board* gameBoard);
	virtual ~BotRandomizer();

	bool MakeMove(std::size_t& x, std::size_t& y) override;
};

#endif
