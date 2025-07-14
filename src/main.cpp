/*
 * SMC Client (Super Mega Cool Client)
 * created the 18/06/25
 *
 * by Mahestro_320
 */

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
    if (values.client_connect_at_launch) {
        client.connect();
    }
}

void processNewCommand(Shell& shell) {
    try {
        shell.processNewCommand();
    } catch (const std::bad_alloc& e) {
        console::out::err("memory error: " + std::string{e.what()});
    } catch (const std::exception& e) {
        console::out::err("unhandled exception: " + std::string{e.what()});
    }
}

} // namespace

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
        processNewCommand(shell);
    }
    client.close();
    return 0;
}
