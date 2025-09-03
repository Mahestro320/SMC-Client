#include "shell/commands/download_upload/download_upload.hpp"

#include <Windows.h>
#include <conio.h>
#include "io/console.hpp"
#include "shell/commands/download_upload/info_printer.hpp"
#include "shell/config.hpp"
#include "shell/system/cursor.hpp"

DownloadUpload::~DownloadUpload() {
    stopInfoThread();
}

void DownloadUpload::_run() {
    shell::cursor::setVisible(false);
    createCancelThread();
    if (!startTransmition()) {
        stop_flag.store(true, std::memory_order_release);
    }
    stopInfoThread();
    waitForThreadsEnd();
    if (is_cancelled_by_esc) {
        console::out::inf("operation cancelled", false);
    }
    shell::cursor::setVisible(true);
}

void DownloadUpload::createCancelThread() {
    std::thread t{[&] { cancelThreadHandle(); }};
    t.detach();
}

void DownloadUpload::cancelThreadHandle() {
    while (!transmition_finished_flag.load(std::memory_order_acquire) && !stop_flag.load(std::memory_order_acquire)) {
        std::this_thread::sleep_for(std::chrono::microseconds(10));
        if (_kbhit() && _getch() == VK_ESCAPE) {
            stop_flag.store(true, std::memory_order_release);
            is_cancelled_by_esc = true;
        }
    }
    cancel_finished_flag.store(true, std::memory_order_release);
}

void DownloadUpload::createInfoThread() {
    info_finished_flag.store(false, std::memory_order_release);
    info_thread = new std::jthread{[&](std::stop_token stop_token) { infoThreadHandle(stop_token); }};
    info_thread->detach();
}

void DownloadUpload::infoThreadHandle(std::stop_token stop_token) {
    const Config& config{Config::getInstance()};
    const ConfigValues& config_values{config.getValues()};
    DownloadUploadInfoPrinter info_printer{};
    info_printer.setBufferSize(config_values.cmd_download_buffer_size);
    while (true) {
        info_printer.setBufferCount(buffer_count.load(std::memory_order_acquire));
        info_printer.setCurrBufferIndex(curr_buffer_idx.load(std::memory_order_acquire));
        info_printer.update();
        info_printer.print();
        std::unique_lock lock{info_mtx};
        if (info_cv.wait_for(lock, std::chrono::seconds{1}, [&] { return stop_token.stop_requested(); })) {
            break;
        }
    }
    info_printer.clear();
    info_finished_flag.store(true, std::memory_order_release);
}

void DownloadUpload::stopInfoThread() {
    if (!info_thread) {
        return;
    }
    info_thread->request_stop();
    info_cv.notify_all();
    while (!info_finished_flag.load(std::memory_order_acquire)) {
        std::this_thread::sleep_for(std::chrono::microseconds{10});
    }
    delete info_thread;
    info_thread = nullptr;
}

void DownloadUpload::waitForThreadsEnd() {
    // we don't check transmition and info flag because at this point, they must not run
    while (!cancel_finished_flag.load(std::memory_order_acquire)) {
        std::this_thread::sleep_for(std::chrono::microseconds{10});
    }
}
