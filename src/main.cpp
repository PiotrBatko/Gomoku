#include <cstdlib>

#include "AppConfig/FileAppConfigLoader.hpp"
#include "GameController.hpp"
#include "View/GomokuView.hpp"

int main()
{
    FileAppConfigLoader fileAppConfigLoader;
    bool result = fileAppConfigLoader.LoadFileAppConfigConstants(fileAppConfigContainer);
    if (!result) {
        return EXIT_FAILURE;
    }

    GomokuView view;
    view.Run();
    return 0;

    // Temporarily stop GameController creation and running

    GameController gameController;
    const bool status = gameController.Run();
    if (!status) {
       return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
