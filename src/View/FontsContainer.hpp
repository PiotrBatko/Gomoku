#ifndef FONTSCONTAINER_HPP
#define FONTSCONTAINER_HPP

#include <unordered_map>
#include <string>

#include "SFML/Graphics/Font.hpp"

class FontsContainer
{
public:

    void LoadFont(const std::string& name, const std::string& pathToFile);
    const sf::Font& GetFont(const std::string& name);

private:

    std::unordered_map<std::string, sf::Font> m_Fonts;
};

extern FontsContainer fontsContainer;

#endif
