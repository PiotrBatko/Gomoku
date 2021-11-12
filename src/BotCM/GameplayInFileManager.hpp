#ifndef _BOTCM_GAMEPLAYINFILEMANAGER_HPP_
#define _BOTCM_GAMEPLAYINFILEMANAGER_HPP_

#include <iostream>
#include <fstream>
#include <string>

class Coordinates;

namespace CM {

class GameplayInFileManager {
public:
    GameplayInFileManager();
    virtual ~GameplayInFileManager();

    void SaveToFile(const Coordinates& coordinates);
    Coordinates LoadFromFile();

private:
    void openInputFileIfNotOpen();
    void openOutputFileIfNotOpen();
    Coordinates getCoordinatesFromReadLine(const std::string& line);

    std::ifstream inputFile;
    std::ofstream outputFile;

    bool inputFileOpen;
    bool outputFileOpen;

    static const std::string gameplayFileName;
};

}

#endif
