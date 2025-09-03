#pragma once

#include "system/beg.hpp"

#include <atomic>
#include <condition_variable>
#include <thread>

class DownloadUpload {
  protected:
    std::atomic<bool> stop_flag{};
    std::atomic<bool> transmition_finished_flag{};
    std::atomic<bool> cancel_finished_flag{};
    std::atomic<bool> info_finished_flag{};
    std::atomic<uint64_t> buffer_count{};
    std::atomic<uint64_t> curr_buffer_idx{};
    std::jthread* info_thread{};
    std::mutex info_mtx{};
    std::condition_variable info_cv{};
    bool is_cancelled_by_esc{};

    void _run();

    void createCancelThread();
    void cancelThreadHandle();

    void createInfoThread();
    void infoThreadHandle(std::stop_token stop_token);

    virtual bool startTransmition() = 0;
    void stopInfoThread();

    void waitForThreadsEnd();

    ~DownloadUpload();
    DownloadUpload() = default;
};
