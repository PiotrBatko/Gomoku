#include "GameView.hpp"

void GameView::SetGameModel(GameModel& model)
{
    m_Model = &model;
}

GameModel& GameView::GetGameModel() const
{
    return *m_Model;
}
