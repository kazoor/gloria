#include <stdexcept>
#include <cstdlib>
#include "core/application/application.hpp"

int main() {
	gloria::core::Application app;

	try {
		app.run();
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}