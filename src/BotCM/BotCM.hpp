#ifndef _BOT_CM_HPP
#define _BOT_CM_HPP

#include "../Player.hpp"

class Board;

namespace CM {

class BotCM : public Player {
public:
	BotCM(const Board* gameBoard, PlayerType playerType, Field playerColor);
	virtual ~BotCM();

	void NotifyAboutOpponentMove(Coordinates opponentMove) override;
	Coordinates MakeMove() override;

private:
	bool verifyAllocationCounter();
};

}

#endif
