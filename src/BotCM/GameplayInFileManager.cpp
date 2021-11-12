#include <BotCM/GameplayInFileManager.hpp>

#include "../Coordinates.hpp"
#include "../DebugInfo.hpp"

namespace CM {

const std::string GameplayInFileManager::gameplayFileName = "gameplay.txt";

GameplayInFileManager::GameplayInFileManager():
    inputFileOpen(false),
    outputFileOpen(false) {
}

GameplayInFileManager::~GameplayInFileManager() {
    if (inputFileOpen) {
        inputFile.close();
    }
    if (outputFileOpen) {
        outputFile.close();
    }
}

void GameplayInFileManager::SaveToFile(const Coordinates& coordinates) {
    openOutputFileIfNotOpen();
    outputFile << coordinates.x << " " << coordinates.y << std::endl;
}

Coordinates GameplayInFileManager::LoadFromFile() {
    openInputFileIfNotOpen();

    std::string line;
    if (!std::getline(inputFile, line)) {
        // Here we know that we have reached the end of the text file.
        LOG_ERROR("End of the file with the battle movements reached.");
        exit(1);
    }
    Coordinates coordinates = getCoordinatesFromReadLine(line);
    return coordinates;
}

Coordinates GameplayInFileManager::getCoordinatesFromReadLine(const std::string& line) {
    unsigned short spacesCount = 0u;
    unsigned short spaceIndex = 0u;
    for (unsigned short i = 0u; i < line.size(); ++i) {
        const char c = line[i];
        if (c == ' ') {
            spacesCount++;
            spaceIndex = i;
        }
    }
    if (spacesCount != 1u) {
        LOG_ERROR("Spaces count other than 1 in read line.");
        throw std::exception();
    }

    const std::string xString = line.substr(0u, spaceIndex);
    const std::string yString = line.substr(spaceIndex + 1, line.size() - spaceIndex - 1u);

    Coordinates coordinates;
    try {
        coordinates.x = static_cast<std::size_t>(std::stoi(xString));
        coordinates.y = static_cast<std::size_t>(std::stoi(yString));
    } catch (std::exception& e) {
        LOG_ERROR("Wrong values in the read line.");
        throw e;
    }
    return coordinates;
}

void GameplayInFileManager::openOutputFileIfNotOpen() {
    if (!outputFileOpen) {
        outputFile.open(gameplayFileName);
        outputFileOpen = true;
    }
}

void GameplayInFileManager::openInputFileIfNotOpen() {
    if (!inputFileOpen) {
        inputFile.open(gameplayFileName);
        if (!inputFile) {
            LOG_ERROR("File ", gameplayFileName, " not found.");
            throw std::exception();
        }
        inputFileOpen = true;
    }
}

}
