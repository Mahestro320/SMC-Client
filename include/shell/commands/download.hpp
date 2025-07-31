#pragma once

#include <atomic>
#include <condition_variable>
#include <filesystem>
#include <fstream>
#include <shared_mutex>
#include "../command.hpp"
#include "multithread/queue.hpp"
#include "network/request/handlers/io_get_file_content.hpp"
#include "shell/commands/download/info_printer.hpp"

class DownloadCommand final : public Command {
  private:
    static inline constexpr uint64_t MAX_QUEUE_SIZE{0xFF};
    static inline constexpr char ESC_KEY{0x1B};

    std::filesystem::path input_path_raw{}, input_path_complete{}, input_path_real{};
    std::filesystem::path output_path{};
    std::ofstream* output_file{};
    uint64_t buffer_count{}, buffer_size{};
    IOGetFileContentRH handler{};

    bool is_cancelled_by_esc{};
    std::mutex mtx{};
    std::condition_variable cv{};
    Queue<std::vector<char>> queue{};
    DownloadInfoPrinter info_printer{};
    std::atomic<uint64_t> curr_buffer_idx{};
    std::atomic<bool> error_flag{};
    std::atomic<bool> stop_flag{};
    std::atomic<bool> reader_finished_flag{};
    std::atomic<bool> writer_finished_flag{};
    std::atomic<bool> cancel_finished_flag{};

    bool init();
    void initQueue();
    void createThreads();
    void loadBufferSize();
    bool getInputPath();
    bool getRawInputPath();
    bool readCompleteInputPath();
    bool readRealInputPath();
    bool getOutputPath();
    bool createOutputFile();
    bool beginHandler();
    void deleteFileInstance();
    void deleteFileIfNeeded();

    void infoLoop();
    void initInfoPrinter();
    void startInfoLoop();
    void endInfoLoop();
    void printInfoMessage();
    void printInfoMessageNoWait();

    void createReaderThread();
    void readerThreadHandle();
    bool downloadNextBuffer();
    void stopHandler();
    void waitForFreeSpaceInQueue();

    void createWriterThread();
    void writerThreadHandle();
    void waitForQueueToBeFilled();
    bool writeNextBuffer();

    void createCancelThread();
    void cancelThreadHandle();

    bool transmitionFinished();

    std::filesystem::path getNewOutputFileNameInDir(const std::filesystem::path& path) const;

  public:
    DownloadCommand() = default;

    exit_code_t run() override;
};
