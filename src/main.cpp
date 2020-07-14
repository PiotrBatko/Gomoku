#include <cstdlib>

#include "AppConfig/FileAppConfigLoader.hpp"
#include "GameController.hpp"

int main()
{
    FileAppConfigLoader fileAppConfigLoader;
    bool result = fileAppConfigLoader.LoadFileAppConfigConstants(fileAppConfigContainer);
    if (!result) {
        return EXIT_FAILURE;
    }

    GameController gameController;
    const bool status = gameController.Run();
    if (!status) {
       return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
