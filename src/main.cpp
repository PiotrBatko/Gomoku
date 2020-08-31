#include <cstdlib>
#include <thread>

#include "AppConfig/FileAppConfigLoader.hpp"
#include "GameController.hpp"
#include "View/FontsContainer.hpp"
#include "View/GomokuView.hpp"

int main()
{
    FileAppConfigLoader fileAppConfigLoader;
    bool result = fileAppConfigLoader.LoadFileAppConfigConstants(fileAppConfigContainer);
    if (!result) {
        return EXIT_FAILURE;
    }

    fontsContainer.LoadFont("default", "res/fonts/Ubuntu-L.ttf");

    GomokuView view;

    GameController gameController;
    gameController.RegisterView(view);

    const bool status = gameController.Run();
    if (!status) {
       return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
