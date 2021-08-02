#include "InitialFieldCapturer.hpp"

#include "../Board.hpp"
#include "../Random.hpp"
#include "../DebugInfo.hpp"
#include "../GameView.hpp"

namespace CM {

InitialFieldCapturer::InitialFieldCapturer() {
    board = nullptr;
    emptyFieldsManager = nullptr;
}

InitialFieldCapturer::~InitialFieldCapturer() {
}

bool InitialFieldCapturer::Run(Board& board, Player* blackPlayer, std::vector<GameView*>& views) {
    this->views = views;
    this->board = &board;

    CM::BotCM* botCM = dynamic_cast<CM::BotCM*>(blackPlayer);
    if (!botCM) {
        LOG_ERROR("botCM == nullptr");
        return false;
    }

    emptyFieldsManager = &(botCM->GetEmptyFieldsManager());
    emptyFieldsManager->InitializeCollection(board.getWidth(), board.getHeight());


    //White - BotCM
    //Black - console player
    bool result = false;
    result = SetFieldOnBoardAndNotifyView(6, 5, Field::Black); if (!result) return false;
    result = SetFieldOnBoardAndNotifyView(9, 5, Field::Black); if (!result) return false;
    //result = SetFieldOnBoardAndNotifyView(6, 5, Field::Black); if (!result) return false;
    //result = SetFieldOnBoardAndNotifyView(10, 5, Field::Black); if (!result) return false;
    //result = SetFieldOnBoardAndNotifyView(11, 8, Field::Black); if (!result) return false;
    //result = SetFieldOnBoardAndNotifyView(10, 9, Field::Black); if (!result) return false;

    //result = SetFieldOnBardAndNotifyView(3, 13, Field::Black); if (!result) return false;
    //result = SetFieldOnBardAndNotifyView(8,  8, Field::Black); if (!result) return false;

    //result = SetFieldOnBardAndNotifyView(5, 6, Field::Black); if (!result) return false;

    //result = SetFieldOnBardAndNotifyView(4 , 9, Field::White); if (!result) return false;
    //result = SetFieldOnBardAndNotifyView(5 , 9, Field::White); if (!result) return false;
    //result = SetFieldOnBardAndNotifyView(6 , 9, Field::White); if (!result) return false;
    //result = SetFieldOnBardAndNotifyView(7 , 6, Field::White); if (!result) return false;
    //result = SetFieldOnBardAndNotifyView(8 , 6, Field::Black); if (!result) return false;
    //result = SetFieldOnBardAndNotifyView(9 , 6, Field::Black); if (!result) return false;
    //result = SetFieldOnBardAndNotifyView(10, 6, Field::Black); if (!result) return false;
    //result = SetFieldOnBardAndNotifyView(11, 6, Field::Black); if (!result) return false;
    //result = SetFieldOnBardAndNotifyView(10, 7, Field::Black); if (!result) return false;
    //result = SetFieldOnBardAndNotifyView(12, 5, Field::White); if (!result) return false;

    //result = SetFieldOnBardAndNotifyView(8 , 9, Field::Black); if (!result) return false;

    //result = SetFieldOnBardAndNotifyView(9 , 9, Field::White); if (!result) return false;
    //result = SetFieldOnBardAndNotifyView(10, 9, Field::White); if (!result) return false;
    //result = SetFieldOnBardAndNotifyView(11, 9, Field::White); if (!result) return false;
    //result = SetFieldOnBardAndNotifyView(12, 9, Field::White); if (!result) return false;

    //result = SetFieldOnBardAndNotifyView(16, 16, Field::Black); if (!result) return false;
    //result = SetFieldOnBardAndNotifyView(17, 0, Field::Black); if (!result) return false;

    /*
    for (std::size_t i = 3u; i < 13u; ++i) {
        for (std::size_t j = 3u; j < 13u; ++j) {
            if (i >= 7u && i <= 9u && j >= 7u && j <= 9u) {
                continue;
            }
            if (Random::RandomizeInt(2) == 0) {
                result = SetFieldOnBardAndNotifyView(i, j, Field::White); if (!result) return false;
            } else {
                result = SetFieldOnBardAndNotifyView(i, j, Field::Black); if (!result) return false;
            }
        }
    }
    result = SetFieldOnBardAndNotifyView(7, 7, Field::Black); if (!result) return false;
    result = SetFieldOnBardAndNotifyView(7, 8, Field::Black); if (!result) return false;
    result = SetFieldOnBardAndNotifyView(7, 9, Field::Black); if (!result) return false;
    result = SetFieldOnBardAndNotifyView(8, 7, Field::Black); if (!result) return false;
    result = SetFieldOnBardAndNotifyView(8, 9, Field::Black); if (!result) return false;
    result = SetFieldOnBardAndNotifyView(9, 7, Field::Black); if (!result) return false;
    result = SetFieldOnBardAndNotifyView(9, 8, Field::Black); if (!result) return false;
    result = SetFieldOnBardAndNotifyView(9, 9, Field::Black); if (!result) return false;
    */
    return true;
}

bool InitialFieldCapturer::SetFieldOnBoardAndNotifyView(const std::size_t x, const std::size_t y, Field field) {
    bool result = board->SetField(x, y, field);
    if (!result) return false;

    result = emptyFieldsManager->SetFieldNotEmpty(Coordinates(x, y));
    if (!result) return false;

    for (auto view : views) {
        PawnColor pawnColor;
        if (field == Field::White) {
            pawnColor = PawnColor::White;
        } else {
            pawnColor = PawnColor::Black;
        }

        view->PawnAdded(pawnColor, Coordinates(x, y));
    }
    return true;
}

}
