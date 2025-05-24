#include <cstdlib>
#include <iostream>
#include <signal.h>

#include <spdlog/spdlog.h>

#include "governor.hpp"
#include "oberon.hpp"

const std::string help_text =
	"oberon-governor v" + std::string(GOVERNOR_VERSION) + "\n"
	"This program takes no arguments and must be run as root on an AMD Oberon system, on a systemd distro it can be run through oberon-governor.service\n"
	"Currently the only tested system is the ASRock BC-250.\n";

Governor* g;

void stop(int signal) {
	if (g)
		g->stop();
}

int main(int argc, char *argv[]) {
	for (int i = 1; i < argc; i++) {
		bool found = false;

		// Help
		for (const std::string& a : {"-h", "--help"}) {
			if (a == argv[i]) {
				found = true;
				break;
			}
		}
		if (found) { // Print help text and exit
            spdlog::info(help_text);
			std::exit(EXIT_SUCCESS);
		}

		// Unknown argument
        spdlog::error("Unknown argument '{}'", argv[i]);
		std::exit(EXIT_FAILURE);
	}

	Oberon oberon;
	Governor governor(oberon);

	g = &governor;
	for (const int s : { SIGHUP, SIGINT, SIGQUIT, SIGTERM })
		signal(s, stop);

    spdlog::info("Starting governor");
	governor.run();
    spdlog::info("Stopping governor");
}
