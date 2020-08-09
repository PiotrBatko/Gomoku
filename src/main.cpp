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
    std::thread viewThread(
        [&view]()
        {
            view.Run();
        }
    );

    viewThread.join();
    return 0;

    // Temporarily stop GameController creation and running

    GameController gameController;
    const bool status = gameController.Run();
    if (!status) {
       return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
