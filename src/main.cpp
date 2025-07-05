/*
 * SMC Client (Super Mega Cool Client)
 * created the 18/06/25
 *
 * by Mahestro_320
 */

#include <array>
#include "common.hpp"
#include "io/console.hpp"
#include "network/client.hpp"
#include "shell.hpp"
#include "shell/config.hpp"

namespace {

bool init() {
    Config& config{Config::getInstance()};
    return config.load();
}

void connectIfNeeded(Client& client) {
    Config& config{Config::getInstance()};
    const ConfigValues& values{config.getValues()};
    if (values.connect_at_launch) {
        client.connect();
    }
}

} // namespace

// TESTER CD AVEC DES CHARS RANDOM

int main(int /* argc */, char** /* argv */) {
    if (!init()) {
        return 1;
    }
    Client client{};
    connectIfNeeded(client);
    console::out::inf();

    Shell shell{client};
    shell.start();
    while (common::is_running) {
        try {
            shell.processNewCommand();
        } catch (const std::exception& e) {
            console::out::err("error while processing a command: " + std::string{e.what()});
        }
    }
    client.closeSocket();
    return 0;
}
