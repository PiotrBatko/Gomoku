#include "InitialFieldCapturer.hpp"

#include "BotCM.hpp"
#include "../Board.hpp"
#include "../Random.hpp"

namespace CM {

InitialFieldCapturer::InitialFieldCapturer() {
}

InitialFieldCapturer::~InitialFieldCapturer() {
}

void InitialFieldCapturer::Run(Board& board, Player* blackPlayer) {
    //White - BotCM
    //Black - console player
    //m_Board.SetField(5, 4, Field::Black);
    //m_Board.SetField(5, 6, Field::Black);

    //m_Board.SetField(4 , 9, Field::White);
    //m_Board.SetField(5 , 9, Field::White);
    //m_Board.SetField(6 , 9, Field::White);
    //m_Board.SetField(7 , 6, Field::White);
    //m_Board.SetField(8 , 6, Field::Black);
    //m_Board.SetField(9 , 6, Field::Black);
    //m_Board.SetField(10, 6, Field::Black);
    //m_Board.SetField(11, 6, Field::Black);
    //m_Board.SetField(10, 7, Field::Black);
    //m_Board.SetField(12, 5, Field::White);

    //m_Board.SetField(8 , 9, Field::Black);

    //m_Board.SetField(9 , 9, Field::White);
    //m_Board.SetField(10, 9, Field::White);
    //m_Board.SetField(11, 9, Field::White);
    //m_Board.SetField(12, 9, Field::White);

    //m_Board.SetField(16, 16, Field::Black);
    //m_Board.SetField(17, 0, Field::Black);

    ///*
    CM::BotCM* botCM = dynamic_cast<CM::BotCM*>(blackPlayer);
    CM::EmptyFieldsManager& emptyFieldsManager = botCM->GetEmptyFieldsManager();
    emptyFieldsManager.InitializeCollection(board.getWidth(), board.getHeight());

    for (std::size_t i = 3u; i < 13u; ++i) {
        for (std::size_t j = 3u; j < 13u; ++j) {
            if (i >= 7u && i <= 9u && j >= 7u && j <= 9u) {
                continue;
            }
            if (Random::RandomizeInt(2) == 0) {
                board.SetField(i, j, Field::White);
                emptyFieldsManager.SetFieldNotEmpty(Coordinates(i,j));
            } else {
                board.SetField(i, j, Field::Black);
                emptyFieldsManager.SetFieldNotEmpty(Coordinates(i,j));
            }
        }
    }
    board.SetField(7, 7, Field::Black); emptyFieldsManager.SetFieldNotEmpty(Coordinates(7,7));
    board.SetField(7, 8, Field::Black); emptyFieldsManager.SetFieldNotEmpty(Coordinates(7,8));
    board.SetField(7, 9, Field::Black); emptyFieldsManager.SetFieldNotEmpty(Coordinates(7,9));
    board.SetField(8, 7, Field::Black); emptyFieldsManager.SetFieldNotEmpty(Coordinates(8,7));
    board.SetField(8, 9, Field::Black); emptyFieldsManager.SetFieldNotEmpty(Coordinates(8,9));
    board.SetField(9, 7, Field::Black); emptyFieldsManager.SetFieldNotEmpty(Coordinates(9,7));
    board.SetField(9, 8, Field::Black); emptyFieldsManager.SetFieldNotEmpty(Coordinates(9,8));
    board.SetField(9, 9, Field::Black); emptyFieldsManager.SetFieldNotEmpty(Coordinates(9,9));
    //*/
}

}
