#include "GameController.hpp"

#include <cstdlib>

int main() {
	GameController gameController;
	const bool status = gameController.Run();
	if (!status) {
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
