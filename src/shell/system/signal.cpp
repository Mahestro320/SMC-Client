#include <csignal>
#include "io/console.hpp"
#include "network/client.hpp"
#include "shell.hpp"
#include "shell/system/signal.hpp"

namespace {

inline Shell* g_shell;

void chillHandle(int signal) {
    console::out::inf("signal triggered (" + std::to_string(signal) + "), exiting...");
    Client& client{g_shell->getClient()};
    client.close();
    std::exit(signal);
}

void criticalHandle(int signal) {
    console::out::err("critical error ! signal: " + std::to_string(signal));
    std::exit(signal);
}

} // namespace

void shell::signal::setSignals(Shell* shell) {
    g_shell = shell;

    std::signal(SIGINT, chillHandle);
    std::signal(SIGTERM, chillHandle);
    std::signal(SIGBREAK, chillHandle);

    std::signal(SIGABRT, criticalHandle);
    std::signal(SIGFPE, criticalHandle);
    std::signal(SIGILL, criticalHandle);
    std::signal(SIGSEGV, criticalHandle);
}
