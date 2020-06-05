#ifndef _GAME_CONTROLLER_HPP_
#define _GAME_CONTROLLER_HPP_

#include <memory>
#include <utility>

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include "Board.hpp"
#include "Coordinates.hpp"
#include "Field.hpp"

class Player;

class GameController {
public:

    GameController();
    virtual ~GameController();

    bool Run();

private:

    Coordinates makePlayerMove(Player* const player, const Field field);

    void drawGameBoard();

    bool Initialize();

    std::unique_ptr<Player> createPlayer(const int playerTypeId);

    Board m_Board;

    std::unique_ptr<Player> m_WhitePlayer;
    std::unique_ptr<Player> m_BlackPlayer;

    // View related members
    static constexpr std::size_t FieldWidthInPixels = 25u;
    static constexpr std::size_t FieldHeightInPixels = FieldWidthInPixels;

    sf::RenderWindow m_Window;
    sf::Font m_Font;
};

#endif
