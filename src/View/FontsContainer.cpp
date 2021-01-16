#include "FontsContainer.hpp"

#include <stdexcept>

FontsContainer fontsContainer;

void FontsContainer::LoadFont(const std::string& name, const std::string& pathToFile)
{
    sf::Font font;
    if (not font.loadFromFile(pathToFile))
    {
        throw std::runtime_error("Cannot load font from file " + pathToFile);
    }

    m_Fonts[name] = font;
}

const sf::Font& FontsContainer::GetFont(const std::string& name)
{
    if (m_Fonts.count(name) > 0)
    {
        return m_Fonts[name];
    }

    throw std::runtime_error("Inavalible font " + name + " requested");
}
